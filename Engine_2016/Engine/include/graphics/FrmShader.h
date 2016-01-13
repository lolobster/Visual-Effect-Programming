//--------------------------------------------------------------------------------------
// File: FrmShader.cpp
// Desc: 
//
// Author:     QUALCOMM, Advanced Content Group - Snapdragon SDK
//
//               Copyright (c) 2013 QUALCOMM Technologies, Inc. 
//                         All Rights Reserved. 
//                      QUALCOMM Proprietary/GTDR
//--------------------------------------------------------------------------------------
#ifndef _FRM_SHADER_H_
#define _FRM_SHADER_H_

#include "FrmPlatform.h"
#include <EABase/eabase.h>

#if !defined(OS_ANDROID)
#include <EGL/egl.h>
#endif // !defined(OS_ANDROID)

#include <GLES2/GL2.h>
#include <GLES2/gl2ext.h>

// Shader attribute type
struct FRM_SHADER_ATTRIBUTE
{
    const CHAR* strName;
    UINT32      nLocation;
};


// Helper functions for loading and compiling shaders
bool FrmCompileShaderFromString( const char* strShaderSource, GLuint hShaderHandle );
bool FrmLoadShaderObject( const char* strShaderSource, GLuint hShaderHandle );
bool FrmLoadShaderObjectFromFile( const char* strFileName, GLuint hShaderHandle );


// Compiles a GLSL vertex shader and fragment shader into a binary shader program
bool FrmCompileShaderProgram( const char* strVertexShaderSource,
                              const char* strFragmentShaderSource, 
                              GLuint* pShaderProgramHandle,
                              const FRM_SHADER_ATTRIBUTE* pAttributes = NULL,
                              uint32_t nNumAttributes = 0 );


// Compiles a GLSL vertex shader and fragment shader into a binary shader program
bool FrmCompileShaderProgramFromFile( const char* strVertexShaderFileName,
                                      const char* strFragmentShaderFileName, 
                                      GLuint* pShaderProgramHandle,
                                      const FRM_SHADER_ATTRIBUTE* pAttributes = NULL,
                                      uint32_t nNumAttributes = 0 );


// Loads a precompiled binary shader program
/*
BOOL FrmLoadShaderProgramFromFile( const CHAR* strBinaryShaderFileName,
                                   GLuint* pShaderProgramHandle,
                                   const FRM_SHADER_ATTRIBUTE* pAttributes = NULL,
                                   UINT32 nNumAttributes = 0 );
*/

#endif // _FRM_SHADER_H_
