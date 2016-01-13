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
#include "FrmPlatform.h"
#include <graphics/FrmShader.h>
#include <graphics/FrmResource.h>
#include <graphics/FrmUtilsGLES.h>
#include <core/log.h>
#include <es_assert.h>


// Common Sample Includes
//#include "../../src/common.h"


//--------------------------------------------------------------------------------------
// Name: FrmBindShaderAttributes()
// Desc: Bind the attributes to the shader program
//--------------------------------------------------------------------------------------
VOID FrmBindShaderAttributes( GLuint hShaderProgram,
                              const FRM_SHADER_ATTRIBUTE* pAttributes,
                              uint32_t nNumAttributes )
{
    // If specific attributes were supplied, use though	
    if( nNumAttributes )
    {
        for( UINT32 i=0; i<nNumAttributes; i++ )
        {
            glBindAttribLocation( hShaderProgram, pAttributes[i].nLocation,
                                                  pAttributes[i].strName );
        }
    }
    else // Otherwise, bind some generic name mappings
    {
        glBindAttribLocation( hShaderProgram, 0, "FRM_VERTEX_ATTRIBUTE_0" );
        glBindAttribLocation( hShaderProgram, 1, "FRM_VERTEX_ATTRIBUTE_1" );
        glBindAttribLocation( hShaderProgram, 2, "FRM_VERTEX_ATTRIBUTE_2" );
        glBindAttribLocation( hShaderProgram, 3, "FRM_VERTEX_ATTRIBUTE_3" );
        glBindAttribLocation( hShaderProgram, 4, "FRM_VERTEX_ATTRIBUTE_4" );
        glBindAttribLocation( hShaderProgram, 5, "FRM_VERTEX_ATTRIBUTE_5" );
        glBindAttribLocation( hShaderProgram, 6, "FRM_VERTEX_ATTRIBUTE_6" );
        glBindAttribLocation( hShaderProgram, 7, "FRM_VERTEX_ATTRIBUTE_7" );

        glBindAttribLocation( hShaderProgram, FRM_VERTEX_POSITION,    "FRM_VERTEX_POSITION" );
        glBindAttribLocation( hShaderProgram, FRM_VERTEX_BONEWEIGHTS, "FRM_VERTEX_BONEWEIGHTS" );
        glBindAttribLocation( hShaderProgram, FRM_VERTEX_BONEINDICES, "FRM_VERTEX_BONEINDICES" );
        glBindAttribLocation( hShaderProgram, FRM_VERTEX_NORMAL,      "FRM_VERTEX_NORMAL" );
        glBindAttribLocation( hShaderProgram, FRM_VERTEX_TANGENT,     "FRM_VERTEX_TANGENT" );
        glBindAttribLocation( hShaderProgram, FRM_VERTEX_BINORMAL,    "FRM_VERTEX_BINORMAL" );
        glBindAttribLocation( hShaderProgram, FRM_VERTEX_TEXCOORD0,   "FRM_VERTEX_TEXCOORD0" );
        glBindAttribLocation( hShaderProgram, FRM_VERTEX_TEXCOORD1,   "FRM_VERTEX_TEXCOORD1" );
    }
}

//--------------------------------------------------------------------------------------
// Name: FrmCompileShaderFromString()
// Desc: 
//--------------------------------------------------------------------------------------
bool FrmCompileShaderFromString( const char* strShaderSource, GLuint hShaderHandle )
{
    glShaderSource( hShaderHandle, 1, &strShaderSource, NULL );
    glCompileShader( hShaderHandle );

    // Check for compile success
    GLint nCompileResult = 0;
    glGetShaderiv( hShaderHandle, GL_COMPILE_STATUS, &nCompileResult );
    if( 0 == nCompileResult )
    {
		char strInfoLog[1024];
		GLint nLength;
		glGetShaderInfoLog( hShaderHandle, 1024, &nLength, strInfoLog );
		LOG_ERROR("Unable to compile shader: %s", strInfoLog);
		LOG_ERROR(strShaderSource);
		assert(0);
        return false;
    }

    return true;
}


//--------------------------------------------------------------------------------------
// Name: FrmLoadShaderObjectFromFile()
// Desc: Install the given shaders into OpenGL for use by the pipeline.
//--------------------------------------------------------------------------------------
bool FrmLoadShaderObjectFromFile( const char* strFileName, GLuint hShaderHandle )
{
    char* strShaderSource;
    if( FALSE == FrmLoadFile( strFileName, (VOID**)&strShaderSource ) )
    {
        LOG_ERROR( "ERROR: Could not load shader file %s" );
        return FALSE;
    }

    bool bResult = FrmCompileShaderFromString( strShaderSource, hShaderHandle );
    if( FALSE == bResult )
    {
        LOG_ERROR( "ERROR: Could compile shader file %s" );
    }

	FrmUnloadFile( strShaderSource );
    return bResult;
}


//--------------------------------------------------------------------------------------
// Name: FrmLinkShaderProgram()
// Desc: Helper function to link a shader program
//--------------------------------------------------------------------------------------
bool FrmLinkShaderProgram( UINT32 hShaderProgram )
{
    // Link the whole program together
    glLinkProgram( hShaderProgram );

    // Check for link success
    GLint LinkStatus;
    glGetProgramiv( hShaderProgram, GL_LINK_STATUS, &LinkStatus );
    if( FALSE == LinkStatus )
    {
        char  strInfoLog[1024];
        INT32 nLength;
        glGetProgramInfoLog( hShaderProgram, 1024, &nLength, strInfoLog );
        LOG_ERROR( strInfoLog, "\n" );
        return FALSE;
    }

    return TRUE;
}


//--------------------------------------------------------------------------------------
// Name: FrmCompileShaderProgram()
// Desc: 
//--------------------------------------------------------------------------------------
bool FrmCompileShaderProgram( const char* strVertexShader,
                              const char* strFragmentShader, 
                              GLuint* pShaderProgramHandle,
                              const FRM_SHADER_ATTRIBUTE* pAttributes,
                              UINT32 nNumAttributes )
{
    // Create the object handles
    GLuint hVertexShader   = glCreateShader( GL_VERTEX_SHADER );
    GLuint hFragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

    // Compile the shaders
    if( !FrmCompileShaderFromString( strVertexShader, hVertexShader ) )
    {
        glDeleteShader( hVertexShader );
        glDeleteShader( hFragmentShader );
        return FALSE;
    }
    if( !FrmCompileShaderFromString( strFragmentShader, hFragmentShader ) )
    {
        glDeleteShader( hVertexShader );
        glDeleteShader( hFragmentShader );
        return FALSE;
    }

    // Attach the individual shaders to the common shader program
    GLuint hShaderProgram  = glCreateProgram();
    glAttachShader( hShaderProgram, hVertexShader );
    glAttachShader( hShaderProgram, hFragmentShader );

    // Bind the shader attributes
    FrmBindShaderAttributes( hShaderProgram, pAttributes, nNumAttributes );

    // Link the vertex shader and fragment shader together
    if( FALSE == FrmLinkShaderProgram( hShaderProgram ) )
    {
        glDeleteProgram( hShaderProgram );
        return FALSE;
    }

    // It's now safe to toss away the original shader handles
    glDeleteShader( hVertexShader );
    glDeleteShader( hFragmentShader );

    // Return the shader program
    (*pShaderProgramHandle) = hShaderProgram;
    return TRUE;
}


//--------------------------------------------------------------------------------------
// Name: FrmCompileShaderProgramFromFile()
// Desc: 
//--------------------------------------------------------------------------------------
bool FrmCompileShaderProgramFromFile( const char* strVertexShaderFileName,
                                      const char* strFragmentShaderFileName, 
                                      GLuint* pShaderProgramHandle,
                                      const FRM_SHADER_ATTRIBUTE* pAttributes,
                                      UINT32 nNumAttributes )
{
    // Create the object handles
    GLuint hVertexShader   = glCreateShader( GL_VERTEX_SHADER );
    GLuint hFragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

    // Compile the shaders
    if( !FrmLoadShaderObjectFromFile( strVertexShaderFileName, hVertexShader ) )
    {
        glDeleteShader( hVertexShader );
        glDeleteShader( hFragmentShader );
        return FALSE;
    }
    if( !FrmLoadShaderObjectFromFile( strFragmentShaderFileName, hFragmentShader ) )
    {
        glDeleteShader( hVertexShader );
        glDeleteShader( hFragmentShader );
        return FALSE;
    }


    // Attach the individual shaders to the common shader program
    GLuint hShaderProgram = glCreateProgram();
    glAttachShader( hShaderProgram, hVertexShader );
    glAttachShader( hShaderProgram, hFragmentShader );

    // Bind the attributes to the shader program
    FrmBindShaderAttributes( hShaderProgram, pAttributes, nNumAttributes );

    // Link the vertex shader and fragment shader together
    if( FALSE == FrmLinkShaderProgram( hShaderProgram ) )
    {
        glDeleteProgram( hShaderProgram );
        return FALSE;
    }

    // It's now safe to toss away the original shader handles
    glDeleteShader( hVertexShader );
    glDeleteShader( hFragmentShader );

    // Return the shader program
    (*pShaderProgramHandle) = hShaderProgram;
    return TRUE;
}

/*
//--------------------------------------------------------------------------------------
// Name: FrmLoadShaderProgramFromFile()
// Desc: 
//--------------------------------------------------------------------------------------
bool FrmLoadShaderProgramFromFile( const char* strBinaryShaderFileName,
                                   GLuint* pShaderProgramHandle,
                                   const FRM_SHADER_ATTRIBUTE* pAttributes,
                                   UINT32 nNumAttributes )
{
    // Load the shader source
    char* strShaderSource;
    UINT32 nShaderLength;
    if( FALSE == FrmLoadFile( strBinaryShaderFileName, (VOID**)&strShaderSource,
                              &nShaderLength ) )
    {
        FrmLogMessage( "ERROR: Could not load shader file '", strBinaryShaderFileName, "'\n" );
        return FALSE;
    }

    // Create the object handles
    GLuint hVertexShader   = glCreateShader( GL_VERTEX_SHADER );
    GLuint hFragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    GLuint shaders[2] = { hVertexShader, hFragmentShader };

    // Compile the shader
    glShaderBinary( 2, shaders, GL_BINARY_FORMAT_OPENGL_ES_2_0_AMD_EMULATOR,
                    strShaderSource, nShaderLength );
    FrmUnloadFile( strShaderSource );

    // Check for compile success
    GLint nCompileResult1 = 0;
    GLint nCompileResult2 = 0;
    glGetShaderiv( hVertexShader, GL_COMPILE_STATUS, &nCompileResult1 );
    glGetShaderiv( hFragmentShader, GL_COMPILE_STATUS, &nCompileResult2 );
    if( 0 == nCompileResult1 || 0 == nCompileResult2 )
    {
        char strInfoLog[1024];
        GLint nLength;
        glGetShaderInfoLog( hVertexShader, 1024, &nLength, strInfoLog );
        if( strInfoLog[0] ) FrmLogMessage( strInfoLog, "\n" );
        glGetShaderInfoLog( hFragmentShader, 1024, &nLength, strInfoLog );
        if( strInfoLog[0] ) FrmLogMessage( strInfoLog, "\n" );
        glDeleteShader( hVertexShader );
        glDeleteShader( hFragmentShader );
        return FALSE;
    }

    // Attach the individual shaders to the common shader program
    GLuint hShaderProgram  = glCreateProgram();
    glAttachShader( hShaderProgram, hVertexShader );
    glAttachShader( hShaderProgram, hFragmentShader );

    // Bind the attributes to the shader program
    FrmBindShaderAttributes( hShaderProgram, pAttributes, nNumAttributes );
    
    // Link the vertex shader and fragment shader together
    if( FALSE == FrmLinkShaderProgram( hShaderProgram ) )
    {
        glDeleteProgram( hShaderProgram );
        return FALSE;
    }

    // It's now safe to toss away the original shader handles
    glDeleteShader( hVertexShader );
    glDeleteShader( hFragmentShader );

    // Return the shader program
    (*pShaderProgramHandle) = hShaderProgram;
    return TRUE;
}

*/