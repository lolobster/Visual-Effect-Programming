#include <slmath/vec4.h>

SLMATH_BEGIN()

vec4 normalize( const vec4& v )
{
	assert( check(v) );

	float len = length(v);
	assert( len >= FLT_MIN );
	
	float invlen = 1.f / len;
	vec4 res( v.x*invlen, v.y*invlen, v.z*invlen, v.w*invlen );
	assert( check(res) );
	return res;
}

SLMATH_END()

// This file is part of 'slmath' C++ library. Copyright (C) 2009 Jani Kajala (kajala@gmail.com). See http://sourceforge.net/projects/slmath/
