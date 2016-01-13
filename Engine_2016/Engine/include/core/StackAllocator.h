#ifndef _MEMORY_STACKALLOCATOR_H_
#define _MEMORY_STACKALLOCATOR_H_

#include "Allocator.h"

namespace core
{

class StackAllocator : public Allocator
{
public:
	StackAllocator(uint32_t size, void* pStart);
	virtual ~StackAllocator();

	virtual void* allocate(uint32_t size, uint8_t alignment);
		
	virtual void deallocate(void* p);

private:
	StackAllocator();
	StackAllocator(const StackAllocator&);
	StackAllocator& operator=(const StackAllocator&);

	struct AllocationHeader
	{
#if _DEBUG
		void*	prevAddress;
#endif
		uint8_t	adjustment;
	};

	void*		m_initialPosition;
#if _DEBUG
	void*		m_prevPosition;
#endif
	void*		m_currentPosition;
	uint32_t	m_size;
};

}

#endif

