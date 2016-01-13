//--------------------------------------------------------------------------------------
// File: StdLib_Win32.cpp
// Desc: Wrapper around various C standard library functions for portability.
//
// Author:     QUALCOMM, Advanced Content Group - Snapdragon SDK
//
//               Copyright (c) 2013 QUALCOMM Technologies, Inc. 
//                         All Rights Reserved. 
//                      QUALCOMM Proprietary/GTDR
//--------------------------------------------------------------------------------------
//#include "FrmPlatform.h"
//#include "FrmStdLib.h"
#include <EABase\eabase.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

// Memory allocation functions
void*       FrmMalloc( uint32_t nSize )                          { return malloc( (size_t)nSize ); }
void*       FrmRealloc( void* iptr, uint32_t nSize )             { return realloc( iptr, (size_t)nSize ); }
void        FrmFree( void* ptr )                               { free( ptr ); }

// Memory manipulation functions
void*       FrmMemcpy( void* s, const void* ct, uint32_t n )     { return memcpy( s, ct, n ); }
void*       FrmMemmove( void* s, const void* ct, uint32_t n )    { return memmove( s, ct, n ); }
int32_t       FrmMemcmp( const void* cs, const void* ct, uint32_t n )   { return memcmp( cs, ct, n ); }
const void* FrmMemchr( const void* cs, uint8_t c, uint32_t n )      { return memchr( cs, c, n ); }
const void* FrmMemset( void* s, uint8_t c, uint32_t n )             { return memset( s, c, n ); }

// String manipulation functions
char*       FrmStrcpy( char* s, const char* ct )              { return strcpy( s, ct ); }
char*       FrmStrncpy( char* s, const char* ct, uint32_t n )   { return strncpy( s, ct, n ); }
char*       FrmStrcat( char* s, const char* ct )              { return strcat( s, ct ); }
char*       FrmStrncat( char* s, const char* ct, uint32_t n )   { return strncat( s, ct, n ); }
int       FrmStrcmp( const char* cs, const char* ct )       { return strcmp( cs, ct ); }
int       FrmStricmp( const char* cs, const char* ct )      { return _stricmp( cs, ct ); }
int       FrmStrncmp( const char* cs, const char* ct, uint32_t n )   { return strncmp( cs, ct, n ); }
const char* FrmStrchr( const char* cs, char c )               { return strchr( cs, c ); }
const char* FrmStrrchr( const char* cs, char c )              { return strrchr( cs, c ); }
uint32_t      FrmStrspn( const char* cs, const char* ct )       { return strspn( cs, ct ); }
uint32_t      FrmStrcspn( const char* cs, const char* ct )      { return strcspn( cs, ct ); }
const char* FrmStrpbrk( const char* cs, const char* ct )      { return strpbrk( cs, ct ); }
const char* FrmStrstr( const char* cs, const char* ct )       { return strstr( cs, ct ); }
uint32_t      FrmStrlen( const char* cs )                       { return strlen( cs ); }
char*       FrmStrerror( int n )                            { return strerror( n ); }
char*       FrmStrtok( char* s, const char* ct )              { return strtok( s, ct ); }

int FrmSprintf( char* s, int n, const char* format, ... )
{ 
    va_list args;
    va_start( args, format );
    int ret = vsprintf_s( s, n, format, args );
    va_end( args );
    return ret;
}
