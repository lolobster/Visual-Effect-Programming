#ifndef _MEMORY_FREELISTALLOCATOR_H_
#define _MEMORY_FREELISTALLOCATOR_H_

#include "Allocator.h"

namespace core
{

class FreeListAllocator : public Allocator
{
public:
	struct FreeBlock
	{
		uint32_t	size;
		FreeBlock*	next;
	};

	FreeListAllocator(uint32_t size, void* start);
	virtual ~FreeListAllocator();

	virtual void* allocate(uint32_t size, uint8_t alignment);
		
	virtual void deallocate(void* p);

	virtual FreeBlock* getFreelist() { return m_freeBlocks; }

	int getFreeNodeCount()
	{
		int res = 0;
		FreeBlock* block = m_freeBlocks;
		while(block)
		{
			++res;
			block = block->next;
		}

		if( res == 2 )
			return res;
		return res;
	}

	int getFreeMemory()
	{
		int res = 0;
		FreeBlock* block = m_freeBlocks;
		while(block)
		{
			res += block->size;
			block = block->next;
		}

		return res;
	}

private:

	struct AllocationHeader
	{
		uint32_t	size;
		uint32_t	adjustment;
	};



	FreeListAllocator();
	FreeListAllocator(const FreeListAllocator&);
	FreeListAllocator& operator=(const FreeListAllocator&);

	FreeBlock*	m_freeBlocks;
};

}

#endif

