#ifndef _OBJECT_H_
#define _OBJECT_H_
#include <new>
#include <es_assert.h>

namespace core
{
	class Allocator;

class Object
{
public:
	Object();

    virtual ~Object();

	inline void addRef()
	{
		assert( m_numOfRefs >= 0 );
		++m_numOfRefs;
	}

    inline int releaseRef()
	{
 		assert( m_numOfRefs > 0 );
		return --m_numOfRefs;
	}

	inline int getRefCount() const
	{
		return m_numOfRefs;
	}

	void* operator new (std::size_t size);

	void* operator new (std::size_t size, void* ptr);

	void operator delete (void* ptr);

	void operator delete (void* ptr, void* voidptr2);

	void* operator new[] (std::size_t size);

	void* operator new[] (std::size_t size, void* ptr);

	void operator delete[] (void* ptr);

	void operator delete[] (void* ptr, void* voidptr2);

private:
	// Member variables
	int m_numOfRefs;

	// Non-allowed methods (declared but not defined anywhere, result link error if used)
	Object( const Object& );
	Object& operator=( const Object& );
};


}


void* engineAllocMem(core::Allocator* alloc, size_t size, size_t alignment = 16);
void engineFreeMem(core::Allocator* alloc, void* p);

void* engineAllocMem(size_t size);
void engineFreeMem(void* p);

/**
 * About the realloc function:
 * void * engineReallocMem(void *ud, void *ptr, size_t osize, size_t nsize);
 * (`osize' is the old size, `nsize' is the new size)
 *
 * * engineReallocMem(ud, NULL, x, s) creates a new block of size `s' (no
 * matter 'x').
 *
 * * engineReallocMem(ud, p, x, 0) frees the block `p'
 * (in this specific case, frealloc must return NULL);
 * particularly, frealloc(ud, NULL, 0, 0) does nothing
 * (which is equivalent to free(NULL) in ANSI C)
 *
 * engineReallocMem returns NULL if it cannot create or reallocate the area
 * (any reallocation to an equal or smaller size cannot fail!)
 */
void * engineReallocMem(void *ud, void *ptr, size_t x, size_t s);

void* engineAllocMemString(size_t size);
void engineFreeMemString(void* p);

#endif // OBJECT_H_


