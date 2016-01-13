#include <core/Object.h>
#include <memory.h>
#include <stdio.h>
#include <typeinfo>
//#include <es_util.h>
//#include <config.h>
#include <es_assert.h>
#include <core/FreeListAllocator.h>
#include <malloc.h>
#include <string.h>
#include <core/log.h>
#include <core/RefCounter.h>

#define ERROR_PRINT_FUNC printf
#define DEBUG_PRINT_FUNC printf


extern core::RefCounter refs;


void* engineAllocMem(core::Allocator* alloc, size_t size, size_t alignment)
{
	return alloc->allocate(size,alignment);
};

void engineFreeMem(core::Allocator* alloc, void* p)
{
	alloc->deallocate(p);
};

void* engineAllocMem(size_t size)
{
	return engineAllocMem(core::getDefaultAllocator(), size, 16);
}

void* engineAllocMem(size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
	return engineAllocMem(size);
}

void* engineAllocMem(size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
	return engineAllocMem(size);
}

void engineFreeMem(void* p)
{
	engineFreeMem(core::getDefaultAllocator(), p);
}

void * engineReallocMem(void *ud, void *ptr, size_t x, size_t s)
{
	core::Allocator* alloc = (core::Allocator*)ud;
	assert(alloc != 0);
	assert( s >= 0);

	if( s == 0 )
	{
		if( ptr )
			engineFreeMem(alloc,ptr);
		return NULL;
	}
	
	if( s <= x )
		return ptr;


	void *newPtr = engineAllocMem(alloc,s);

	if( ptr != NULL )
	{
		memcpy(newPtr,ptr,x);
		engineFreeMem(alloc,ptr);
	}
	
	return newPtr;
}


void* engineAllocMemString(size_t size)
{
	return engineAllocMem(core::getStringAllocator(), size, 4);
}

void* engineAllocMemString(size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
	return engineAllocMemString(size);
}

void* engineAllocMemString(size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{	
	return engineAllocMemString(size);
}

void engineFreeMemString(void* p)
{
	engineFreeMem(core::getStringAllocator(), p);
}

namespace core
{

Allocator* getDefaultAllocator()
{
	return refs.allocator;
}


Allocator* getStringAllocator()
{
	return refs.stringAllocator;
}

Allocator* getLuaAllocator()
{
	return refs.luaAllocator;
}

Object::Object()
: m_numOfRefs(0)
{
    refs.add(this,"");
}

Object::~Object()
{
    refs.release(this);
    if( this->m_numOfRefs != 0 )
    {
        assert( this->m_numOfRefs == 0 ); // "Can not delete Object, when it have references some where else";
    }
}

void* Object::operator new (std::size_t size)
{
	return getDefaultAllocator()->allocate(size,16);
}

void* Object::operator new (std::size_t size, void* ptr)
{
	Allocator* alloc = (Allocator*)ptr;
	return alloc->allocate(size,16);
}

void Object::operator delete (void* ptr)
{
	getDefaultAllocator()->deallocate(ptr);	
}

void Object::operator delete (void* ptr, void* voidptr2)
{
	Allocator* alloc = (Allocator*)voidptr2;
	return alloc->deallocate(ptr);
}

void* Object::operator new[] (std::size_t size)
{
	return getDefaultAllocator()->allocate(size,16);
}

void* Object::operator new[] (std::size_t size, void* ptr)
{
	Allocator* alloc = (Allocator*)ptr;
	return alloc->allocate(size,16);
}

void Object::operator delete[] (void* ptr)
{
	return getDefaultAllocator()->deallocate(ptr);
}

void Object::operator delete[] (void* ptr, void* voidptr2)
{
	Allocator* alloc = (Allocator*)voidptr2;
	return alloc->deallocate(ptr);
}

}







