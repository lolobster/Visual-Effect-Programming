#include <core/Allocator.h>
#include <core/FreeListAllocator.h>
#include <core/LinearAllocator.h>
#include <core/PoolAllocator.h>
#include <core/StackAllocator.h>
#include <core/log.h>
#include <es_assert.h>

#include <stdio.h>
#include <memory.h>
#include <malloc.h>

namespace core
{
#if(ANDROID)
#define nullptr 0
#endif

	namespace
	{
		typedef unsigned int uint;

		typedef uint8_t u8;
		typedef uint16_t u16;
		typedef uint32_t u32;
		typedef uint64_t u64;

		typedef uintptr_t uptr;

		void doMerge(FreeListAllocator::FreeBlock* block, int count)
		{
			while( block != 0 && count > 0 )
			{
				--count;
				if( (unsigned char*)(block)+block->size == (unsigned char*)block->next )
				{
					uint32_t	delSize = block->next->size;
					FreeListAllocator::FreeBlock*	delNext = block->next->next;
					block->next->size = 0;
					block->next->next = 0;

					block->size += delSize;
					block->next = delNext;
				}
				else
				{
					block = block->next;
				}
			}
		}

		inline u8 alignAdjustment(void* pAddress, u8 alignment)
		{
			u8 adjustment =  alignment - ( (uptr)pAddress & (alignment-1) );
    
			if(adjustment == alignment)
				return 0; //already aligned
    
			return adjustment;
		}

		inline uint8_t alignAdjustmentWithHeader(void* pAddress, u8 alignment, u8 headerSize)
		{
			uint8_t adjustment =  alignment - ( (uptr)pAddress & (alignment-1) );
    
			if(adjustment == alignment)
			{
				adjustment = 0; //already aligned
			}
			else
			{
				int i=0;
			}

			u8 neededSpace = headerSize;

			if(adjustment < neededSpace)
			{
				neededSpace -= adjustment;

				adjustment += alignment * (neededSpace / alignment);

				if(neededSpace % alignment > 0)
					adjustment += alignment;
			}
    
			return adjustment;
		}
	}

// Allocator
Allocator::Allocator(void*			rawData)
{
	m_rawData = rawData;
	m_usedMemory     = 0;
	m_maxUsedMemory	 = 0;
	m_numAllocations = 0;
	m_maxUsedMemory = 0;
}

Allocator::~Allocator()
{
	assert(m_numAllocations == 0 && m_usedMemory == 0);
}


uint32_t Allocator::getUsedMemory()
{
	return m_usedMemory;
}

uint32_t Allocator::getMaxUsedMemory()
{
	return m_maxUsedMemory;
}

uint32_t Allocator::getNumAllocations()
{
	return m_numAllocations;
}

void Allocator::allocate(uint32_t size)
{
	m_usedMemory += size;
	++m_numAllocations;

	if( m_usedMemory > m_maxUsedMemory )
	{
		m_maxUsedMemory = m_usedMemory; 
	}
}

void Allocator::deallocate(uint32_t size)
{
	m_usedMemory -= size;
	--m_numAllocations;
}

void Allocator::clear()
{
	m_numAllocations     = 0;
	m_usedMemory         = 0;
}

typedef unsigned char* data_ptr;

// FreeListAllocator
FreeListAllocator::FreeListAllocator(uint32_t size, void* pStart) 
	: Allocator(pStart), m_freeBlocks((FreeBlock*)pStart)
{
	assert(size > sizeof(FreeBlock));

	m_freeBlocks->size  = size;
	m_freeBlocks->next = nullptr;
}

FreeListAllocator::~FreeListAllocator()
{
	m_freeBlocks        = nullptr;
}

void* FreeListAllocator::allocate(uint32_t size, uint8_t alignment)
{
	if( alignment < __alignof(AllocationHeader) )
	{
		alignment = __alignof(AllocationHeader);
	}

	u8 modulus =  alignment - ( (uptr)size & (alignment-1) );

	size += modulus;
	assert( (size % alignment) == 0 );

	assert(size != 0);

	//Check free blocks
	FreeBlock* pPrevFreeBlock = nullptr;
	FreeBlock* pFreeBlock     = m_freeBlocks;

	while(pFreeBlock)
	{
		{
			uint8_t adjustment = alignAdjustmentWithHeader(pFreeBlock, alignment, sizeof(AllocationHeader));
			size += adjustment;
//			assert( (size % alignment) == 0 );
		}

		if(pFreeBlock->size < size)
		{
			pPrevFreeBlock = pFreeBlock;
			pFreeBlock = pFreeBlock->next;
			continue;
		}

		assert(sizeof(AllocationHeader) == sizeof(FreeBlock));

		//If allocations in the remaining memory will be impossible
		if(pFreeBlock->size - size <= sizeof(AllocationHeader))
		{
			//Increase allocation size instead of creating a new FreeBlock
	//		assert( (pFreeBlock->size % alignment) == 0 );
			size = pFreeBlock->size;

			if(pPrevFreeBlock != nullptr)
				pPrevFreeBlock->next = pFreeBlock->next;
			else
				m_freeBlocks = pFreeBlock->next;
		}
		else
		{
			//Else create a new FreeBlock containing remaining memory
			FreeBlock* pNextBlock = (FreeBlock*)( (uptr)pFreeBlock + size );
			pNextBlock->size = pFreeBlock->size - size;
			pNextBlock->next = pFreeBlock->next;
			
			if(pPrevFreeBlock != nullptr)
				pPrevFreeBlock->next = pNextBlock;
			else
				m_freeBlocks = pNextBlock;
		}

		uptr alignedAddress = (uptr)pFreeBlock + sizeof(AllocationHeader);
	
		AllocationHeader* pHeader = (AllocationHeader*)(alignedAddress - sizeof(AllocationHeader));
		pHeader->size             = size;
		pHeader->adjustment       = sizeof(AllocationHeader);
	
		Allocator::allocate(size);
		
		void* res = (void*)alignedAddress;
		return res;
	}

	LOG_ERROR("Couldn't find free block large enough! (size=%d)", size);
	assert(false && "Couldn't find free block large enough!");

	return nullptr;
}



void FreeListAllocator::deallocate(void* p)
{
	assert(p != 0);
	typedef unsigned char* uptr;

	uptr blockStart;
	uint32_t blockSize;

	{
		AllocationHeader* pHeader = (AllocationHeader*)((uptr)p - sizeof(AllocationHeader));

		uint32_t size = pHeader->size;
		blockStart = (uptr)p - pHeader->adjustment;
		blockSize   = size;
		pHeader->adjustment = 0;
		pHeader->size = 0;
	}

	FreeBlock* prevFreeBlock = 0;
	FreeBlock* nextFreeBlock = m_freeBlocks;

	while( (uptr)nextFreeBlock < (blockStart) )
	{
		prevFreeBlock = nextFreeBlock;
		nextFreeBlock = nextFreeBlock->next;
		assert( (uptr)prevFreeBlock < blockStart);
	}

	assert( (uptr)nextFreeBlock >= (blockStart) );

	if( prevFreeBlock == 0 )
	{
		FreeBlock* newFreeBlock = (FreeBlock*) blockStart;
		newFreeBlock->next = m_freeBlocks;
		newFreeBlock->size = blockSize;
		m_freeBlocks = newFreeBlock;
		doMerge(m_freeBlocks,3);
	}
	else
	{
		assert( prevFreeBlock->next == nextFreeBlock);
		FreeBlock* newFreeBlock = (FreeBlock*) blockStart;
		newFreeBlock->next = 0;
		newFreeBlock->size = blockSize;

		newFreeBlock->next = nextFreeBlock;
		prevFreeBlock->next = newFreeBlock;
		doMerge(prevFreeBlock,3);
	}

	Allocator::deallocate(blockSize);
}


// LinearAllocator
LinearAllocator::LinearAllocator(uint32_t size, void* pStart) 
	: Allocator(pStart), m_size(size), m_initialPosition(pStart), m_currentPosition(pStart)
{
	assert(size > 0);
}

LinearAllocator::~LinearAllocator()
{
	m_initialPosition   = nullptr;
	m_currentPosition   = nullptr;
	m_size               = 0;
}

void* LinearAllocator::allocate(uint32_t size, uint8_t alignment)
{
	assert(size != 0);

	uint8_t adjustment =  alignAdjustment(m_currentPosition, alignment);

	if(getUsedMemory() + adjustment + size > m_size)
		return nullptr;

	uintptr_t alignedAddress = (uintptr_t)m_currentPosition + adjustment;

	m_currentPosition = (void*)(alignedAddress + size);

	
	Allocator::allocate(size + adjustment);

	return (void*)alignedAddress;
}

void LinearAllocator::deallocate(void* p)
{
	assert( false && "Use clear() instead" );
}

void LinearAllocator::clear()
{
	Allocator::clear();

	m_currentPosition   = m_initialPosition;
}

// PoolAllocator
PoolAllocator::PoolAllocator(uint32_t objectSize, uint8_t objectAlignment, uint32_t size, void* pMem) 
	: Allocator(pMem), m_objectSize(objectSize), m_objectAlignment(objectAlignment), m_size(size)
{
	assert(objectSize >= sizeof(void*));

	uint8_t adjustment = alignAdjustment(pMem, objectAlignment);

	m_freeList = (void**)((uintptr_t)pMem + adjustment);

	uint32_t numObjects = (size-adjustment)/objectSize;

	void** p = m_freeList;

	for(uint32_t i = 0; i < numObjects-1; i++)
	{
		*p = (void*)( (uintptr_t) p + objectSize );
		p = (void**) *p;
	}

	*p = nullptr;
}

PoolAllocator::~PoolAllocator()
{
	m_freeList = nullptr;
}

void* PoolAllocator::allocate(uint32_t size, uint8_t alignment)
{
	assert(size == m_objectSize && alignment == m_objectAlignment);

	if(m_freeList == nullptr)
		return nullptr;

	void* p = m_freeList;

	m_freeList = (void**)(*m_freeList);

	Allocator::allocate(size);

	return p;
}

void PoolAllocator::deallocate(void* p)
{
	*((void**)p) = m_freeList;

	m_freeList = (void**)p;

	Allocator::deallocate(m_objectSize);
}


// StackAllocator
StackAllocator::StackAllocator(uint32_t size, void* pStart) 
	: Allocator(pStart), m_size(size), m_initialPosition(pStart), m_currentPosition(pStart)
{
	assert(size > 0);

#if _DEBUG
	m_prevPosition    = nullptr;
#endif
}

StackAllocator::~StackAllocator()
{
	m_initialPosition   = nullptr;
#if _DEBUG
	m_prevPosition      = nullptr;
#endif
	m_currentPosition   = nullptr;
	m_size               = 0;
}

void* StackAllocator::allocate(uint32_t size, uint8_t alignment)
{
	assert(size != 0);

	uint8_t adjustment = alignAdjustmentWithHeader(m_currentPosition, alignment, sizeof(AllocationHeader));

	if(getUsedMemory() + adjustment + size > m_size)
		return nullptr;

	uintptr_t alignedAddress = (uintptr_t)m_currentPosition + adjustment;

	AllocationHeader* pHeader = (AllocationHeader*)(alignedAddress-sizeof(AllocationHeader));

	pHeader->adjustment   = adjustment;

#if _DEBUG
	pHeader->prevAddress = m_prevPosition;
	m_prevPosition    = (void*)alignedAddress;
#endif

	m_currentPosition = (void*)(alignedAddress + size);

	Allocator::allocate(size + adjustment);

	return (void*)alignedAddress;
}

void StackAllocator::deallocate(void* p)
{
#if _DEBUG
	assert( p == m_prevPosition );
#endif

	AllocationHeader* pHeader = (AllocationHeader*)((uintptr_t)p - sizeof(AllocationHeader));

	Allocator::deallocate( (uintptr_t)m_currentPosition - (uintptr_t)p + pHeader->adjustment );
	
	m_currentPosition = (void*)( (uintptr_t)p - pHeader->adjustment );

	#if _DEBUG
	m_prevPosition = pHeader->prevAddress;
	#endif

}


}

