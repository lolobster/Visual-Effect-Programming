//--------------------------------------------------------------------------------------
// File: FrmResourceGLES.cpp
// Desc: Wrappers for OpenGL resources to ease their use
//
// Author:     QUALCOMM, Advanced Content Group - Snapdragon SDK
//
//               Copyright (c) 2013 QUALCOMM Technologies, Inc. 
//                         All Rights Reserved. 
//                      QUALCOMM Proprietary/GTDR
//--------------------------------------------------------------------------------------
#include <graphics/FrmResourceGLES.h>

//--------------------------------------------------------------------------------------
// Name: FrmCreateTexture()
// Desc: Create a texture from the supplied initial data.
//--------------------------------------------------------------------------------------
BOOL FrmCreateTexture( UINT32 nWidth, UINT32 nHeight, UINT32 nNumLevels,
                       UINT32 nInternalPixelFormat, UINT32 nPixelType,
                       UINT32 nBorder, VOID* pInitialData, UINT32 nBaseSize,
                       CFrmTexture** ppTexture )
{
    if( (!pInitialData) || (!ppTexture) )
        return FALSE;

    GLint hTextureHandle;
    if( false == FrmCreateTextureHandle( nWidth, nHeight, nNumLevels,
                                   nInternalPixelFormat, nPixelType, nBorder,
                                   pInitialData, nBaseSize, &hTextureHandle ) )
    {
//		LOG_ERROR( "FrmCreateTexture Failed!" );
        // Tracking down a problem that may be state bleed, may be driver error, may be 
        // something else.
        // It manifests as a failure to load a texture.  By simply waiting and trying
        // again it seems to work.  Put this here as part of tracking it down
        if( FALSE == FrmCreateTextureHandle( nWidth, nHeight, nNumLevels,
            nInternalPixelFormat, nPixelType, nBorder,
            pInitialData, nBaseSize, &hTextureHandle ) )
        {
            return FALSE;
        }
    }

    CFrmTexture* pTexture = new CFrmTexture();
    pTexture->m_nWidth         = nWidth;
    pTexture->m_nHeight        = nHeight;
    pTexture->m_nNumLevels     = nNumLevels;
    pTexture->m_hTextureHandle = hTextureHandle;
    (*ppTexture) = pTexture;

    return TRUE;
}


//--------------------------------------------------------------------------------------
// Name: FrmCreateTextureHandle()
// Desc: Create a texture from the supplied initial data.
//--------------------------------------------------------------------------------------
BOOL FrmCreateTextureHandle( UINT32 nWidth, UINT32 nHeight, UINT32 nNumLevels,
                       UINT32 nInternalPixelFormat, UINT32 nPixelType,
                       UINT32 nBorder, VOID* pInitialData, UINT32 nBaseSize,
                       GLint* pTexId )
{
    if( (!pInitialData) || (!pTexId) )
        return FALSE;

    // Create a texture
    glGenTextures( 1, (GLuint*)pTexId );
    glBindTexture( GL_TEXTURE_2D, *pTexId );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT );
    if( nNumLevels > 1 )
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

    UINT32 nSubPixelFormat = nInternalPixelFormat;
    BOOL   bCompressed     = FALSE;
    if( nInternalPixelFormat == GL_ATC_RGB_AMD || nInternalPixelFormat == GL_ATC_RGBA_EXPLICIT_ALPHA_AMD )
        bCompressed     = TRUE;

    // Create the mip chain
    {
        UINT32 nMipWidth  = nWidth;
        UINT32 nMipHeight = nHeight;
        UINT32 nBPP       = 8 * nBaseSize / ( nWidth * nHeight );
        UINT32 nMipSize   = 16 * ((nMipWidth+3)/4) * ((nMipHeight+3)/4) * nBPP / 8;

        // Populate the mip-levels
        for( UINT32 nLevel=0; nLevel<nNumLevels; nLevel++ )
        {
            // Load the mip data
            if( bCompressed )
            {
                //LogInfo("Creating compressed texture!\n");
                glCompressedTexImage2D( GL_TEXTURE_2D, nLevel, nInternalPixelFormat, 
                                        nMipWidth, nMipHeight, nBorder, nMipSize, pInitialData );
            }
            else
            {
                //LogInfo("Creating uncompressed texture!\n");
                glTexImage2D( GL_TEXTURE_2D, nLevel, nInternalPixelFormat, nMipWidth, nMipHeight,
                              nBorder, nSubPixelFormat, nPixelType, pInitialData );
            }

            // Advance to the next level
            pInitialData = (VOID*)( (BYTE*)pInitialData + nMipSize );
            if( nMipWidth > 1 )  nMipWidth  /= 2;
            if( nMipHeight > 1 ) nMipHeight /= 2;
            nMipSize = 16 * ((nMipWidth+3)/4) * ((nMipHeight+3)/4) * nBPP / 8;
        }
    }

    GLenum test = glGetError();
    if( GL_NO_ERROR != test )
    {
//        LOG_ERROR("Error creating texture!\n");
        return FALSE;
    }

    return TRUE;
}


//--------------------------------------------------------------------------------------
// Name: FrmCreateTexture()
// Desc: Create a 3D texture from the supplied initial data.
//--------------------------------------------------------------------------------------
BOOL FrmCreateTexture3D( UINT32 nWidth, UINT32 nHeight, UINT32 nDepth, UINT32 nNumLevels,
                         UINT32 nInternalPixelFormat, UINT32 nPixelType, UINT32 nBorder,
                         VOID* pInitialData, UINT32 nBaseSize, CFrmTexture3D** ppTexture )
{
    if( (!pInitialData) || (!ppTexture) )
        return FALSE;

    GLint hTextureHandle;
    if( FALSE == FrmCreateTexture3DHandle( nWidth, nHeight, nDepth, nNumLevels,
                                     nInternalPixelFormat, nPixelType, nBorder,
                                     pInitialData, nBaseSize, &hTextureHandle ) )
        return FALSE;

    CFrmTexture3D* pTexture = new CFrmTexture3D();
    pTexture->m_nWidth         = nWidth;
    pTexture->m_nHeight        = nHeight;
    pTexture->m_nDepth         = nDepth;
    pTexture->m_hTextureHandle = hTextureHandle;
    (*ppTexture) = pTexture;

    return TRUE;
}


//--------------------------------------------------------------------------------------
// Name: FrmCreateTexture()
// Desc: Create a 3D texture from the supplied initial data.
//--------------------------------------------------------------------------------------
BOOL FrmCreateTexture3DHandle( UINT32 nWidth, UINT32 nHeight, UINT32 nDepth, UINT32 nNumLevels,
                         UINT32 nInternalPixelFormat, UINT32 nPixelType, UINT32 nBorder,
                         VOID* pInitialData, UINT32 nBaseSize, GLint* pTexId )
{
    if( (!pInitialData) || (!pTexId) )
        return FALSE;

    // Create a texture
    glGenTextures( 1, (GLuint*)pTexId );
    glBindTexture( GL_TEXTURE_3D_OES, *pTexId );
    glTexParameteri( GL_TEXTURE_3D_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_3D_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_3D_OES, GL_TEXTURE_WRAP_S,     GL_REPEAT );
    glTexParameteri( GL_TEXTURE_3D_OES, GL_TEXTURE_WRAP_T,     GL_REPEAT );
    glTexParameteri( GL_TEXTURE_3D_OES, GL_TEXTURE_WRAP_R_OES, GL_REPEAT );
    if( nNumLevels > 1 )
        glTexParameteri( GL_TEXTURE_3D_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

    UINT32 nSubPixelFormat = nInternalPixelFormat;
    BOOL   bCompressed     = FALSE;
    if( nInternalPixelFormat == GL_ATC_RGB_AMD || nInternalPixelFormat == GL_ATC_RGBA_EXPLICIT_ALPHA_AMD )
        bCompressed     = TRUE;

    // Create the mip-chain for each slice
    {
        UINT32 nMipWidth  = nWidth;
        UINT32 nMipHeight = nHeight;
        UINT32 nBPP       = 8 * nBaseSize / ( nWidth * nHeight );
        UINT32 nMipSize   = 16 * ((nMipWidth+3)/4) * ((nMipHeight+3)/4) * nBPP / 8;

        // Populate the mip-levels
        for( UINT32 nLevel=0; nLevel<nNumLevels; nLevel++ )
        {
            // Load the mip data
            if( bCompressed )
                glCompressedTexImage3DOES( GL_TEXTURE_3D_OES, nLevel, nInternalPixelFormat, 
                                           nMipWidth, nMipHeight, nDepth, nBorder, nMipSize, pInitialData );
            else
                glTexImage3DOES( GL_TEXTURE_3D_OES, nLevel, nInternalPixelFormat,
                                 nMipWidth, nMipHeight, nDepth, nBorder, nSubPixelFormat, nPixelType, pInitialData );

            // Advance to the next level
            pInitialData = (VOID*)( (BYTE*)pInitialData + nMipSize );
            if( nMipWidth > 1 )  nMipWidth  /= 2;
            if( nMipHeight > 1 ) nMipHeight /= 2;
            nMipSize = 16 * ((nMipWidth+3)/4) * ((nMipHeight+3)/4) * nBPP / 8;
        }
    }

    if( GL_NO_ERROR != glGetError() )
        return FALSE;

    return TRUE;
}


//--------------------------------------------------------------------------------------
// Name: FrmCreateCubeMap()
// Desc: Create a cubemap from the supplied initial data.
//--------------------------------------------------------------------------------------
BOOL FrmCreateCubeMap( UINT32 nWidth, UINT32 nHeight, UINT32 nNumLevels,
                       UINT32 nInternalPixelFormat, UINT32 nPixelType,
                       UINT32 nBorder, VOID* pInitialData, UINT32 nBaseSize,
                       CFrmCubeMap** ppCubeMap )
{
    if( (!pInitialData) || (!ppCubeMap) )
        return FALSE;

    GLint hTextureHandle;
    if( FALSE == FrmCreateCubeMapHandle( nWidth, nHeight, nNumLevels,
                                   nInternalPixelFormat, nPixelType, nBorder,
                                   pInitialData, nBaseSize, &hTextureHandle ) )
    {
        // Tracking down a problem that may be state bleed, may be driver error, may be 
        // something else.
        // It manifests as a failure to load a texture.  By simply waiting and trying
        // again it seems to work.  Put this here as part of tracking it down
        if( FALSE == FrmCreateCubeMapHandle( nWidth, nHeight, nNumLevels,
            nInternalPixelFormat, nPixelType, nBorder,
            pInitialData, nBaseSize, &hTextureHandle ) )
        {
//            LOG_ERROR("Unable to create CubeMap in GL\n");
            return FALSE;
        }
    }

    CFrmCubeMap* pCubeMap = new CFrmCubeMap();
    pCubeMap->m_nWidth         = nWidth;
    pCubeMap->m_nHeight        = nHeight;
    pCubeMap->m_hTextureHandle = hTextureHandle;
    (*ppCubeMap) = pCubeMap;

    return TRUE;
}


//--------------------------------------------------------------------------------------
// Name: FrmCreateCubeMapHandle()
// Desc: Create a cubemap from the supplied initial data.
//--------------------------------------------------------------------------------------
BOOL FrmCreateCubeMapHandle( UINT32 nWidth, UINT32 nHeight, UINT32 nNumLevels, 
                       UINT32 nInternalPixelFormat, UINT32 nPixelType,
                       UINT32 nBorder, VOID* pInitialData, UINT32 nBaseSize,
                       GLint* pTexId )
{
    if( (!pInitialData) || (!pTexId) )
        return FALSE;

    // Create a texture
    glGenTextures( 1, (GLuint*)pTexId );
    glBindTexture( GL_TEXTURE_CUBE_MAP, *pTexId );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE );
    if( nNumLevels > 1 )
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

    UINT32 nSubPixelFormat = nInternalPixelFormat;
    BOOL   bCompressed     = FALSE;
    if( nInternalPixelFormat == GL_ATC_RGB_AMD || nInternalPixelFormat == GL_ATC_RGBA_EXPLICIT_ALPHA_AMD )
        bCompressed     = TRUE;

    // Create the mip-chain for each face
    for( UINT32 nFace=0; nFace<6; nFace++ )
    {
        UINT32 nMipWidth  = nWidth;
        UINT32 nMipHeight = nHeight;
        UINT32 nBPP       = 8 * nBaseSize / ( nWidth * nHeight );
        UINT32 nMipSize   = 16 * ((nMipWidth+3)/4) * ((nMipHeight+3)/4) * nBPP / 8;

        // Populate the mip-levels
        for( UINT32 nLevel=0; nLevel<nNumLevels; nLevel++ )
        {
            // Load the mip data
            if( bCompressed )
            {
//                LOG("Creating compressed cube texture: CUBE_MAP_POSITIVE_X + %d\n", nFace);
                glCompressedTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + nFace, nLevel, nInternalPixelFormat, 
                                        nMipWidth, nMipHeight, nBorder, nMipSize, pInitialData );
            }
            else
            {
  //              LOG("Creating uncompressed cube texture: CUBE_MAP_POSITIVE_X + %d\n", nFace);
                glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + nFace, nLevel, nInternalPixelFormat,
                              nMipWidth, nMipHeight, nBorder, nSubPixelFormat, nPixelType, pInitialData );
            }


            if( GL_NO_ERROR != glGetError() )
            {
    //            LOG("Unable to create cube texture\n");
                return FALSE;
            }

            // Advance to the next level
            pInitialData = (VOID*)( (BYTE*)pInitialData + nMipSize );
            if( nMipWidth > 1 )  nMipWidth  /= 2;
            if( nMipHeight > 1 ) nMipHeight /= 2;
            nMipSize = 16 * ((nMipWidth+3)/4) * ((nMipHeight+3)/4) * nBPP / 8;
        }
    }

    return TRUE;
}


//--------------------------------------------------------------------------------------
// Name: FrmCreateVertexBuffer()
// Desc: Create a vertex buffer from the supplied initial data.
//--------------------------------------------------------------------------------------
BOOL FrmCreateVertexBuffer( UINT32 nNumVertices, UINT32 nVertexSize, VOID* pSrcVertices,
                            CFrmVertexBuffer** ppVertexBuffer )
{
    if( (!pSrcVertices) || (!ppVertexBuffer) )
        return FALSE;

    GLuint hBufferHandle;
    if( FALSE == FrmCreateVertexBuffer( nNumVertices, nVertexSize, pSrcVertices,
                                        &hBufferHandle ) )
        return FALSE;

    CFrmVertexBuffer* pVertexBuffer = new CFrmVertexBuffer();
    pVertexBuffer->m_nNumVertices   = nNumVertices;
    pVertexBuffer->m_nVertexSize    = nVertexSize;
    pVertexBuffer->m_hBufferHandle  = hBufferHandle;
    (*ppVertexBuffer) = pVertexBuffer;

    return TRUE;
}


//--------------------------------------------------------------------------------------
// Name: FrmCreateVertexBuffer()
// Desc: Create a vertex buffer from the supplied initial data.
//--------------------------------------------------------------------------------------
BOOL FrmCreateVertexBuffer( UINT32 nNumVertices, UINT32 nVertexSize, VOID* pSrcVertices,
                            GLuint* pBufferHandle )
{
    if( (!pSrcVertices) || (!pBufferHandle) )
        return FALSE;

    GLuint hBufferHandle;
    glGenBuffers( 1, &hBufferHandle );
    glBindBuffer( GL_ARRAY_BUFFER, hBufferHandle );
    glBufferData( GL_ARRAY_BUFFER, nNumVertices*nVertexSize, NULL, GL_DYNAMIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, nNumVertices*nVertexSize, pSrcVertices ); 
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    
    (*pBufferHandle) = hBufferHandle;
    return TRUE;
}


//--------------------------------------------------------------------------------------
// Name: FrmCreateIndexBuffer()
// Desc: Create an index buffer from the supplied initial data.
//--------------------------------------------------------------------------------------
BOOL FrmCreateIndexBuffer( UINT32 nNumIndices, UINT32 nIndexSize, VOID* pSrcIndices,
                           CFrmIndexBuffer** ppIndexBuffer )
{
    if( (!pSrcIndices) || (!ppIndexBuffer) )
        return FALSE;

    GLuint hBufferHandle;
    if( FALSE == FrmCreateIndexBuffer( nNumIndices, nIndexSize, pSrcIndices,
                                       &hBufferHandle ) )
        return FALSE;

    CFrmIndexBuffer* pIndexBuffer  = new CFrmIndexBuffer();
    pIndexBuffer->m_nNumIndices    = nNumIndices;
    pIndexBuffer->m_nIndexSize     = nIndexSize;
    pIndexBuffer->m_hBufferHandle  = hBufferHandle;
    (*ppIndexBuffer) = pIndexBuffer;

    return TRUE;
}


//--------------------------------------------------------------------------------------
// Name: FrmCreateIndexBuffer()
// Desc: Create an index buffer from the supplied initial data.
//--------------------------------------------------------------------------------------
BOOL FrmCreateIndexBuffer( UINT32 nNumIndices, UINT32 nIndexSize, VOID* pSrcIndices,
                           GLuint* pBufferHandle )
{
    if( (!pSrcIndices) || (!pBufferHandle) )
        return FALSE;

    GLuint hBufferHandle;
    glGenBuffers( 1, &hBufferHandle );
    glBindBuffer( GL_ARRAY_BUFFER, hBufferHandle );
    glBufferData( GL_ARRAY_BUFFER, nNumIndices*nIndexSize, NULL, GL_DYNAMIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, nNumIndices*nIndexSize, pSrcIndices ); 
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    
    (*pBufferHandle) = hBufferHandle;
    return TRUE;
}

 
//--------------------------------------------------------------------------------------
// Name: CFrmFrameBufferObject()
// Desc: FBO constructor
//--------------------------------------------------------------------------------------
CFrmFrameBufferObject::CFrmFrameBufferObject()
{
    m_hTexture      = 0;
    m_hFrameBuffer  = 0;
    m_hRenderBuffer = 0;
}


//--------------------------------------------------------------------------------------
// Name: ~CFrmFrameBufferObject()
// Desc: FBO destructor
//--------------------------------------------------------------------------------------
CFrmFrameBufferObject::~CFrmFrameBufferObject()
{
    Destroy();
}


//--------------------------------------------------------------------------------------
// Name: Create()
// Desc: Creates objects for rendering to an FBO
//--------------------------------------------------------------------------------------
BOOL CFrmFrameBufferObject::Create( UINT32 nWidth, UINT32 nHeight, UINT32 nFormat, UINT32 nType )
{
    Destroy();

    m_nWidth  = nWidth;
    m_nHeight = nHeight;
    m_nFormat = nFormat;
    m_nType   = nType;

    // Create an offscreen texture
    glGenTextures( 1, &m_hTexture );
    glBindTexture( GL_TEXTURE_2D, m_hTexture );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE );
    glTexImage2D( GL_TEXTURE_2D, 0, nFormat, nWidth, nHeight, 0, nFormat, nType, NULL );

    if( m_nFormat == GL_DEPTH_COMPONENT )
    {
        glGenFramebuffers( 1, &m_hFrameBuffer );
        glBindFramebuffer( GL_FRAMEBUFFER, m_hFrameBuffer );
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_hTexture, 0 );
    }
    else // ( m_nFormat == GL_RGB || m_nFormat == GL_RGBA )
    {
        glGenRenderbuffers( 1, &m_hRenderBuffer );
        glBindRenderbuffer( GL_RENDERBUFFER, m_hRenderBuffer );
        glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_nWidth, m_nHeight );

        glGenFramebuffers( 1, &m_hFrameBuffer );
        glBindFramebuffer( GL_FRAMEBUFFER, m_hFrameBuffer );
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_hTexture, 0 );
        glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_hRenderBuffer );
    }

    if( GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus( GL_FRAMEBUFFER ) )
        return FALSE;

    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glBindRenderbuffer( GL_RENDERBUFFER, 0 );

    return TRUE;
}


//--------------------------------------------------------------------------------------
// Name: Destroy()
// Desc: Destroys objects used to render to an FBO
//--------------------------------------------------------------------------------------
VOID CFrmFrameBufferObject::Destroy()
{
    glDeleteTextures( 1, &m_hTexture );
    glDeleteFramebuffers( 1, &m_hFrameBuffer );
    glDeleteRenderbuffers( 1, &m_hRenderBuffer );
}


//--------------------------------------------------------------------------------------
// Name: Begin()
// Desc: Binds objects for rendering to an FBO
//--------------------------------------------------------------------------------------
VOID CFrmFrameBufferObject::Begin()
{
    // Save values so we can restore the viewport when finished rendering to the FBO
    glGetIntegerv( GL_VIEWPORT, (GLint*)&m_PreviousViewport );

    glBindFramebuffer( GL_FRAMEBUFFER, m_hFrameBuffer );
 
    if( m_nFormat == GL_DEPTH_COMPONENT )
    {
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_hTexture, 0 );
    }
    else // ( m_nFormat == GL_RGB || m_nFormat == GL_RGBA )
    {
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_hTexture, 0 );
        glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_hRenderBuffer );
    }
    
    glViewport( 0, 0, m_nWidth, m_nHeight );
}


//--------------------------------------------------------------------------------------
// Name: End()
// Desc: Ends the rendering to an FBO
//--------------------------------------------------------------------------------------
VOID CFrmFrameBufferObject::End()
{
    // Restore rendering the previous framebuffer
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glViewport( m_PreviousViewport.x, m_PreviousViewport.y,
                m_PreviousViewport.w, m_PreviousViewport.h );
}

