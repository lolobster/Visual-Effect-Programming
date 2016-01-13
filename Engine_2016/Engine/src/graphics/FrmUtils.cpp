//--------------------------------------------------------------------------------------
// File: FrmUtils.cpp
// Desc: 
//
// Author:     QUALCOMM, Advanced Content Group - Snapdragon SDK
//
//               Copyright (c) 2013 QUALCOMM Technologies, Inc. 
//                         All Rights Reserved. 
//                      QUALCOMM Proprietary/GTDR
//--------------------------------------------------------------------------------------
#include "FrmPlatform.h"
#include <graphics/FrmUtils.h>
#include <core/FileStream.h>

#include <core/Ref.h>
//--------------------------------------------------------------------------------------
// Name: FrmLoadFile()
// Desc: 
//--------------------------------------------------------------------------------------
BOOL FrmLoadFile( const CHAR* strFileName, VOID** ppData, UINT32* pnSize )
{
	core::Ref<core::FileStream> fs = new core::FileStream(strFileName, core::FileStream::READ_ONLY);

	int size = fs->available();

	if( 0 == size )
    {
        return FALSE;
    }

    if( ppData )
    {
        BYTE* pData = new BYTE[size+1];
		fs->read(pData, size );
        pData[size] = 0;

        (*ppData) = pData;
    }

    if( pnSize )
        (*pnSize) = size;

    return TRUE;
}


//--------------------------------------------------------------------------------------
// Name: FrmUnloadFile()
// Desc: 
//--------------------------------------------------------------------------------------
VOID FrmUnloadFile( VOID* pData )
{
    delete[] (BYTE* )pData;
}

/*
//--------------------------------------------------------------------------------------
// Name: FrmSaveImageAsTGA()
// Desc: Save a 32-bit RGBA image as a TGA file.
//--------------------------------------------------------------------------------------
BOOL FrmSaveImageAsTGA( const CHAR* strFileName, INT16 nWidth, INT16 nHeight, 
                        UINT32* pBits32 )
{
    typedef struct _TARGA_HEADER
    {
        BYTE   IDLength;
        BYTE   ColormapType;
        BYTE   ImageType;
        BYTE   ColormapSpecification[5];
        UINT16 XOrigin;
        UINT16 YOrigin;
        UINT16 ImageWidth;
        UINT16 ImageHeight;
        BYTE   PixelDepth;
        BYTE   ImageDescriptor;
    } TARGA_HEADER;
    
    // Create the TGA file
    FRM_FILE* pFile;
    if( FALSE == FrmFile_Open( strFileName, FRM_FILE_WRITE, &pFile ) )
        return FALSE;

    // Write out the TGA header
    TARGA_HEADER header;
    FrmMemset( &header, 0, sizeof(header) );
    header.ImageType   = 2;
    header.ImageWidth  = nWidth;
    header.ImageHeight = nHeight;
    header.PixelDepth  = 32;
    FrmFile_Write( pFile, &header, sizeof(header) );

    // Write out the pixels
    FrmFile_Write( pFile, pBits32, sizeof(UINT32)*nWidth*nHeight );

    // Close the file and return
    FrmFile_Close( pFile );
    return TRUE;
}


BOOL FrmSaveImageAsTGA24( const CHAR* strFileName, INT16 nWidth, INT16 nHeight, 
                       BYTE* pBits24 )
{
    typedef struct _TARGA_HEADER
    {
        BYTE   IDLength;
        BYTE   ColormapType;
        BYTE   ImageType;
        BYTE   ColormapSpecification[5];
        UINT16 XOrigin;
        UINT16 YOrigin;
        UINT16 ImageWidth;
        UINT16 ImageHeight;
        BYTE   PixelDepth;
        BYTE   ImageDescriptor;
    } TARGA_HEADER;

    // Create the TGA file
    FRM_FILE* pFile;
    if( FALSE == FrmFile_Open( strFileName, FRM_FILE_WRITE, &pFile ) )
        return FALSE;

    // Write out the TGA header
    TARGA_HEADER header;
    FrmMemset( &header, 0, sizeof(header) );
    header.ImageType   = 2;
    header.ImageWidth  = nWidth;
    header.ImageHeight = nHeight;
    header.PixelDepth  = 24;
    FrmFile_Write( pFile, &header, sizeof(header) );

    // Write out the pixels
    FrmFile_Write( pFile, pBits24, sizeof(BYTE)*3*nWidth*nHeight );

    // Close the file and return
    FrmFile_Close( pFile );
    return TRUE;
}*/

//////////////////////////////////////////////////////////////////////////

BOOL FrmSaveScreenShot( const CHAR* strFileName )
{
    return FALSE;
}