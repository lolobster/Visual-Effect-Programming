//--------------------------------------------------------------------------------------
// File: FrmMath.h
// Desc: 
//
// Author:     QUALCOMM, Advanced Content Group - Snapdragon SDK
//
//               Copyright (c) 2013 QUALCOMM Technologies, Inc. 
//                         All Rights Reserved. 
//                      QUALCOMM Proprietary/GTDR
//--------------------------------------------------------------------------------------
#ifndef _FRM_MATH_H_
#define _FRM_MATH_H_


// Use row-major matrices.
#define FRM_ROW_MAJOR

#if defined(OS_ANDROID)
    #define LOC_INCLINE inline
#else
    #define LOC_INCLINE __inline
#endif // defined(OS_ANDROID)

// Min and Max from WinDef.h
#ifndef NOMINMAX

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#endif  /* NOMINMAX */

//--------------------------------------------------------------------------------------
// Degree/radian conversions
//--------------------------------------------------------------------------------------
const FLOAT32 FRM_PI = 3.1415926f;

#define FrmRadians( a )   ( (a) * (FRM_PI/180.0f) )
#define FrmDegrees( a )   ( (a) * (180.0f/FRM_PI) )


//--------------------------------------------------------------------------------------
// Name: srtuct FRMCOLOR
// Desc: A 4-component color, packed into a 32-bit value
//--------------------------------------------------------------------------------------
typedef struct _FRMCOLOR
{
#ifdef __cplusplus
    _FRMCOLOR() {}
    _FRMCOLOR( UINT32 val ) { v = val; }
    _FRMCOLOR( FLOAT32 _r, FLOAT32 _g, FLOAT32 _b, FLOAT32 _a=1.0f ) { r = (UINT8)(255*_r); g = (UINT8)(255*_g); b = (UINT8)(255*_b); a = (UINT8)(255*_a); }
    _FRMCOLOR& operator = ( const _FRMCOLOR& val ) { v = val.v; return *this; }
    _FRMCOLOR& operator = ( const UINT32& val ) { v = val; return *this; }
#endif // __cplusplus

    union
    {
        struct{ UINT8 r, g, b, a; };
        UINT32 v; // Swizzled as ABGR
    };
} FRMCOLOR;

inline FRMCOLOR FRMMAKECOLOR( BYTE r, BYTE g, BYTE b, BYTE a = 255 )
{
    FRMCOLOR color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    return color;
}


//--------------------------------------------------------------------------------------
// Name: struct FRMVECTOR2
// Desc: A 2-component vector
//--------------------------------------------------------------------------------------
typedef struct _FRMVECTOR2
{
#ifdef __cplusplus
    _FRMVECTOR2() {}
    _FRMVECTOR2( FLOAT32 _x, FLOAT32 _y ) { x = _x; y = _y; }
    _FRMVECTOR2& operator  = ( const _FRMVECTOR2& val ) { x = val.x; y = val.y; return *this; }
    _FRMVECTOR2& operator += ( const _FRMVECTOR2& val ) { x += val.x; y += val.y; return *this; }
    _FRMVECTOR2& operator -= ( const _FRMVECTOR2& val ) { x -= val.x; y -= val.y; return *this; }
    _FRMVECTOR2& operator *= ( FLOAT32 val ) { x *= val; y *= val; return *this; }
    _FRMVECTOR2& operator /= ( FLOAT32 val ) { x /= val; y /= val; return *this; }
    _FRMVECTOR2  operator + () const { return _FRMVECTOR2(x,y); }
    _FRMVECTOR2  operator - () const { return _FRMVECTOR2(-x,-y); }
    _FRMVECTOR2  operator + ( const _FRMVECTOR2& val ) const { return _FRMVECTOR2( x+val.x, y+val.y ); }
    _FRMVECTOR2  operator - ( const _FRMVECTOR2& val ) const { return _FRMVECTOR2( x-val.x, y-val.y ); }
    _FRMVECTOR2  operator * ( FLOAT32 val ) const { return _FRMVECTOR2( x*val, y*val ); }
    _FRMVECTOR2  operator / ( FLOAT32 val ) const { return _FRMVECTOR2( x/val, y/val ); }
    friend _FRMVECTOR2 operator * ( FLOAT32 s, const _FRMVECTOR2& v ){ return _FRMVECTOR2( s*v.x, s*v.y ); }
#endif // __cplusplus

    union
    {
        struct{ FLOAT32 x, y; };
        struct{ FLOAT32 tu, tv; };
        FLOAT32 v[2];
    };
} FRMVECTOR2;


//--------------------------------------------------------------------------------------
// Name: struct FRMVECTOR3
// Desc: A 3-component vector
//--------------------------------------------------------------------------------------
typedef struct _FRMVECTOR3
{
#ifdef __cplusplus
    _FRMVECTOR3() {}
    _FRMVECTOR3( FLOAT32 _x, FLOAT32 _y, FLOAT32 _z ) { x = _x; y = _y; z = _z; }
    _FRMVECTOR3& operator  = ( const _FRMVECTOR3& val ) { x = val.x; y = val.y; z = val.z; return *this; }
    _FRMVECTOR3& operator += ( const _FRMVECTOR3& val ) { x += val.x; y += val.y; z += val.z; return *this; }
    _FRMVECTOR3& operator -= ( const _FRMVECTOR3& val ) { x -= val.x; y -= val.y; z -= val.z; return *this; }
    _FRMVECTOR3& operator *= ( FLOAT32 val ) { x *= val; y *= val; z *= val; return *this; }
    _FRMVECTOR3& operator /= ( FLOAT32 val ) { x /= val; y /= val; z /= val; return *this; }
    _FRMVECTOR3  operator + () const { return _FRMVECTOR3(x,y,z); }
    _FRMVECTOR3  operator - () const { return _FRMVECTOR3(-x,-y,-z); }
    _FRMVECTOR3  operator + ( const _FRMVECTOR3& val ) const { return _FRMVECTOR3( x+val.x, y+val.y, z+val.z ); }
    _FRMVECTOR3  operator - ( const _FRMVECTOR3& val ) const { return _FRMVECTOR3( x-val.x, y-val.y, z-val.z ); }
    _FRMVECTOR3  operator * ( FLOAT32 val ) const { return _FRMVECTOR3( x*val, y*val, z*val ); }
    _FRMVECTOR3  operator / ( FLOAT32 val ) const { return _FRMVECTOR3( x/val, y/val, z/val ); }
    friend _FRMVECTOR3 operator * ( FLOAT32 s, const _FRMVECTOR3& v ){ return _FRMVECTOR3( s*v.x, s*v.y, s*v.z ); }
#endif // __cplusplus

    union
    {
        struct{ FLOAT32 x, y, z; };
        struct{ FLOAT32 r, g, b; };
        FLOAT32 v[3];
    };
} FRMVECTOR3;


//--------------------------------------------------------------------------------------
// Name: struct FRMVECTOR4
// Desc: A 4-component vector
//--------------------------------------------------------------------------------------
typedef struct _FRMVECTOR4
{
#ifdef __cplusplus
    _FRMVECTOR4() {}
    _FRMVECTOR4( FLOAT32 _x, FLOAT32 _y, FLOAT32 _z, FLOAT32 _w ) { x = _x; y = _y; z = _z; w = _w; }
    _FRMVECTOR4& operator  = ( const _FRMVECTOR4& val ) { x = val.x; y = val.y; z = val.z; w = val.w; return *this; }
    _FRMVECTOR4& operator += ( const _FRMVECTOR4& val ) { x += val.x; y += val.y; z += val.z; w += val.w; return *this; }
    _FRMVECTOR4& operator -= ( const _FRMVECTOR4& val ) { x -= val.x; y -= val.y; z -= val.z; w -= val.w; return *this; }
    _FRMVECTOR4& operator *= ( FLOAT32 val ) { x *= val; y *= val; z *= val; w *= val; return *this; }
    _FRMVECTOR4& operator /= ( FLOAT32 val ) { x /= val; y /= val; z /= val; w /= val; return *this; }
    _FRMVECTOR4  operator + () const { return _FRMVECTOR4(x,y,z,w); }
    _FRMVECTOR4  operator - () const { return _FRMVECTOR4(-x,-y,-z,-w); }
    _FRMVECTOR4  operator + ( const _FRMVECTOR4& val ) const { return _FRMVECTOR4( x+val.x, y+val.y, z+val.z, w+val.w ); }
    _FRMVECTOR4  operator - ( const _FRMVECTOR4& val ) const { return _FRMVECTOR4( x-val.x, y-val.y, z-val.z, w-val.w ); }
    _FRMVECTOR4  operator * ( FLOAT32 val ) const { return _FRMVECTOR4( x*val, y*val, z*val, w*val ); }
    _FRMVECTOR4  operator / ( FLOAT32 val ) const { return _FRMVECTOR4( x/val, y/val, z/val, w/val ); }
    friend _FRMVECTOR4 operator * ( FLOAT32 s, const _FRMVECTOR4& v ){ return _FRMVECTOR4( s*v.x, s*v.y, s*v.z, s*v.w ); }
#endif // __cplusplus

    union
    {
        struct{ FLOAT32 x, y, z, w; };
        struct{ FLOAT32 r, g, b, a; };
        FLOAT32 v[4];
    };
} FRMVECTOR4;


//--------------------------------------------------------------------------------------
// Name: struct FRMMATRIX3X3
// Desc: A 3x3 matrix
//--------------------------------------------------------------------------------------
typedef struct _FRMMATRIX3X3
{
#ifdef FRM_ROW_MAJOR
    FLOAT32& M(UINT32 r, UINT32 c) { return m[r][c]; }
#else
    FLOAT32& M(UINT32 r, UINT32 c) { return m[c][r]; }
#endif

    FLOAT32 m[3][3];
} FRMMATRIX3X3;


//--------------------------------------------------------------------------------------
// Name: struct FRMMATRIX4X3
// Desc: A 4x3 matrix
//--------------------------------------------------------------------------------------
typedef struct _FRMMATRIX4X3
{
#ifdef FRM_ROW_MAJOR
    FLOAT32& M(UINT32 r, UINT32 c) { return m[r][c]; }
#else
    FLOAT32& M(UINT32 r, UINT32 c) { return m[c][r]; }
#endif

#ifdef FRM_ROW_MAJOR
    FLOAT32 m[4][3];
#else
    FLOAT32 m[3][4];
#endif
} FRMMATRIX4X3;


//--------------------------------------------------------------------------------------
// Name: struct FRMMATRIX4X4
// Desc: A 4x4 matrix
//--------------------------------------------------------------------------------------
typedef struct _FRMMATRIX4X4
{
#ifdef FRM_ROW_MAJOR
    FLOAT32& M(UINT32 r, UINT32 c) { return m[r][c]; }
#else
    FLOAT32& M(UINT32 r, UINT32 c) { return m[c][r]; }
#endif

    FLOAT32 m[4][4];
} FRMMATRIX4X4;


//--------------------------------------------------------------------------------------
// Scalar math functions
//--------------------------------------------------------------------------------------
#define FrmMin( x, y )    ( ( x < y ) ? x : y )
#define FrmMax( x, y )    ( ( x > y ) ? x : y )
#define FrmClampZero( x ) ( ( x > 0 ) ? x : 0 )
LOC_INCLINE FLOAT32    FrmFloor( FLOAT32 value );
LOC_INCLINE VOID       FrmSrand( UINT32 seed );
LOC_INCLINE FLOAT32    FrmRand();
LOC_INCLINE FLOAT32    FrmPow( FLOAT32 x, FLOAT32 y );
LOC_INCLINE FLOAT32    FrmSin( FLOAT32 deg );
LOC_INCLINE FLOAT32    FrmCos( FLOAT32 deg );
LOC_INCLINE FLOAT32    FrmAcos( FLOAT32 deg );
LOC_INCLINE FLOAT32    FrmTan( FLOAT32 deg );
LOC_INCLINE FLOAT32    FrmSqrt( FLOAT32 value );


//--------------------------------------------------------------------------------------
// Common helper functions
//--------------------------------------------------------------------------------------
LOC_INCLINE FLOAT32    FrmLerp( FLOAT32 t, FLOAT32 a, FLOAT32 b );
LOC_INCLINE FRMCOLOR   FrmLerp( FLOAT32 t, const FRMCOLOR& a, const FRMCOLOR& b );
LOC_INCLINE FRMVECTOR3 FrmSphrand( FLOAT32 fRadius );
LOC_INCLINE FLOAT32    ApplyVariance( FLOAT32 fValue, FLOAT32 fVariance );
LOC_INCLINE FLOAT32    FrmSCurve(const float t);
LOC_INCLINE FLOAT32    FrmVector2Dot( FRMVECTOR2& vVector0, FRMVECTOR2& vVector1 );
LOC_INCLINE FLOAT32    FrmVector2Length( FRMVECTOR2& vVector );
LOC_INCLINE FRMVECTOR2 FrmVector2Normalize( FRMVECTOR2& vVector );


//--------------------------------------------------------------------------------------
// Operations for 3-component vectors
//--------------------------------------------------------------------------------------
LOC_INCLINE FRMVECTOR3 FrmVector3Lerp( FRMVECTOR3& v0, FRMVECTOR3& v1, FLOAT32 s );
LOC_INCLINE FRMVECTOR3 FrmVector3Mul( FRMVECTOR3& vVector, FLOAT32 fScalar );
LOC_INCLINE FRMVECTOR3 FrmVector3Add( FRMVECTOR3& vVector0, FRMVECTOR3& vVector1 );
LOC_INCLINE FRMVECTOR3 FrmVector3Cross( FRMVECTOR3& vVector0, FRMVECTOR3& vVector1 );
LOC_INCLINE FLOAT32    FrmVector3Dot( FRMVECTOR3& vVector0, FRMVECTOR3& vVector1 );
LOC_INCLINE FLOAT32    FrmVector3Length( FRMVECTOR3& vVector );
LOC_INCLINE FRMVECTOR3 FrmVector3Normalize( FRMVECTOR3& vVector );
LOC_INCLINE FRMVECTOR4 FrmVector3Transform( FRMVECTOR3& vSrcVector, FRMMATRIX4X4& matSrcMatrix );
LOC_INCLINE FRMVECTOR3 FrmVector3TransformCoord( FRMVECTOR3& vSrcVector, FRMMATRIX4X4& matSrcMatrix );
LOC_INCLINE FRMVECTOR3 FrmVector3TransformNormal( FRMVECTOR3& vSrcVector, FRMMATRIX4X4& matSrcMatrix );


//--------------------------------------------------------------------------------------
// Operations for 4-component vectors
//--------------------------------------------------------------------------------------
LOC_INCLINE FLOAT32    FrmVector4Dot( FRMVECTOR4& vVector0, FRMVECTOR4& vVector1 );
LOC_INCLINE FLOAT32    FrmVector4Length( FRMVECTOR4& vVector );
LOC_INCLINE FRMVECTOR4 FrmVector4Normalize( FRMVECTOR4& vVector );
LOC_INCLINE FRMVECTOR4 FrmVector4Transform( FRMVECTOR4& vSrcVector, FRMMATRIX4X4& matSrcMatrix );
LOC_INCLINE FRMVECTOR4 FrmVector4SLerp( FRMVECTOR4& q0, FRMVECTOR4& q1, FLOAT32 s );
LOC_INCLINE FRMVECTOR4 FrmQuaternionMultiply( FRMVECTOR4& q0, FRMVECTOR4& q1 );


//--------------------------------------------------------------------------------------
// Operations for 3x3 matrices
//--------------------------------------------------------------------------------------
LOC_INCLINE FRMMATRIX3X3 FrmMatrixTranspose( FRMMATRIX3X3& matSrcMatrix );


//--------------------------------------------------------------------------------------
// Operations for 4x4 matrices
//--------------------------------------------------------------------------------------
LOC_INCLINE FRMMATRIX4X4 FrmMatrixIdentity();
LOC_INCLINE FRMMATRIX3X3 FrmMatrixNormalOrthonormal( FRMMATRIX4X4& matSrcMatrix );
LOC_INCLINE FRMMATRIX3X3 FrmMatrixNormal( FRMMATRIX4X4& matSrcMatrix );
LOC_INCLINE FRMMATRIX4X4 FrmMatrixMultiply( FRMMATRIX4X4& matSrcMatrixA, FRMMATRIX4X4& matSrcMatrixB );
LOC_INCLINE FRMMATRIX4X4 FrmMatrixInverse( FRMMATRIX4X4& matSrcMatrix );
LOC_INCLINE FRMMATRIX4X4 FrmMatrixInverseOrthonormal( FRMMATRIX4X4& matSrcMatrix );
LOC_INCLINE FRMMATRIX4X4 FrmMatrixTranspose( FRMMATRIX4X4& matSrcMatrix );
LOC_INCLINE FRMMATRIX4X4 FrmMatrixTranslate( FRMVECTOR3& vVector );
LOC_INCLINE FRMMATRIX4X4 FrmMatrixTranslate( FLOAT32 tx, FLOAT32 ty, FLOAT32 tz );
LOC_INCLINE FRMMATRIX4X4 FrmMatrixScale( FRMVECTOR3& vVector );
LOC_INCLINE FRMMATRIX4X4 FrmMatrixScale( FLOAT32 sx, FLOAT32 sy, FLOAT32 sz );
LOC_INCLINE FRMMATRIX4X4 FrmMatrixRotate( FLOAT32 fAngle, FRMVECTOR3& vAxis );
LOC_INCLINE FRMMATRIX4X4 FrmMatrixRotate( FLOAT32 fAngle, FLOAT32 rx, FLOAT32 ry, FLOAT32 rz );
LOC_INCLINE FRMMATRIX4X4 FrmMatrixRotate( FRMVECTOR4& vQuaternion );
LOC_INCLINE FRMMATRIX4X4 FrmMatrixReflect( FRMMATRIX4X4& matSrcMatrix, FRMVECTOR3& n, FLOAT32 d );
LOC_INCLINE FRMMATRIX4X4 FrmMatrixLookAtLH( FRMVECTOR3& vPosition, FRMVECTOR3& vLookAt, FRMVECTOR3& vUp );
LOC_INCLINE FRMMATRIX4X4 FrmMatrixLookAtRH( FRMVECTOR3& vPosition, FRMVECTOR3& vLookAt, FRMVECTOR3& vUp );
LOC_INCLINE FRMMATRIX4X4 FrmMatrixPerspectiveFovLH( FLOAT32 fFOVy, FLOAT32 fAspect, FLOAT32 zNear, FLOAT32 zFar );
LOC_INCLINE FRMMATRIX4X4 FrmMatrixPerspectiveFovRH( FLOAT32 fFOVy, FLOAT32 fAspect, FLOAT32 zNear, FLOAT32 zFar );


//--------------------------------------------------------------------------------------
// Inline platform-specific implementation of the above functions
//--------------------------------------------------------------------------------------
#if defined(ANDROID)
#include <graphics/win32/FrmMath_Platform.inl>
#endif // !defined(OS_ANDROID)

#if defined(_WIN32)
#include <graphics/win32/FrmMath_Platform.inl>
#endif


#endif // _FRM_MATH_H_
