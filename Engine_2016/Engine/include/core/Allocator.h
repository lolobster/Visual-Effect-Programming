#ifndef _MEMORY_ALLOCATOR_H_
#define _MEMORY_ALLOCATOR_H_

#include <EABase/eabase.h>
//#include <stdint.h>

namespace core
{

class Allocator
{
public:
	Allocator(void* rawData);

	virtual ~Allocator();

	virtual void* allocate(uint32_t size, uint8_t alignment) = 0;

	virtual void deallocate(void* p) = 0;

	uint32_t getUsedMemory();
	uint32_t getMaxUsedMemory();

	uint32_t getNumAllocations();
	
	void* getRawData()
	{
		return m_rawData;
	}

private:
	Allocator();
	Allocator(const Allocator&);
	Allocator& operator=(const Allocator&);

	uint32_t		m_maxUsedMemory;
	uint32_t		m_usedMemory;
	uint32_t		m_numAllocations;
	
protected:
	void*			m_rawData;

	void allocate(uint32_t size);

	void deallocate(uint32_t size);

	void clear();
};

Allocator* getDefaultAllocator();
Allocator* getStringAllocator();

}

#endif

