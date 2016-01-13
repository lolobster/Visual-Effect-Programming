#ifndef _LOG_H_
#define _LOG_H_

#if defined(ANDROID)
#include <android/log.h>
//#include <stdio.h>
#endif

#include <stdarg.h>

#if defined(__cplusplus)
#define ENGINE_EXTERN_C_FUNC extern "C"
namespace core
{

struct LogTag
{
	const char* tag;
	int logPriority;
	int logColor;
};

enum LogColorFlags
{
	LOG_COLOR_BLUE_FLAG=0x0001,
	LOG_COLOR_GREEN_FLAG=0x0002,
    LOG_COLOR_RED_FLAG= 0x0004,
	LOG_COLOR_HIGH_INTENSITY_FLAG=0x0008
};

void printLog(const LogTag& tag, const char* const fmt, ... );

// Log helper functions
void printLog(const char* const tag, const char* const fmt, ... );
void printInfo(const char* const tag, const char* const fmt, ... );
void printWarning(const char* const tag, const char* const fmt, ... );
void printError(const char* const tag, const char* const fmt, ... );

}
#else
#define ENGINE_EXTERN_C_FUNC
#endif

#define ENGINE_LOG_TAG "Engine"

#define LOGS_EGABLED


#if defined(LOGS_EGABLED)
	ENGINE_EXTERN_C_FUNC void LOG_ERROR(const char*const fmt,...);	
	ENGINE_EXTERN_C_FUNC void LOG_WARNING(const char*const fmt,...);	
	ENGINE_EXTERN_C_FUNC void LOG_INFO(const char*const fmt,...);	
	ENGINE_EXTERN_C_FUNC void LOG(const char*const fmt,...);

	
	ENGINE_EXTERN_C_FUNC void LOG_ERROR_LUA(const char*const fmt,...);	
	ENGINE_EXTERN_C_FUNC void LOG_LUA(const char*const fmt,...);	
#else
	#define LOG_ERROR(fmt, ...) (void)fmt
	#define LOG_WARNING(fmt, ...) (void)fmt
	#define LOG_INFO(fmt, ...) (void)fmt
	#define LOG(fmt, ...) (void)fmt
#endif


#endif

