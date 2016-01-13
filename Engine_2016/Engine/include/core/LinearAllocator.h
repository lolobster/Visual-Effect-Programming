#ifndef _MEMORY_LINEARALLOCATOR_H_
#define _MEMORY_LINEARALLOCATOR_H_

#include "Allocator.h"

namespace core
{

class LinearAllocator : public Allocator
{
public:
	LinearAllocator(uint32_t size, void* start);
	virtual ~LinearAllocator();

	virtual void* allocate(uint32_t size, uint8_t alignment);
		
	virtual void deallocate(void* p);

	void clear();

private:
	LinearAllocator();
	LinearAllocator(const LinearAllocator&);
	LinearAllocator& operator=(const LinearAllocator&);

	void*		m_initialPosition;
	void*		m_currentPosition;
	uint32_t	m_size;
};

}

#endif

