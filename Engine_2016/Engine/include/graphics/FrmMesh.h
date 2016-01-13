//--------------------------------------------------------------------------------------
// File: FrmMesh.h
// Desc: 
//
// Author:     QUALCOMM, Advanced Content Group - Snapdragon SDK
//
//               Copyright (c) 2013 QUALCOMM Technologies, Inc. 
//                         All Rights Reserved. 
//                      QUALCOMM Proprietary/GTDR
//--------------------------------------------------------------------------------------
#ifndef _FRM_MESH_H_
#define _FRM_MESH_H_

#include "FrmPlatform.h"
#include "OpenGLES/FrmResourceGLES.h"
#include "FrmResource.h"
#include "FrmMath.h"


//--------------------------------------------------------------------------------------
// Forward references
//--------------------------------------------------------------------------------------
struct FRM_MESH_FRAME;
struct FRM_ANIMATION;


//--------------------------------------------------------------------------------------
// Enums and defintions
//--------------------------------------------------------------------------------------
const UINT32 FRM_ANIMATION_TICKS_PER_SEC = 4800;


//--------------------------------------------------------------------------------------
// Name: struct FRM_MESH_TEXTURE
// Desc: 
//--------------------------------------------------------------------------------------
struct FRM_MESH_TEXTURE
{
    CHAR               m_strTexture[32];
    CFrmTexture*       m_pTexture;
};


//--------------------------------------------------------------------------------------
// Name: struct FRM_MESH_BONE
// Desc: 
//--------------------------------------------------------------------------------------
struct FRM_MESH_BONE
{
    FRM_MESH_FRAME*   m_pFrame;             // The mesh frame associated with this bone
    FRMMATRIX4X4      m_matBoneOffset;
};


//--------------------------------------------------------------------------------------
// Name: struct FRM_MESH_SUBSET
// Desc: 
//--------------------------------------------------------------------------------------
struct FRM_MESH_SUBSET
{
    UINT32             m_nPrimType;
    UINT32             m_nNumIndices;
    UINT32             m_nIndexOffset;

    UINT32*            m_pBoneIds;

    FRMVECTOR4         m_vDiffuseColor;
    FRMVECTOR4         m_vSpecularColor;
    FRMVECTOR3         m_vAmbientColor;
    FRMVECTOR3         m_vEmissiveColor;

    UINT32             m_nNumTextures;
    FRM_MESH_TEXTURE*  m_pTextures;
};


//--------------------------------------------------------------------------------------
// Name: struct FRM_MESH
// Desc: 
//--------------------------------------------------------------------------------------
struct FRM_MESH
{
    FRM_VERTEX_ELEMENT m_VertexLayout[8];
    UINT32             m_Reserved;

    UINT32             m_nNumVertices;
    UINT16             m_nVertexSize;
    UINT32             m_hVertexBuffer;

    UINT32             m_nNumIndices;
    UINT16             m_nIndexSize;
    UINT32             m_hIndexBuffer;

    UINT32             m_nNumBonesPerVertex;
    UINT32             m_nNumBoneMatrices;
    UINT32             m_nNumBones;
    FRM_MESH_BONE*     m_pBones;

    UINT32             m_nNumSubsets;
    FRM_MESH_SUBSET*   m_pSubsets;
};


//--------------------------------------------------------------------------------------
// Name: struct FRM_MESH_FRAME
// Desc: 
//--------------------------------------------------------------------------------------
struct FRM_ANIMATION;

struct FRM_MESH_FRAME
{
    CHAR               m_strName[32];

    FRMMATRIX4X4       m_matTransform;
    FRMMATRIX4X4       m_matBoneOffset2;
    FRMMATRIX4X4       m_matCombined;

    FRM_MESH*          m_pMesh;
    FRM_ANIMATION*     m_pAnimationData;

    FRM_MESH_FRAME*    m_pChild;
    FRM_MESH_FRAME*    m_pNext;
};


//--------------------------------------------------------------------------------------
// Name: struct FRM_MESH_FILE_HEADER
// Desc: 
//--------------------------------------------------------------------------------------
struct FRM_MESH_FILE_HEADER
{
    UINT32 nMagic;
    UINT32 nSystemDataSize;
    UINT32 nBufferDataSize;
    UINT32 nNumFrames;
};

const UINT32 FRM_MESH_MAGIC_ID = ( ('M'<<0L) | ('S'<<8L) | ('H'<<16) | (1<<24L) );


//--------------------------------------------------------------------------------------
// Name: struct FRM_ANIMATION_KEY
// Desc: 
//--------------------------------------------------------------------------------------
struct FRM_ANIMATION_KEY
{
    FRMVECTOR3         m_vScale;
    FRMVECTOR4         m_vRotate;
    FRMVECTOR3         m_vTranslate;
};


//--------------------------------------------------------------------------------------
// Name: struct FRM_ANIMATION
// Desc: 
//--------------------------------------------------------------------------------------
struct FRM_ANIMATION
{
    CHAR               m_strFrameName[32];
    BYTE               m_Reserved0[32];

    FRM_MESH_FRAME*    m_pMeshFrame;
    BYTE               m_Reserved1[40];

    UINT32             m_nNumKeys;
    UINT32*            m_pKeyTimes;
    FRM_ANIMATION_KEY* m_pKeyValues;
};


//--------------------------------------------------------------------------------------
// Name: struct FRM_ANIMATION_SET
// Desc: 
//--------------------------------------------------------------------------------------
struct FRM_ANIMATION_SET
{
    CHAR               m_strName[64];
    UINT32             m_nPeriodInTicks;
    UINT32             m_nNumAnimations;
    FRM_ANIMATION*     m_pAnimations;

    VOID SetTime( UINT32 nPeriodicAnimTime );
};


//--------------------------------------------------------------------------------------
// Name: struct FRM_ANIMATION_FILE_HEADER
// Desc: 
//--------------------------------------------------------------------------------------
struct FRM_ANIMATION_FILE_HEADER
{
    UINT32 m_nMagicID;
    UINT32 m_nDataSize;
    UINT32 m_nAnimationsOffset;
    UINT32 m_nKeyTimesOffset;
    UINT32 m_nKeyValuesOffset;
};

const UINT32 FRM_ANIMATION_FILE_MAGIC_ID = (('A'<<0)|('N'<<8)|('I'<<16)|('M'<<24));


//--------------------------------------------------------------------------------------
// Name: FrmReadAnimation()
// Desc: Reads animation data from a file
//--------------------------------------------------------------------------------------
BOOL FrmReadAnimation( const CHAR* strFileName, FRM_ANIMATION_SET** ppAnimationSet );


//--------------------------------------------------------------------------------------
// Name: class CFrmMesh
// Desc: 
//--------------------------------------------------------------------------------------
class CFrmMesh
{
public:
    CFrmMesh()
    {
        m_pBufferData = 0;
        m_pSystemData = 0;
        m_pFrames     = 0;
        m_nNumFrames  = 0;
    }

    ~CFrmMesh()
    {
        Destroy();
    }

    BOOL Load( const CHAR* strFileName );
    VOID Destroy();

    UINT32 GetNumSubsets( UINT32 nFrame )
    {
        if( 0 == m_pFrames[nFrame].m_pMesh )
            return 0;
        return m_pFrames[nFrame].m_pMesh->m_nNumSubsets;
    }

    FRM_MESH* GetMesh( UINT32 nFrame )
    {
        return m_pFrames[nFrame].m_pMesh;
    };

    FRM_MESH* GetMeshData( UINT32 nFrame, VOID** ppVertexData = 0, 
                           VOID** ppIndexData = 0 )
    {
        FRM_MESH* pMesh = m_pFrames[nFrame].m_pMesh;
        if( 0 == pMesh )
            return 0;

        if( m_pBufferData )
        {
            if( ppVertexData ) (*ppVertexData) = m_pBufferData + (UINT32)pMesh->m_hVertexBuffer;
            if( ppIndexData )  (*ppIndexData)  = m_pBufferData + (UINT32)pMesh->m_hIndexBuffer;
        }
        else
        {
            if( ppVertexData ) (*ppVertexData) = 0;
            if( ppIndexData )  (*ppIndexData)  = 0;
        }

        return pMesh;
    };

    VOID RenderSubset( UINT32 nFrame, UINT32 nSubset )
    {
        FRM_MESH* pMesh = m_pFrames[nFrame].m_pMesh;
        if( 0 == pMesh )
            return;
        FRM_MESH_SUBSET* pSubset = &pMesh->m_pSubsets[nSubset];

		FrmClearVertexAttributeArrays();

        for( UINT32 i=0; i<pSubset->m_nNumTextures; i++ )
        {
            FrmSetTexture( i, pSubset->m_pTextures[i].m_pTexture ? pSubset->m_pTextures[i].m_pTexture->m_hTextureHandle : 0 );
        }

        FrmSetVertexBuffer( pMesh->m_hVertexBuffer );
        FrmSetVertexLayout( pMesh->m_VertexLayout, pMesh->m_nVertexSize );
        FrmSetIndexBuffer( pMesh->m_hIndexBuffer );
        FrmDrawIndexedVertices( pSubset->m_nPrimType, pSubset->m_nNumIndices, 
                                pMesh->m_nIndexSize, pSubset->m_nIndexOffset );
        FrmSetVertexBuffer( 0 );
    }

    VOID Render()
    {
        for( UINT32 i = 0; i < m_nNumFrames; i++ )
        {
            FRM_MESH* pMesh = m_pFrames[i].m_pMesh;
            if( 0 == pMesh )
                continue;
        
            for( UINT32 j = 0; j < pMesh->m_nNumSubsets; j++ )
            {
                RenderSubset( i, j );
            }
        }
    }

public:
    UINT32          m_nSystemDataSize;
    UINT32          m_nBufferDataSize;
    BYTE*           m_pSystemData;
    BYTE*           m_pBufferData;

    FRM_MESH_FRAME* m_pFrames;
    UINT32          m_nNumFrames;
};


#endif // _FRM_MESH_H_
