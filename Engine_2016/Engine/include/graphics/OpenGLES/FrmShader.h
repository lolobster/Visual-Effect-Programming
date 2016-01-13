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
BOOL FrmCompileShaderFromString( const CHAR* strShaderSource, GLuint hShaderHandle );
BOOL FrmLoadShaderObject( const CHAR* strShaderSource, GLuint hShaderHandle );
BOOL FrmLoadShaderObjectFromFile( const CHAR* strFileName, GLuint hShaderHandle );


// Compiles a GLSL vertex shader and fragment shader into a binary shader program
BOOL FrmCompileShaderProgram( const CHAR* strVertexShaderSource,
                              const CHAR* strFragmentShaderSource, 
                              GLuint* pShaderProgramHandle,
                              const FRM_SHADER_ATTRIBUTE* pAttributes = NULL,
                              UINT32 nNumAttributes = 0 );


// Compiles a GLSL vertex shader and fragment shader into a binary shader program
BOOL FrmCompileShaderProgramFromFile( const CHAR* strVertexShaderFileName,
                                      const CHAR* strFragmentShaderFileName, 
                                      GLuint* pShaderProgramHandle,
                                      const FRM_SHADER_ATTRIBUTE* pAttributes = NULL,
                                      UINT32 nNumAttributes = 0 );


// Loads a precompiled binary shader program
/*
BOOL FrmLoadShaderProgramFromFile( const CHAR* strBinaryShaderFileName,
                                   GLuint* pShaderProgramHandle,
                                   const FRM_SHADER_ATTRIBUTE* pAttributes = NULL,
                                   UINT32 nNumAttributes = 0 );
*/

#endif // _FRM_SHADER_H_
