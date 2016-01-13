#ifndef _REF_H_
#define _REF_H_
#include <es_assert.h>
namespace core
{

template < class Type >
class Ref
{
public:
	inline Ref( const Ref& obj );
	inline Ref( );
	inline Ref( Type* p );
	inline Ref( int p );

	inline ~Ref<Type>();

	
	inline Ref<Type>& operator=( const Ref<Type>& o );
	inline Ref<Type>& operator=( Type* o );
	inline Ref<Type>& operator=( int o );
	
	inline Type* operator->();

	
#if !defined(SWIG)
	inline operator Type*();
#endif	
	
	inline Type* ptr() const;
	inline const Type* operator ->() const;
	inline bool operator ==( const Type* const pObj ) const;
	inline bool operator !=( const Type* const pObj ) const;
	inline bool operator ==( const Ref<Type>& obj ) const;
	inline bool operator !=( const Ref<Type>& obj ) const;
	inline bool operator !( ) const;
	inline Type& operator* () const;

private:
	// Member variables
	Type*		m_ptr;

};

template < class Type >
inline bool operator ==( Type* lhs, Ref<Type> rhs );

template < class Type >
inline bool operator !=( Type* lhs, Ref<Type> rhs );

#include "Ref.inl"

}

#endif


