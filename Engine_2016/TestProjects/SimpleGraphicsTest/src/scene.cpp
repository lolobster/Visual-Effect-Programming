//=============================================================================
// FILE: scene.cpp
//
// Create and render a scene
//
// Author:        QUALCOMM, Advanced Content Group - Snapdragon SDK
//
//                  Copyright (c) 2013 QUALCOMM Technologies, Inc.
//                              All Rights Reserved.
//                          QUALCOMM Proprietary/GTDR
//=============================================================================

//=============================================================================
// INCLUDES AND VARIABLE DEFINITIONS
//=============================================================================

#include <scene.h>

#include <es_assert.h>


#include <core/log.h>


using namespace Engine;


//=============================================================================
// CONSTS
//=============================================================================

//=============================================================================
// IMPLEMENTATION
//=============================================================================
namespace Engine
{
//--------------------------------------------------------------------------------------
// Name: OffscreenRT()
// Desc: Class constructor.  Clears render target data
//--------------------------------------------------------------------------------------
OffscreenRT::OffscreenRT()
{
    BufferHandle = 0;
    TextureHandle = 0;
    DepthTextureHandle = 0;
    Width = 0;
    Height = 0;
}


//--------------------------------------------------------------------------------------
// Name: ~OffscreenRT()
// Desc: Class destructor.
//--------------------------------------------------------------------------------------
OffscreenRT::~OffscreenRT()
{
    FreeRT();
}


//--------------------------------------------------------------------------------------
// Name: CreateRT
// Desc: Creates a render target for drawing to.
//--------------------------------------------------------------------------------------
BOOL OffscreenRT::CreateRT( UINT32 DesiredWidth, UINT32 DesiredHeight )
{
    // Free the old RT, in case it was previously created
    FreeRT();

    Width = DesiredWidth;
    Height = DesiredHeight;

    glGenRenderbuffers( 1, &DepthTextureHandle );
    glBindRenderbuffer( GL_RENDERBUFFER, DepthTextureHandle );
    glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, Width, Height );

    glGenFramebuffers( 1, &BufferHandle );
    glBindFramebuffer( GL_FRAMEBUFFER, BufferHandle );

    glGenTextures( 1, &TextureHandle );
    glBindTexture( GL_TEXTURE_2D, TextureHandle );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, TextureHandle );
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureHandle, 0 );

    // Make sure everything went ok
    switch( glCheckFramebufferStatus(GL_FRAMEBUFFER) )
    {
    case GL_FRAMEBUFFER_UNSUPPORTED:
        return FALSE;
    }

    // Reset default frame buffer
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );

    return TRUE;
}


//--------------------------------------------------------------------------------------
// Name: FreeOffscreenRT
// Desc: 
//--------------------------------------------------------------------------------------
VOID OffscreenRT::FreeRT()
{
    if( BufferHandle )
    {
        glBindFramebuffer( GL_FRAMEBUFFER, BufferHandle );
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0 );
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    }

    if( BufferHandle )
        glDeleteFramebuffers( 1, &BufferHandle );
    if( TextureHandle )
        glDeleteTextures( 1, &TextureHandle );
    if( DepthTextureHandle )
        glDeleteRenderbuffers( 1, &DepthTextureHandle );
}


//--------------------------------------------------------------------------------------
// Name: SetFramebuffer
// Desc: Sets the render target as the active frame buffer.
//--------------------------------------------------------------------------------------
VOID OffscreenRT::SetFramebuffer()
{
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, TextureHandle );
    glBindFramebuffer( GL_FRAMEBUFFER, BufferHandle );

    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureHandle, 0 );
    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthTextureHandle );

    glViewport( 0, 0, Width, Height );
}





}


//-----------------------------------------------------------------------------
Scene::Scene()
//-----------------------------------------------------------------------------
{
 //   m_strName = strName;

    // Set default values
//    m_nWidth = 400;
//    m_nHeight = 400;

    // Need defaults for input control
    //m_Input.m_nButtons        = 0;
    //m_Input.m_nLastButtons    = 0;
    //m_Input.m_nPressedButtons = 0;
    //m_Input.m_nPointerState   = 0;

    //m_CubemapHandle = 0;

}

//-----------------------------------------------------------------------------
Scene::~Scene()
//-----------------------------------------------------------------------------
{
//    if (m_CubemapHandle != 0)
//    {
//        glDeleteTextures(1, &m_CubemapHandle);
//        m_CubemapHandle = 0;
//    }
}

/*
//--------------------------------------------------------------------------------------
// Name: LoadTGA()
// Desc: Helper function to load a 32-bit TGA image file
//--------------------------------------------------------------------------------------
BYTE* Scene::LoadTGA( const CHAR* strFileName, UINT32* pWidth, UINT32* pHeight, UINT32* nFormat )
{
    struct TARGA_HEADER
    {
        BYTE   IDLength, ColormapType, ImageType;
        BYTE   ColormapSpecification[5];
        UINT16 XOrigin, YOrigin;
        UINT16 ImageWidth, ImageHeight;
        BYTE   PixelDepth;
        BYTE   ImageDescriptor;
    };

    static TARGA_HEADER header;

    // Read the TGA file
    FRM_FILE* pFile;
    if( FALSE == FrmFile_Open( strFileName, FRM_FILE_READ, &pFile ) )
        return 0;

    FrmFile_Read( pFile, &header, sizeof(header) );
    UINT32 nPixelSize = header.PixelDepth / 8;
    (*pWidth)  = header.ImageWidth;
    (*pHeight) = header.ImageHeight;
    (*nFormat) = nPixelSize == 3 ? GL_RGB : GL_RGBA;

    BYTE* pBits = new BYTE[ nPixelSize * header.ImageWidth * header.ImageHeight ];
    FrmFile_Read( pFile, pBits, nPixelSize * header.ImageWidth * header.ImageHeight );
    FrmFile_Close( pFile );

    // Convert the image from BGRA to RGBA
    BYTE* p = pBits;
    for( UINT32 y=0; y<header.ImageHeight; y++ )
    {
        for( UINT32 x=0; x<header.ImageWidth; x++ )
        {
            BYTE temp = p[2]; p[2] = p[0]; p[0] = temp;
            p += nPixelSize;
        }
    }

    return pBits;
}

//--------------------------------------------------------------------------------------
// Name: SetTexture()
// Desc: Prepares a texture for a draw call
//--------------------------------------------------------------------------------------
VOID Scene::SetTexture( INT32 ShaderVar, UINT32 TextureHandle, INT32 SamplerSlot )
{
    glActiveTexture( GL_TEXTURE0 + SamplerSlot );
    glBindTexture( GL_TEXTURE_2D, TextureHandle );
    glUniform1i( ShaderVar, SamplerSlot );
}

//--------------------------------------------------------------------------------------
// Name: LoadCubeMap()
// Desc: Loads cube map texture
//--------------------------------------------------------------------------------------
BOOL Scene::LoadCubeMap(const CHAR* ImageFileName)
{
    // Create a cube map texture from six loaded images
    {
        UINT32 nWidth, nHeight, nFormat;
        BYTE* pImageData[6];
        CHAR ImageFilePath[6][512] = { 0 };

        // Set the data file path so that necessary image files can be found
        sprintf( ImageFilePath[0], "Demos/AdrenoShaders/Textures/%s_RT.tga", ImageFileName );
        sprintf( ImageFilePath[1], "Demos/AdrenoShaders/Textures/%s_LF.tga", ImageFileName );
        sprintf( ImageFilePath[2], "Demos/AdrenoShaders/Textures/%s_DN.tga", ImageFileName );
        sprintf( ImageFilePath[3], "Demos/AdrenoShaders/Textures/%s_UP.tga", ImageFileName );
        sprintf( ImageFilePath[4], "Demos/AdrenoShaders/Textures/%s_FR.tga", ImageFileName );
        sprintf( ImageFilePath[5], "Demos/AdrenoShaders/Textures/%s_BK.tga", ImageFileName );

        // Load images
        for( UINT32 i = 0; i < 6; i++ )
        {
            pImageData[i] = LoadTGA( ImageFilePath[i], &nWidth, &nHeight, &nFormat );
        }
        if( 0 == pImageData[0] || 0 == pImageData[1] || 0 == pImageData[2] )
            return FALSE;
        if( 0 == pImageData[3] || 0 == pImageData[4] || 0 == pImageData[5] )
            return FALSE;

        glGenTextures( 1, &m_CubemapHandle );
        glBindTexture( GL_TEXTURE_CUBE_MAP, m_CubemapHandle );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        for( UINT32 i = 0; i < 6; i++ )
        {
            glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, nFormat, nWidth, nHeight, 0, nFormat, GL_UNSIGNED_BYTE, pImageData[i] );
        }

        delete[] pImageData[0];
        delete[] pImageData[1];
        delete[] pImageData[2];
        delete[] pImageData[3];
        delete[] pImageData[4];
        delete[] pImageData[5];
    }

    return TRUE;
}*/

//--------------------------------------------------------------------------------------
// Name: CheckOpenGL()
// Desc: Helper function
//--------------------------------------------------------------------------------------
void Scene::checkOpenGL()
{
#ifdef _DEBUG
    GLenum test = glGetError();
    if( test != GL_NO_ERROR )
    {
        switch( test )
        {
        case GL_INVALID_ENUM:
            LOG_ERROR("gl error: GL_INVALID_ENUM\n" );
            break;

        case GL_INVALID_VALUE:
            LOG_ERROR( "gl error: GL_INVALID_VALUE\n" );
            break;

        case GL_INVALID_OPERATION:
            LOG_ERROR( "gl error: GL_INVALID_OPERATION\n" );
            break;

        case GL_OUT_OF_MEMORY:
            LOG_ERROR( "gl error: GL_OUT_OF_MEMORY\n" );
            break;

        default:
            {
                LOG_ERROR( "gl error: %X\n", test );
            }
        }
    }
    assert( test == GL_NO_ERROR );


	EGLint err = eglGetError();

	if( err != EGL_SUCCESS )
    {
        switch( err )
        {
        case EGL_NOT_INITIALIZED:
            LOG_ERROR( "egl error: EGL_NOT_INITIALIZED\n" );
            break;

        case EGL_BAD_ACCESS:
            LOG_ERROR( "egl error: EGL_BAD_ACCESS\n" );
            break;

        case EGL_BAD_ALLOC:
            LOG_ERROR( "egl error: EGL_BAD_ALLOC\n" );
            break;

        case EGL_BAD_ATTRIBUTE:
            LOG_ERROR( "egl error: EGL_BAD_ATTRIBUTE\n" );
            break;
        
		case EGL_BAD_CONTEXT:
            LOG_ERROR( "egl error: EGL_BAD_CONTEXT\n" );
            break;
			        
		case EGL_BAD_CONFIG:
            LOG_ERROR( "egl error: EGL_BAD_CONFIG\n" );
            break;

		case EGL_BAD_CURRENT_SURFACE:
            LOG_ERROR( "egl error: EGL_BAD_CURRENT_SURFACE\n" );
            break;

		case EGL_BAD_DISPLAY:
            LOG_ERROR( "egl error: EGL_BAD_DISPLAY\n" );
            break;
		case EGL_BAD_SURFACE:
            LOG_ERROR( "egl error: EGL_BAD_SURFACE\n" );
            break;
		case EGL_BAD_MATCH:
            LOG_ERROR( "egl error: EGL_BAD_MATCH\n" );
            break;
		case EGL_BAD_PARAMETER:
            LOG_ERROR( "egl error: EGL_BAD_PARAMETER\n" );
            break;
		case EGL_BAD_NATIVE_PIXMAP:
            LOG_ERROR( "egl error: EGL_BAD_NATIVE_PIXMAP\n" );
            break;
		case EGL_BAD_NATIVE_WINDOW:
            LOG_ERROR( "egl error: EGL_BAD_NATIVE_WINDOW\n" );
            break;
		case EGL_CONTEXT_LOST:
            LOG_ERROR( "egl error: EGL_CONTEXT_LOST\n" );
            break;
        default:
            {
                LOG_ERROR( "egl error: %X\n", err );
            }
        }
    }
	assert( err == EGL_SUCCESS );
#endif
}

//--------------------------------------------------------------------------------------
// Name: 
// Desc: 
//--------------------------------------------------------------------------------------
/*BOOL Scene::CheckFrameBufferStatus()
{
    switch( glCheckFramebufferStatus(GL_FRAMEBUFFER) )
    {
    case GL_FRAMEBUFFER_COMPLETE:
        return TRUE;
    case GL_FRAMEBUFFER_UNSUPPORTED:
        return FALSE;
    }
    FrmAssert(0);
    return FALSE;
}*/

//--------------------------------------------------------------------------------------
// Name: RenderScreenAlignedQuad
// Desc: Draws a full-screen rectangle
//--------------------------------------------------------------------------------------
/*VOID Scene::RenderScreenAlignedQuadInv()
{
    glDisable( GL_DEPTH_TEST );

    // There is currently a bug in Qualcomm's shader compiler in that it can't
    // negate a value.  Therefore, change the shader and the order of the 
    // quad here.
    const FLOAT32 Quad[] =
    {

        -1.0, -1.0f, 0.0f, 1.0f,
        +1.0, -1.0f, 1.0f, 1.0f,
        -1.0, +1.0f, 0.0f, 0.0f,
        +1.0, +1.0f, 1.0f, 0.0f,  
    };

    glVertexAttribPointer( 0, 4, GL_FLOAT, 0, 0, Quad );
    glEnableVertexAttribArray( 0 );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    glDisableVertexAttribArray( 0 );
    glEnable( GL_DEPTH_TEST );
}

//--------------------------------------------------------------------------------------
// Name: RenderScreenAlignedQuad
// Desc: Draws a full-screen rectangle
//--------------------------------------------------------------------------------------
VOID Scene::RenderScreenAlignedQuad()
{
    glDisable( GL_DEPTH_TEST );

    // There is currently a bug in Qualcomm's shader compiler in that it can't
    // negate a value.  Therefore, change the shader and the order of the 
    // quad here.
    const FLOAT32 Quad[] =
    {
        +1.0, +1.0f, 1.0f, 1.0f,
        -1.0, +1.0f, 0.0f, 1.0f,
        +1.0, -1.0f, 1.0f, 0.0f,
        -1.0, -1.0f, 0.0f, 0.0f,
    };

    glVertexAttribPointer( 0, 4, GL_FLOAT, 0, 0, Quad );
    glEnableVertexAttribArray( 0 );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    glDisableVertexAttribArray( 0 );
    glEnable( GL_DEPTH_TEST );
}

//--------------------------------------------------------------------------------------
// Name: RenderScreenAlignedQuad
// Desc: Draws a full-screen rectangle
//--------------------------------------------------------------------------------------
VOID Scene::RenderScreenAlignedQuad( FRMVECTOR2 TopLeft, FRMVECTOR2 BottomRight )
{
    glDisable( GL_DEPTH_TEST );

    // There is currently a bug in Qualcomm's shader compiler in that it can't
    // negate a value.  Therefore, change the shader and the order of the 
    // quad here.
    const FLOAT32 Quad[] =
    {


        TopLeft.x, BottomRight.y,		0.0f, 1.0f,
        BottomRight.x, BottomRight.y,	1.0f, 1.0f,
        TopLeft.x, TopLeft.y,			0.0f, 0.0f,
        BottomRight.x, TopLeft.y,		1.0f, 0.0f,
        
    };

    glVertexAttribPointer( 0, 4, GL_FLOAT, 0, 0, Quad );
    glEnableVertexAttribArray( 0 );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    glDisableVertexAttribArray( 0 );
    glEnable( GL_DEPTH_TEST );
}

//--------------------------------------------------------------------------------------
// Name: RenderScreenAlignedQuad
// Desc: Draws a rectangle in screen space
//--------------------------------------------------------------------------------------
VOID Scene::RenderScreenAlignedQuad( FRMVECTOR2& ScreenPos, const FLOAT32 Size )
{
    glDisable( GL_DEPTH_TEST );

    ScreenPos.y = -ScreenPos.y;

    const FLOAT32 Quad[] =
    {
        ScreenPos.x + Size, ScreenPos.y - Size, 1.0f, 1.0f,
        ScreenPos.x - Size, ScreenPos.y - Size, 0.0f, 1.0f,
        ScreenPos.x + Size, ScreenPos.y + Size, 1.0f, 0.0f,
        ScreenPos.x - Size, ScreenPos.y + Size, 0.0f, 0.0f,
    };

    glVertexAttribPointer( 0, 4, GL_FLOAT, 0, 0, Quad );
    glEnableVertexAttribArray( 0 );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    glDisableVertexAttribArray( 0 );
    glEnable( GL_DEPTH_TEST );
}

*/




