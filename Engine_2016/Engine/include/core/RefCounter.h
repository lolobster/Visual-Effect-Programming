#ifndef ENGINE_REFCOUNTER_H
#define ENGINE_REFCOUNTER_H
#include <core/log.h>
#include <core/FreeListAllocator.h>

//#define MEMORY_LEAK_DEBUGGING
#define SHOW_LEAKS
//#define ASSERT_ON_LEAKS


namespace core
{
	class Object;

    class RefCounter
    {
    public:
        RefCounter(int memorySize, int stringMemorySize, int luaMemSize = 0)
            : refs(0)
			, allocator(0)
			, stringAllocator(0)
			, luaAllocator(0)
        {
			LOG_INFO("Allocating memory: %2.2f MB, string allocator: %2.2f MB, LUA allocator: %2.2f MB",
				float(float(memorySize)/(1024*1024)),
				float(float(stringMemorySize)/(1024*1024)),
				float(float(luaMemSize)/(1024*1024)) );
			m_data = (uint8_t*) new FreeListAllocator::FreeBlock[memorySize+stringMemorySize+luaMemSize / sizeof(FreeListAllocator::FreeBlock)];
			memset(m_data,0,memorySize);
			allocator = new FreeListAllocator(memorySize, m_data);

			if( stringMemorySize > 0 )
				stringAllocator = new FreeListAllocator(stringMemorySize, m_data+memorySize);

			if( luaMemSize > 0 )
				luaAllocator = new FreeListAllocator(luaMemSize, m_data+memorySize+stringMemorySize);
        }


        ~RefCounter()
        {
			printAllocations();
			delete allocator;
			
			if( stringAllocator ) delete stringAllocator;
			if( luaAllocator ) delete luaAllocator;

			delete [] m_data;
#if defined(SHOW_LEAKS)
            if( refs != 0 )
            {
				LOG_ERROR("[%s] %d Memory leaks detected!", __FUNCTION__, refs);
#if defined(MEMORY_LEAK_DEBUGGING)
                for( size_t i=0; i<m_objects.size(); ++i )
                {
                    ERROR_PRINT_FUNC(m_objectNames[i].c_str());
                }
#endif
            }
            else
            {
				LOG_INFO("No memory leaks detected!");
            }
#if defined(ASSERT_ON_LEAKS)
            assert( refs == 0 );
#endif
#endif
        }

		void printAllocator(const char* const name, FreeListAllocator* a)
		{
			if( a == 0 ) return;
			int scale = 1024;
			LOG_INFO("|%-6s:%10d|%10d|%10d|%11d|%10d|", name,
				a->getUsedMemory()/scale,
				(a->getFreeMemory()+a->getUsedMemory())/scale,
				a->getMaxUsedMemory()/scale,
				a->getNumAllocations(),
				a->getFreeNodeCount() );
		}
		void printAllocations()
		{
			LOG_INFO("|--------------------- Memory usage (kB) ----------------------|");
			LOG_INFO("|Pool     Mem used| Mem total| Max usage|# of allocs|Free nodes|");
			LOG_INFO("|--------------------------------------------------------------|");
			printAllocator("Mem", allocator);
			printAllocator("String", stringAllocator);
			printAllocator("LUA", luaAllocator);			
			LOG_INFO("|--------------------------------------------------------------|");
			LOG_INFO("");
		}

		void add(Object* o, const char* const name)
        {
#if defined(MEMORY_LEAK_DEBUGGING)
            m_objects.push_back(o);
            m_objectNames.push_back(name);
#else
			(void)name;
			(void)o;
#endif
            ++refs;
        }

		void release(core::Object* o)
        {
#if defined(MEMORY_LEAK_DEBUGGING)
            size_t index = 0;
			
			for( ; index<m_objects.size(); ++index )
			{
				if( m_objects[index] == o )
				{
					break;
				}
			}

			m_objects.erase( m_objects.begin() + index );
			m_objectNames.erase( m_objectNames.begin() + index );
#else
			(void)o;
#endif
            --refs;
        }

    private:
#if defined(MEMORY_LEAK_DEBUGGING)
        std::vector<Object*> m_objects;
        std::vector<eastl::string> m_objectNames;
#endif
        int refs;
		uint8_t* m_data;
	public:
		FreeListAllocator* allocator;
		FreeListAllocator* stringAllocator;
		FreeListAllocator* luaAllocator;
		
    };

   
}

extern core::RefCounter refs;






#endif

