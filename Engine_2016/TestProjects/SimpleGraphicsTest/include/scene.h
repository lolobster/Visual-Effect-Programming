//=============================================================================
// FILE: scene.h
//
// Create and render a scene
//
// AUTHOR:          QUALCOMM
//
//                  Copyright (c) 2008 QUALCOMM Incorporated.
//                              All Rights Reserved.
//                          QUALCOMM Proprietary/GTDR
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_


// graphics includes
#include <graphics/FrmInput.h>
#include <graphics/FrmMesh.h>
#include <graphics/FrmShader.h>
#include <core/Object.h>
#include <core/Ref.h>
#include <graphics\Texture.h>
#include <graphics/OpenGLES/es_util.h>

//=============================================================================
// CONSTANTS
//=============================================================================
#define ROTATE_SPEED        0.025f


#define TIME_ONE_SECOND	    1000
#define PI                  3.141592653589793238462f
#define TWO_PI              6.283185307179586476925f

//=============================================================================
// HELPER MACROS
//=============================================================================
#define SETVEC3(P, X, Y, Z)     {P[0] = X; P[1] = Y; P[2] = Z;}
#define SETVEC4(P, X, Y, Z, W)  {P[0] = X; P[1] = Y; P[2] = Z; P[3] = W;}

//=============================================================================
// TYPEDEFS
//=============================================================================

namespace Engine
{
//--------------------------------------------------------------------------------------
// Name: struct OffscreenRT
// Desc: Stores information and handles for off-screen render targets
//--------------------------------------------------------------------------------------
struct OffscreenRT
{
    OffscreenRT();
    ~OffscreenRT();
    BOOL CreateRT( UINT32 DesiredWidth, UINT32 DesiredHeight );
    VOID FreeRT();
    VOID SetFramebuffer();

    UINT32  BufferHandle;       // GL_FRAMEBUFFER
    UINT32  TextureHandle;      // GL_TEXTURE_2D
    UINT32  DepthTextureHandle;
    UINT32  Width;
    UINT32  Height;
};

//--------------------------------------------------------------------------------------
// Name: struct OffscreenRT4
// Desc: Stores information and handles for off-screen render targets
//--------------------------------------------------------------------------------------



}


//=============================================================================
// CScene Class
//=============================================================================
class Scene : public core::Object
{
public:
    Scene();
    virtual ~Scene();


    virtual void update(graphics::ESContext *esContext, float deltaTime) = 0;
	virtual void render(graphics::ESContext *esContext) = 0;

    // Functions common to multiple scenes
//    BYTE*           LoadTGA( const CHAR* strFileName, UINT32* pWidth, UINT32* pHeight, UINT32* nFormat );
//    VOID            SetTexture( INT32 ShaderVar, UINT32 TextureHandle, INT32 SamplerSlot );
    void            checkOpenGL();
//    BOOL            LoadCubeMap(const CHAR* ImageFileName);
//    BOOL            CheckFrameBufferStatus();
//    VOID            RenderScreenAlignedQuad();
//	VOID			RenderScreenAlignedQuadInv();
//    VOID            RenderScreenAlignedQuad( FRMVECTOR2& ScreenPos, const FLOAT32 Size );
//    VOID            RenderScreenAlignedQuad( FRMVECTOR2 TopLeft, FRMVECTOR2 BottomRight );



private:

public:
//	int foo;
 /*   const CHAR* m_strName;
    INT32       m_nWidth;
    INT32       m_nHeight;


    FRM_INPUT   m_Input;

    GLuint      m_CubemapHandle;
	*/
};



#endif

