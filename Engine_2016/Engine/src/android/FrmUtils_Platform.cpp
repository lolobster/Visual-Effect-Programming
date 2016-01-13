//--------------------------------------------------------------------------------------
// File: FrmUtils_Win32.cpp
// Desc: Win32 implementation of the graphics utility helper functions
//
// Author:     QUALCOMM, Advanced Content Group - Adreno SDK
//
//               Copyright (c) 2013 QUALCOMM Technologies, Inc. 
//                         All Rights Reserved. 
//                      QUALCOMM Proprietary/GTDR
//--------------------------------------------------------------------------------------
#include "FrmPlatform.h"
#include <graphics/FrmUtils.h>

#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <android/log.h>


//--------------------------------------------------------------------------------------
// Name: FrmAssert()
// Desc: Platform-dependent assert function.
//--------------------------------------------------------------------------------------
VOID FrmAssert( INT32 nExpression )
{
    // Nothing on Android for now
}


//--------------------------------------------------------------------------------------
// Name: FrmLogMessage()
// Desc: Platform-dependent debug spew functions
//--------------------------------------------------------------------------------------
VOID FrmLogMessage( const CHAR* strPrefix, const CHAR* strMessage,
                    const CHAR* strPostFix )
{
    //typedef enum android_LogPriority {
    //            ANDROID_LOG_UNKNOWN = 0,
    //            ANDROID_LOG_DEFAULT,    /* only for SetMinPriority() */
    //            ANDROID_LOG_VERBOSE,
    //            ANDROID_LOG_DEBUG,
    //            ANDROID_LOG_INFO,
    //            ANDROID_LOG_WARN,
    //            ANDROID_LOG_ERROR,
    //            ANDROID_LOG_FATAL,
    //            ANDROID_LOG_SILENT,     /* only for SetMinPriority(); must be last */
    //            } android_LogPriority;

    if( strPrefix )  
        __android_log_write(ANDROID_LOG_INFO, "OGLES 2.0 Shaders", strPrefix);
    if( strMessage ) 
        __android_log_write(ANDROID_LOG_INFO, "OGLES 2.0 Shaders", strMessage);
    if( strPostFix ) 
        __android_log_write(ANDROID_LOG_INFO, "OGLES 2.0 Shaders", strPostFix);

}



//--------------------------------------------------------------------------------------
// Name: FrmGetTime()
// Desc: Platform-dependent function to get the current time (in seconds).
//--------------------------------------------------------------------------------------
FLOAT32 FrmGetTime()
{
    static BOOL     bInitialized = FALSE;
    static UINT32 m_llBaseTime;

    struct timeval t;
    if( FALSE == bInitialized )
    {
        // Get the base time
        t.tv_sec = t.tv_usec = 0;

        if(gettimeofday(&t, NULL) == -1)
        {
            return 0.0f;
        }

        m_llBaseTime = (UINT32)(t.tv_sec*1000LL + t.tv_usec/1000LL);

        bInitialized = TRUE;
        return 0.0f;
    }

    // Get the current time
    t.tv_sec = t.tv_usec = 0;
    if(gettimeofday(&t, NULL) == -1)
    {
        return 0.0f;
    }

    UINT32 TimeNow = (UINT32)(t.tv_sec*1000LL + t.tv_usec/1000LL);

    // Now figure out elapsed time from base time in seconds
    FLOAT32 fAppTime = (FLOAT32)( TimeNow - m_llBaseTime ) / 1000.0f;
    return fAppTime;
}

