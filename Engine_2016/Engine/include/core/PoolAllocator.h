#ifndef _MEMORY_POOLALLOCATOR_H_
#define _MEMORY_POOLALLOCATOR_H_

#include "Allocator.h"

namespace core
{

class PoolAllocator : public Allocator
{
public:
	PoolAllocator(uint32_t objectSize, uint8_t objectAlignment, uint32_t size, void* pMem);
	virtual ~PoolAllocator();

	virtual void* allocate(uint32_t size, uint8_t alignment);
		
	virtual void deallocate(void* p);

private:
	PoolAllocator();
	PoolAllocator(const PoolAllocator&);
	PoolAllocator& operator=(const PoolAllocator&);

	uint32_t		m_size;
	uint32_t		m_objectSize;
	uint8_t			m_objectAlignment;
	void**			m_freeList;
};

}

#endif

