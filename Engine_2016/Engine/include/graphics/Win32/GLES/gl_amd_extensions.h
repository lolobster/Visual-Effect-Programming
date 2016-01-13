/*

 Copyright (C) 2009-2010 QUALCOMM Incorporated.
 All Rights Reserved. QUALCOMM Proprietary and Confidential.
*/
#ifndef __gl_amd_extensions_h_
#define __gl_amd_extensions_h_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


//////////////////////////////////////////////////////////////////////////////
//
// GL_AMD_performance_monitor
//
//////////////////////////////////////////////////////////////////////////////
#ifndef GL_AMD_performance_monitor
#define GL_AMD_performance_monitor

#define GL_COUNTER_TYPE_AMD                             0x8BC0
#define GL_COUNTER_RANGE_AMD                            0x8BC1
#define GL_UNSIGNED_INT64_AMD                           0x8BC2
#define GL_PERCENTAGE_AMD                               0x8BC3
#define GL_PERFMON_RESULT_AVAILABLE_AMD                 0x8BC4
#define GL_PERFMON_RESULT_SIZE_AMD                      0x8BC5
#define GL_PERFMON_RESULT_AMD                           0x8BC6

GL_API void GL_APIENTRY glGetPerfMonitorGroupsAMD(GLint *numGroups, GLsizei groupsSize, GLuint *groups);
GL_API void GL_APIENTRY glGetPerfMonitorCountersAMD(GLuint group, GLint *numCounters, GLint *maxActiveCounters, GLsizei counterSize, GLuint *counters);
GL_API void GL_APIENTRY glGetPerfMonitorGroupStringAMD(GLuint group, GLsizei bufSize, GLsizei *length, char *groupString);
GL_API void GL_APIENTRY glGetPerfMonitorCounterStringAMD(GLuint group, GLuint counter, GLsizei bufSize, GLsizei *length, char *counterString);
GL_API void GL_APIENTRY glGetPerfMonitorCounterInfoAMD(GLuint group, GLuint counter, GLenum pname, void *data);
GL_API void GL_APIENTRY glGenPerfMonitorsAMD(GLsizei n, GLuint *monitors);
GL_API void GL_APIENTRY glDeletePerfMonitorsAMD(GLsizei n, GLuint *monitors);
GL_API void GL_APIENTRY glSelectPerfMonitorCountersAMD(GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint *countersList);
GL_API void GL_APIENTRY glBeginPerfMonitorAMD(GLuint monitor);
GL_API void GL_APIENTRY glEndPerfMonitorAMD(GLuint monitor);
GL_API void GL_APIENTRY glGetPerfMonitorCounterDataAMD(GLuint monitor, GLenum pname, GLsizei dataSize, GLuint *data, GLint *bytesWritten);

#endif /* GL_AMD_performance_monitor */

//////////////////////////////////////////////////////////////////////////////
//
// Private entry points for QXProfiler support
//
//////////////////////////////////////////////////////////////////////////////

#define GL_TEXTURE_WIDTH_QCOM                           0x8BD2
#define GL_TEXTURE_HEIGHT_QCOM                          0x8BD3
#define GL_TEXTURE_DEPTH_QCOM                           0x8BD4
#define GL_TEXTURE_INTERNAL_FORMAT_QCOM                 0x8BD5
#define GL_TEXTURE_FORMAT_QCOM                          0x8BD6
#define GL_TEXTURE_TYPE_QCOM                            0x8BD7
#define GL_TEXTURE_IMAGE_VALID_QCOM                     0x8BD8
#define GL_TEXTURE_NUM_LEVELS_QCOM                      0x8BD9
#define GL_TEXTURE_TARGET_QCOM                          0x8BDA
#define GL_TEXTURE_OBJECT_VALID_QCOM                    0x8BDB
#define GL_STATE_RESTORE                                0x8BDC
#define GL_ALPHA_TEST_FACTOR_ATI                        0x998D //TEMP TOKEN

#define GL_BLEND_COLOR							0x8005

GL_API void GL_APIENTRY glExtGetTexturesQCOM(GLuint *textures, GLint maxTextures, GLint *numTextures );
GL_API void GL_APIENTRY glExtGetBuffersQCOM(GLuint *buffers, GLint maxBuffers , GLint *numBuffers);
GL_API void GL_APIENTRY glExtGetRenderbuffersQCOM(GLuint *renderbuffers, GLint maxRenderbuffers, GLint *numRenderbuffers );
GL_API void GL_APIENTRY glExtGetFramebuffersQCOM(GLuint *framebuffers, GLint maxFramebuffers, GLint *numFramebuffers );
GL_API void GL_APIENTRY glExtGetTexLevelParameterivQCOM( GLuint texture, GLenum face, GLint level, GLenum pname, GLint * params );
GL_API void GL_APIENTRY glExtTexObjectStateOverrideiQCOM( GLenum target, GLenum pname, GLint param );
GL_API void GL_APIENTRY glExtGetProgramBinarySourceQCOM(GLuint program, GLenum shadertype, char* source, GLint* length);
GL_API void GL_APIENTRY glExtGetTexSubImageQCOM(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset,
                                                GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLvoid *texels);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __gl_amd_extensions_h_ */

