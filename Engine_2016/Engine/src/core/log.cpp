#include <core/log.h>
#include <stdarg.h>

#if defined(_WIN32)
#include <Windows.h>
#include <stdio.h>
#include <es_assert.h>
#endif

#if defined(ANDROID)
#include <android/log.h>
#include <stdio.h>
#include <es_assert.h>

#endif


namespace core
{



#if defined(_WIN32)
static HANDLE hConsole = NULL;

#define COLOR_LOG		FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN 
#define COLOR_INFO		FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define COLOR_WARNING	FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY
#define COLOR_ERROR     FOREGROUND_RED | FOREGROUND_INTENSITY


#define COLOR_LOG_LUA		FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN 
#define COLOR_ERROR_LUA     FOREGROUND_RED | FOREGROUND_INTENSITY

void print_internal(WORD color, const char* const level, const char* const tag, const char* const fmt, va_list ap)
{
	static const int MAX_LOG_LINE = 10*1024;
	if( hConsole == NULL )
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, color);
	printf("%s (%s): ", level, tag);
	char s[MAX_LOG_LINE];
	vsprintf_s(s, MAX_LOG_LINE-1, fmt, ap);
	printf("%s\n",s);
}


#endif

#if defined(ANDROID)
#endif

namespace
{	
	int vsnprintf_internal(char* pDestination, size_t n, const char* pFormat, va_list arguments)
	{
	#ifdef _MSC_VER
	  return _vsnprintf(pDestination, n, pFormat, arguments);
	#else
	  return vsnprintf(pDestination, n, pFormat, arguments);
	#endif
	}
}

#define LOG_MAX_LINE 1024


void printLog(const char* const tag, const char* const fmt, ... )
{
	va_list ap;
	va_start(ap, fmt);
#if defined(_WIN32)
	print_internal(COLOR_LOG, "Log", tag, fmt, ap );
#endif
#if defined(ANDROID)
	__android_log_vprint(ANDROID_LOG_DEBUG, tag, fmt, ap);
#endif
	va_end(ap);
}

void printInfo(const char* const tag, const char* const fmt, ... )
{
	va_list ap;
	va_start(ap, fmt);
#if defined(_WIN32)
	print_internal(COLOR_INFO, "Info", tag, fmt, ap );
#endif
#if defined(ANDROID)
	__android_log_vprint(ANDROID_LOG_INFO, tag, fmt, ap);
#endif
	va_end(ap);
}

void printWarning(const char* const tag, const char* const fmt, ... )
{
	va_list ap;
	va_start(ap, fmt);
#if defined(_WIN32)
	print_internal(COLOR_WARNING, "Warning", tag, fmt, ap );
#endif
#if defined(ANDROID)
	__android_log_vprint(ANDROID_LOG_INFO, tag, fmt, ap);
#endif
	va_end(ap);
}

void printError(const char* const tag, const char* const fmt, ... )
{
	va_list ap;
	va_start(ap, fmt);
#if defined(_WIN32)
	print_internal(COLOR_ERROR, "Error", tag, fmt, ap );
#endif
#if defined(ANDROID)
	__android_log_vprint(ANDROID_LOG_ERROR, tag, fmt, ap);
#endif
	va_end(ap);
}

}

static const char* const tag = "Engine";
extern "C" void LOG_ERROR(const char*const fmt,...)
{
	va_list ap;
	va_start(ap, fmt);
#if defined(_WIN32)
	core::print_internal(COLOR_ERROR, "Error", tag, fmt, ap );
#endif
#if defined(ANDROID)
	__android_log_vprint(ANDROID_LOG_ERROR, tag, fmt, ap);
#endif
	va_end(ap);
}

extern "C" void LOG_WARNING(const char*const fmt,...)
{
	va_list ap;
	va_start(ap, fmt);
#if defined(_WIN32)
	core::print_internal(COLOR_WARNING, "Warning", tag, fmt, ap);
#endif
#if defined(ANDROID)
	__android_log_vprint(ANDROID_LOG_INFO, tag, fmt, ap);
#endif
	va_end(ap);
}

extern "C" void LOG_INFO(const char*const fmt,...)
{
	va_list ap;
	va_start(ap, fmt);
#if defined(_WIN32)
	core::print_internal(COLOR_INFO, "Info", tag, fmt, ap);
#endif
#if defined(ANDROID)
	__android_log_vprint(ANDROID_LOG_INFO, tag, fmt, ap);
#endif
	va_end(ap);
}

extern "C" void LOG(const char*const fmt,...)
{
	va_list ap;
	va_start(ap, fmt);
#if defined(_WIN32)
	core::print_internal(COLOR_LOG, "Log", tag, fmt, ap);
#endif
#if defined(ANDROID)
	__android_log_vprint(ANDROID_LOG_DEBUG, tag, fmt, ap);
#endif
	va_end(ap);
}


extern "C" void LOG_ERROR_LUA(const char*const fmt,...)
{
	va_list ap;
	va_start(ap, fmt);
#if defined(_WIN32)
	core::print_internal(COLOR_ERROR_LUA, "Lua Error", "Lua", fmt, ap);
#endif
#if defined(ANDROID)
	__android_log_vprint(ANDROID_LOG_ERROR, "Lua", fmt, ap);
#endif
	va_end(ap);
}

extern "C" void LOG_LUA(const char*const fmt,...)
{
	va_list ap;
	va_start(ap, fmt);
#if defined(_WIN32)
	core::print_internal(COLOR_LOG_LUA, "Lua Log", "Lua", fmt, ap);
#endif
#if defined(ANDROID)
	__android_log_vprint(ANDROID_LOG_DEBUG, "Lua", fmt, ap);
#endif
	va_end(ap);
}


	