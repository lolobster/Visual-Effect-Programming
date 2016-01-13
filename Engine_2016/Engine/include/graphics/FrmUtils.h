//--------------------------------------------------------------------------------------
// File: FrmUtils.h
// Desc: 
//
// Author:     QUALCOMM, Advanced Content Group - Snapdragon SDK
//
//               Copyright (c) 2013 QUALCOMM Technologies, Inc. 
//                         All Rights Reserved. 
//                      QUALCOMM Proprietary/GTDR
//--------------------------------------------------------------------------------------
#ifndef _FRM_UTILS_H_
#define _FRM_UTILS_H_

//#include <graphics/FrmPlatform.h"
#include "FrmMath.h"
#include "FrmResource.h"
#include "FrmStdLib.h"


#define FRMCOLORRGBA(r,b,g,a ) ( ((a)<<24) | ((b)<<16) | ((g)<<8) | ((r)<<0) )
#define FRMCOLORRGB(r,g,b)     ( (255<<24) | ((b)<<16) | ((g)<<8) | ((r)<<0) )

//--------------------------------------------------------------------------------------
// Primary colors
//--------------------------------------------------------------------------------------
const FRMCOLOR FRMCOLOR_BLACK             = FRMCOLORRGB(   0,   0,   0 );
const FRMCOLOR FRMCOLOR_WHITE             = FRMCOLORRGB( 255, 255, 255 );
const FRMCOLOR FRMCOLOR_RED               = FRMCOLORRGB( 255,   0,   0 );
const FRMCOLOR FRMCOLOR_GREEN             = FRMCOLORRGB(   0, 255,   0 );
const FRMCOLOR FRMCOLOR_BLUE              = FRMCOLORRGB(   0,   0, 255 );
const FRMCOLOR FRMCOLOR_YELLOW            = FRMCOLORRGB( 255, 255,   0 );
const FRMCOLOR FRMCOLOR_PURPLE            = FRMCOLORRGB( 255,   0, 255 );
const FRMCOLOR FRMCOLOR_CYAN              = FRMCOLORRGB(   0, 255, 255 );



//--------------------------------------------------------------------------------------
// Helper for building the parameter list to glClearColor(...), etc.
//--------------------------------------------------------------------------------------
#define FrmColorToParams( FrmColor ) \
    FrmColor.r / 255.0f, \
    FrmColor.g / 255.0f, \
    FrmColor.b / 255.0f, \
    FrmColor.a / 255.0f


//--------------------------------------------------------------------------------------
// Global time function
//--------------------------------------------------------------------------------------
FLOAT32 FrmGetTime();


//--------------------------------------------------------------------------------------
// Helper functions for debugging
//--------------------------------------------------------------------------------------
VOID FrmAssert( INT32 nExpression );
/*VOID FrmLogMessage( const CHAR* strPrefix, const CHAR* strMessage=NULL,
                    const CHAR* strPostFix=NULL );
					*/

//--------------------------------------------------------------------------------------
// Helper functions to read an entire file
//--------------------------------------------------------------------------------------
BOOL FrmLoadFile( const CHAR* strFileName, VOID** ppData, UINT32* pnSize = NULL );
VOID FrmUnloadFile( VOID* pData );


//--------------------------------------------------------------------------------------
// Helper functions for saving screen shots as TGA files
//--------------------------------------------------------------------------------------
BOOL FrmSaveImageAsTGA( const CHAR* strFileName, INT16 nWidth, INT16 nHeight,
                        UINT32* pBits32 );
BOOL FrmSaveImageAsTGA24( const CHAR* strFileName, INT16 nWidth, INT16 nHeight,
                         BYTE* pBits24);
BOOL FrmSaveScreenShot( const CHAR* strFileName );


//--------------------------------------------------------------------------------------
// Name: class CFrmTimer
// Desc: Basic timer class
//--------------------------------------------------------------------------------------
class CFrmTimer
{
public:
    CFrmTimer()
    {
        m_fLastTime       = FrmGetTime();

        m_strFrameRate[0] = '\0';
        m_fFPS            = 0.0f;
        m_fFrameRateTime  = m_fLastTime;
        m_nFrame          = 0;

        Reset();
        Start();
    }

    FLOAT32 Start()
    {
        m_bRunning = TRUE;
        m_fStartTime = FrmGetTime();
        return m_fTotalTimeBeforeLastStart;
    };

    FLOAT32 Stop()
    {
        m_bRunning = FALSE;
        FLOAT32 fTimeSinceLastStart = FrmGetTime() - m_fStartTime;
        m_fTotalTimeBeforeLastStart += fTimeSinceLastStart;
        return m_fTotalTimeBeforeLastStart;
    }

    VOID Reset()
    {
        m_bRunning = FALSE;
        m_fTotalTimeBeforeLastStart = 0.0f;
        m_fStartTime = 0.0f;
        m_fElapsedTime = 0.0f;
    }

    FLOAT32 GetTime()
    {
        if( m_bRunning )
        {
            FLOAT32 fTimeSinceLastStart = FrmGetTime() - m_fStartTime;
            return m_fTotalTimeBeforeLastStart + fTimeSinceLastStart;
        }
        return m_fTotalTimeBeforeLastStart;
    }

    FLOAT32 GetFrameTime()
    {
        return m_fLastTime;
    }

    FLOAT32 GetFrameElapsedTime()
    {
        return m_fElapsedTime;
    }

    VOID MarkFrame()
    {
        m_nFrame++;

        FLOAT32 fCurrentTime = FrmGetTime();
        m_fElapsedTime = fCurrentTime - m_fLastTime;
        m_fLastTime    = fCurrentTime;
    }

    FLOAT32 GetFrameRate()
    {
        FLOAT32 fCurrentTime = FrmGetTime();

        if( fCurrentTime - m_fFrameRateTime > 1.0f )
        {
            m_fFPS = m_nFrame / ( fCurrentTime - m_fFrameRateTime );
            m_fFrameRateTime = fCurrentTime;
            m_nFrame         = 0;
            return m_fFPS;
        }
        return m_fFPS;
    }

    const CHAR* GetFrameRateStr()
    {
        FrmSprintf( m_strFrameRate, 20, "%3.1f fps", GetFrameRate() );
        return m_strFrameRate;
    }

public:
    BOOL    m_bRunning;
    FLOAT32 m_fTotalTimeBeforeLastStart;
    FLOAT32 m_fStartTime;
    FLOAT32 m_fElapsedTime;
    FLOAT32 m_fLastTime;

    CHAR    m_strFrameRate[20];
    FLOAT32 m_fFPS;
    FLOAT32 m_fFrameRateTime;
    UINT32  m_nFrame;
};


//--------------------------------------------------------------------------------------
// Name: class CFrmArcBall
// Desc: A hepler class for implementing an arc ball controller
//--------------------------------------------------------------------------------------
class CFrmArcBall
{  
public:
    CFrmArcBall()
    {
        // Set the scale so that half the screen width is a PI/4 rotation
        m_fWidthScale  = 0.75f;
        m_fHeightScale = 0.75f;
        m_fRadius      = 1.0f;
        m_vStartVector = FRMVECTOR3( 0.0f, 0.0f, 1.0f );
    }

    // The radius of the arcball, in clip-space
    VOID SetRadius( FLOAT32 r )
    {
        m_fRadius = r;
    }

    // Resize the arcball parent window
    VOID Resize( UINT32 w, UINT32 h )
    {
        m_fWidthScale  = 0.75f;
        m_fHeightScale = 0.75f;
        if( w > h )
            m_fWidthScale  *= (FLOAT32)w / (FLOAT32)h;
        else
            m_fHeightScale *= (FLOAT32)h / (FLOAT32)w;
    }

    // Maps a 2D screenspace coordinate to a point on a sphere
    FRMVECTOR3 MapToSphere( FRMVECTOR2 vPoint2D )
    {
        FRMVECTOR3 vPointOnSphere;
        vPointOnSphere.x = m_fRadius * vPoint2D.x * m_fWidthScale;
        vPointOnSphere.y = m_fRadius * vPoint2D.y * m_fHeightScale;
        vPointOnSphere.z = FrmSqrt( FrmClampZero( 1.0f - vPointOnSphere.x*vPointOnSphere.x - vPointOnSphere.y*vPointOnSphere.y ) );
        return FrmVector3Normalize( vPointOnSphere );
    }

    // Begin dragging the arcball
    VOID Begin( FRMVECTOR2 vCurrentPoint )
    {
        m_vStartVector   = MapToSphere( vCurrentPoint );
        m_bIsDragging    = TRUE;
    }
    
    // While dragging, retrieve the rotation delta based on the current pointer position
    FRMVECTOR4 Update( FRMVECTOR2 vCurrentPoint )
    {
        FRMVECTOR3 vCurrentVector = MapToSphere( vCurrentPoint );
        FRMVECTOR3 vAxis  = FrmVector3Cross( m_vStartVector, vCurrentVector );
        FLOAT32    fAngle = FrmVector3Dot( m_vStartVector, vCurrentVector ); // TODO: Is this correct?
        m_vStartVector = vCurrentVector;

        return FRMVECTOR4( vAxis.x, vAxis.y, vAxis.z, -fAngle );
    }

    // End dragging the arcball
    VOID End()
    {
        m_bIsDragging = FALSE;
    }

    // Query whether the arcball is active (i.e. is being dragged)
    BOOL IsDragging()
    {
        return m_bIsDragging;
    }

    // Convenient function to handle application input
    FRMVECTOR4 HandleInput( FRMVECTOR2 vPointerPosition, BOOL bPointerDown )
    {
        if( IsDragging() )
        {
            if( bPointerDown )
                return Update( vPointerPosition );
            else
                End();
        }
        else
        {
            if( bPointerDown )
                Begin( vPointerPosition );
        }

        const FRMVECTOR4 qIdentity = FRMVECTOR4( 0.0f, 0.0f, 0.0f, 1.0f );
        return qIdentity;
    }

private:
    FLOAT32    m_fWidthScale;
    FLOAT32    m_fHeightScale;
    FLOAT32    m_fRadius;
    BOOL       m_bIsDragging;
    FRMVECTOR3 m_vStartVector;
};


#endif // _FRM_UTILS_H_
