// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// http://code.google.com/p/yam2d/
//
// Copyright (c) 2013 Mikko Romppainen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in the
// Software without restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
// and to permit persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies
// or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <graphics/OpenGLES/es_util.h>
/*#include <es_assert.h>
#include "es_util_win32.h"*/
#include <stdio.h>
#include <stdlib.h>
/*
#include <OGLES/Include/GLES/gl.h>
#include <OGLES/Include/EGL/egl.h>
#include <config.h>
*/

#include <core/Allocator.h>
#include <core/log.h>

/*

void* allocMem(size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
	return Engine::getDefaultAllocator()->allocate(size,4);
}

void* allocMem(size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
	return Engine::getDefaultAllocator()->allocate(size,alignment);
}

void freeMem(void* p)
{
	Engine::getDefaultAllocator()->deallocate(p);
}

*/

int Vsnprintf8(char8_t* pDestination, size_t n, const char8_t* pFormat, va_list arguments)
{
#ifdef _MSC_VER
  return _vsnprintf(pDestination, n, pFormat, arguments);
#else
  return vsnprintf(pDestination, n, pFormat, arguments);
#endif
}




namespace graphics
{

// anonymous namespace for internal functions
namespace
{
#if defined(ANDROID)
template <class _Tp>
inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) {  return  __a < __b ? __b : __a; }
#endif

#if defined(YAM_WRITING_LOGS_TO_FILE)
	const char* const logFileName = 	YAM_WRITING_LOGS_TO_FILE;
	FILE* logFileHandle = (strlen(logFileName) > 0) ? fopen (logFileName,"w") : NULL;
#endif

	int toInteger(double v)
	{
		return int(v+0.5);
	}
}


void esRegisterInitFunc ( ESContext *esContext, bool (*initFunc) ( ESContext* ) )
{
	esContext->initFunc = initFunc;
}

void esRegisterDrawFunc ( ESContext *esContext, void (*drawFunc) (ESContext* ) )
{
	esContext->drawFunc = drawFunc;
}


void esRegisterUpdateFunc ( ESContext *esContext, void (*updateFunc) ( ESContext*, float ) )
{
	esContext->updateFunc = updateFunc;
}


void esRegisterDeinitFunc ( ESContext *esContext, void (*deinitFunc) ( ESContext* ) )
{
	esContext->deinitFunc = deinitFunc;
}

void esRegisterTouchEventFunc(ESContext *esContext, void(*touchEventFunc) (ESContext*, core::TouchEventType type, int touchId, int x, int y))
{
	esContext->touchEventFunc = touchEventFunc;
}


/*


void esViewportTearEdges(int sx, int sy, float desiredAspectRatio)
{
	double w = double(sx);
	double h = double(sy);
	double curAspect = w/h;
		
	if( curAspect < desiredAspectRatio )
	{
		double desiredHeight = w/desiredAspectRatio;
		double d = 1.0 - desiredHeight/h;
		double margin = 0.5 * d;
		glViewport(  0, toInteger(margin*h), toInteger(1.0*w), toInteger((1.0 - (2.0*margin))*h) );
	}
	else if( curAspect > desiredAspectRatio )
	{
		double desiredWidth = h*desiredAspectRatio;
		double d = 1.0 - desiredWidth/w;
		double margin = 0.5 * d;
		glViewport( toInteger(margin*w), 0, toInteger((1.0 - (2.0*margin))*w), toInteger(h) );
	}
	else
	{
		glViewport( 0, 0, sx, sy );
	}
}

*/


/*
*/


eastl::string getPath( const eastl::string& fileName )
{
	eastl::string path = fileName;
	int last1 = path.find_last_of('\\');
	int last2 = path.find_last_of('/');
	
	int last = eastl::max(last1,last2);

	if( last > 0 )
	{		
		path = path.substr(0,last+1);
	}
	else
	{
		path = "";
	}

	return path;
}

eastl::string getFileName( const eastl::string& fileName )
{
	eastl::string path = fileName;
	int last1 = path.find_last_of('\\');
	int last2 = path.find_last_of('/');
	
	int last = eastl::max(last1,last2);

	if( last > 0 )
	{		
		path = path.substr(last+1, path.size());
	}
	else
	{
		path = fileName;
	}

	return path;
}
/*
std::vector<unsigned short> readFile(const char* const fileName)
{
	FILE* fp = fopen (fileName,"rb");
	std::vector<unsigned short> res;
	if (fp == NULL)
	{
		return res;
	}

	int start = ftell(fp);
	fseek(fp, 0, SEEK_END);
	int end = ftell(fp);
	res.resize((end-start)/2);
	fseek(fp,start,SEEK_SET);
	int bytesRead = fread(&res[0],2, res.size(), fp);
	(void)bytesRead;
	assert( bytesRead > 0 && bytesRead <= int(res.size()));
	fclose(fp);
	return res;
}
*/
}


void engineAssert(const char* expression, const char* file, int line )
{
	eastl::string s;
	s.sprintf("Assertation failed at %s:%d: %s", file, line, expression );
	LOG_ERROR(s.c_str());
	throw std::exception(s.c_str());
}


