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

//#define WIN32_LEAN_AND_MEAN
#include <windowsx.h>

#include <graphics/OpenGLES/es_util.h>
#include <es_assert.h>
#include <core/Input.h>
#include <graphics/OpenGLES/es_util_win32.h>
#include <core/log.h>
#include <core/ElapsedTimer.h>

namespace core
{
	void initInput();

	void unititInput();
}

namespace graphics
{

// anonymous namespace for internal functions
namespace
{

int xPos = 0;//GET_X_LPARAM(lParam); 
int yPos = 0;//GET_Y_LPARAM(lParam); 
bool leftClicked = false;//(wParam & MK_LBUTTON) != 0;
bool rightClicked = false;//(wParam & MK_RBUTTON) != 0;
bool middleClicked = false;// (wParam & MK_MBUTTON) != 0;


LRESULT WINAPI ESWindowProc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) 
{
	LRESULT  lRet = 1; 
	assert( hWnd != 0 );
	switch (uMsg) 
	{
	case WM_CREATE:
		break;
		
	case WM_PAINT:
		{
			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr (hWnd, GWL_USERDATA );
			assert( esContext != 0 );

			if ( esContext->drawFunc )
			{
				esContext->drawFunc ( esContext );
				eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
			}   

			ValidateRect( esContext->hWnd, NULL );
		}
		break;
	
	case WM_SIZING:
		{
			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
			assert( esContext != 0 );
			RECT rc;
			GetClientRect(hWnd,&rc);
			esContext->height = rc.bottom - rc.top;
			esContext->width = rc.right - rc.left;
		}
		break;

	case WM_SIZE:
		{
			RECT rc;
			GetClientRect(hWnd,&rc);
			int w = rc.right - rc.left;
			int h = rc.bottom - rc.top;
			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
			if( esContext != 0 )
			{
				esContext->height = h;
				esContext->width = w;
			}
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);             
		break; 
      

	case WM_LBUTTONDOWN:
		{
			SetCapture(hWnd);
			leftClicked = true;
			core::mouseState(leftClicked, rightClicked, middleClicked, xPos, yPos);
		
			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
			if( esContext != 0 )
			{
				touchEventFunc(esContext, core::TOUCH_BEGIN, 0, xPos, yPos);
			}
		}
		break;

	case WM_LBUTTONUP:
		{			
			core::mouseState(false, rightClicked, middleClicked, xPos, yPos);

			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
			if( esContext != 0 && leftClicked )
			{
				core::touchEventFunc(esContext, core::TOUCH_END, 0, xPos, yPos);
			}
			leftClicked = false;
			SetCapture(leftClicked||rightClicked||middleClicked ? hWnd : 0);
		}
		break;

	case WM_RBUTTONDOWN:
		{
			SetCapture(hWnd);
			rightClicked = true;
			core::mouseState(leftClicked, rightClicked, middleClicked, xPos, yPos);

			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
			if( esContext != 0 )
			{
				core::touchEventFunc(esContext, core::TOUCH_BEGIN, 1, xPos, yPos);
			}
		}
		break;

	case WM_RBUTTONUP:
		{			
			core::mouseState(leftClicked, false, middleClicked, xPos, yPos);

			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
			if( esContext != 0 && rightClicked )
			{
				core::touchEventFunc(esContext, core::TOUCH_END, 1, xPos, yPos);
			}
			rightClicked = false;
			SetCapture(leftClicked||rightClicked||middleClicked ? hWnd : 0);
		}
		break;

	case WM_MBUTTONDOWN:
		{
			SetCapture(hWnd);
			middleClicked = true;
			core::mouseState(leftClicked, rightClicked, middleClicked, xPos, yPos);

			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
			if( esContext != 0 )
			{
				core::touchEventFunc(esContext, core::TOUCH_BEGIN, 2, xPos, yPos);
			}
		}
		break;

	case WM_MBUTTONUP:
		{		
			core::mouseState(leftClicked, rightClicked, false, xPos, yPos);

			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
			if( esContext != 0 && middleClicked)
			{
				core::touchEventFunc(esContext, core::TOUCH_END, 2, xPos, yPos);
			}
			middleClicked = false;
			SetCapture(leftClicked||rightClicked||middleClicked ? hWnd : 0);
		}
		break;

	case  WM_CAPTURECHANGED:
		{
			if( GetCapture() == 0 )
			{
				core::mouseState(false, false, false, xPos, yPos);
		
				ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
				if( esContext != 0 )
				{
					if (leftClicked) core::touchEventFunc(esContext, core::TOUCH_CANCEL, 0, xPos, yPos);
					if (rightClicked) core::touchEventFunc(esContext, core::TOUCH_CANCEL, 1, xPos, yPos);
					if (middleClicked) core::touchEventFunc(esContext, core::TOUCH_CANCEL, 2, xPos, yPos);
				}
			
				middleClicked = false;
				rightClicked = false;
				leftClicked = false;
			}
		}		
		break;

	case WM_MOUSEWHEEL:
		{ 
			//esLogEngineError("WM_MOUSEWHEEL");
			int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			core::mouseWheel(delta / WHEEL_DELTA);
		}
		break;

	case WM_MOUSEMOVE:
		{
			POINT point;
			point.x = GET_X_LPARAM(lParam);
			point.y = GET_Y_LPARAM(lParam);
			ClientToScreen(hWnd,&point);
			MapWindowPoints(HWND_DESKTOP,hWnd,&point,1);
			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
			if( point.x < 0 || point.y < 0 ||
				point.x >= esContext->width ||  point.y >= esContext->height )
			{
				SetCapture(0);
			}
			else
			{
				xPos = point.x;
				yPos = point.y;
			
				if( esContext != 0 )
				{
					if (leftClicked) core::touchEventFunc(esContext, core::TOUCH_MOVE, 0, xPos, yPos);
					if (rightClicked) core::touchEventFunc(esContext, core::TOUCH_MOVE, 1, xPos, yPos);
					if (middleClicked) core::touchEventFunc(esContext, core::TOUCH_MOVE, 2, xPos, yPos);
				}

				core::mouseState(leftClicked, rightClicked, middleClicked, xPos, yPos);
			}

			
		}
		break;

	default: 
		lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
		break; 
	} 

	return lRet; 
}

}


GLboolean winCreate ( ESContext *esContext, LPCWSTR title, bool resizable )
{
	assert( esContext != 0 );

	WNDCLASS wndclass = {0}; 
	DWORD    wStyle   = 0;
	RECT     windowRect;
	HINSTANCE hInstance = GetModuleHandle(NULL);


	wndclass.style         = CS_OWNDC;
	wndclass.lpfnWndProc   = (WNDPROC)ESWindowProc; 
	wndclass.hInstance     = hInstance; 
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); 
	wndclass.lpszClassName = TEXT("opengles1.x"); 

	if (!RegisterClass (&wndclass) ) 
	{
		LOG_ERROR("Failed to register wndclass");
		return FALSE; 
	}

	if( resizable )
	{
		wStyle = WS_VISIBLE | WS_MAXIMIZEBOX | WS_BORDER | WS_SYSMENU | WS_CAPTION | WS_THICKFRAME;
	}
	else
	{
		wStyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;
	}

	// Adjust the window rectangle so that the client area has
	// the correct number of pixels
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = esContext->width;
	windowRect.bottom = esContext->height;

	AdjustWindowRect ( &windowRect, wStyle, FALSE );
	
	esContext->hWnd = CreateWindow(
									TEXT("opengles1.x"),
									title,
									wStyle,
									10,
									10,
									windowRect.right - windowRect.left,
									windowRect.bottom - windowRect.top,
									NULL,
									NULL,
									hInstance,
									NULL);

	if ( esContext->hWnd == NULL )
	{
		LOG_ERROR("Failed to create window");
		return GL_FALSE;
	}

	// Set user data
	SetWindowLongPtr( esContext->hWnd, GWL_USERDATA, (LONG) (LONG_PTR) esContext );
	
	// Show window
	ShowWindow ( esContext->hWnd, TRUE );

	return GL_TRUE;
}


void winLoop ( ESContext *esContext )
{
	// We must set thread affinity mask for this thread, inorder that QueryPerformanceCounter does not cause troubles for us.
	// See atricle "Game Timing and Multicore Processors (Windows)" http://msdn.microsoft.com/en-us/library/windows/desktop/ee417693(v=vs.85).aspx
	// Enable only core 1, by using mask 0x01.
	SetThreadAffinityMask(GetCurrentThread(), 0x01);

	assert( esContext != 0 );
	MSG msg = { 0 };
	bool done = false;
	core::ElapsedTimer timer;
	timer.reset();

	//bool gotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);
	if( false == esContext->initFunc(esContext) )
	{
		return;
	}

	while (!done)
	{
		if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{
			if( msg.message==WM_QUIT )
			{
				done = true;
				continue;
			}

			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
		else
		{		
			// Call update function if registered
			if ( esContext->updateFunc != NULL )
			{
				float deltaTime = timer.getTime();
				timer.reset();
				if( deltaTime > 0.0f )
				{
					esContext->updateFunc ( esContext, deltaTime );
					core::clearInput();
				}			
			}

			SendMessage( esContext->hWnd, WM_PAINT, 0, 0 );
		}
	}

	if ( esContext->deinitFunc != NULL )
	{
		esContext->deinitFunc ( esContext );
	}
}

EGLBoolean CreateEGL11Context ( EGLNativeWindowType hWnd, EGLDisplay* eglDisplay,
                              EGLContext* eglContext, EGLSurface* eglSurface,
                              EGLint attribList[])
{
	assert(eglDisplay != 0);
	assert(eglSurface != 0);
	assert(eglContext != 0);

	EGLint numConfigs;
	EGLDisplay display;
	EGLContext context;
	EGLSurface surface;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };


	// Get Display
	display = eglGetDisplay(GetDC(hWnd));
	if ( display == EGL_NO_DISPLAY )
	{
		LOG_ERROR("eglGetDisplay failed");
		return EGL_FALSE;
	}

  // Set our EGL API to OpenGL ES
    if ( eglBindAPI( EGL_OPENGL_ES_API ) == EGL_FALSE )
    {
        LOG_ERROR("eglBindAPI FAILED\n");
        return EGL_FALSE;
    }

	    // Initialize EGL
    EGLint nMajorVersion;
    EGLint nMinorVersion;
    if ( eglInitialize(display, &nMajorVersion, &nMinorVersion) == EGL_FALSE )
    {
        LOG_ERROR("eglInitialize FAILED\n");
        return false;
    }
	LOG_INFO("EGL version: %d.%d Initialized",nMajorVersion,nMinorVersion);

    // Get the total number of available configs
    if ( eglGetConfigs(display, NULL, 0, &numConfigs) == EGL_FALSE )
    {
        LOG_ERROR("eglGetConfigs FAILED\n");
        return false;
    }

// Build the requested attibute list
    EGLint eglAttributeList[] = 
    { 
        EGL_RED_SIZE,       8,
        EGL_GREEN_SIZE,     8,
        EGL_BLUE_SIZE,      8,
        EGL_ALPHA_SIZE,     8,
        EGL_DEPTH_SIZE,     16,
        EGL_STENCIL_SIZE,   0,
        EGL_SAMPLE_BUFFERS, 0,
        EGL_SAMPLES,        0,
        EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,
        EGL_NONE
    };

    // Create space for the config list
	eastl::vector<EGLConfig> eglConfigList;
	eglConfigList.resize(numConfigs);

    // Get all the configurations that minimally satisfy our requested attributes
    if ( eglChooseConfig( display, eglAttributeList, &eglConfigList[0], numConfigs, &numConfigs ) == EGL_FALSE )
    {
        LOG_ERROR("eglChooseConfig FAILED\n");
        return false;
    }

    int nConfig = 0;
    BOOL bFoundConfig = false;

    // Try each configuration to see if it matches our attribute list
    while (nConfig < numConfigs && !bFoundConfig)
    {
        EGLint nAttribValue = 0;
        UINT32 nAttrib=0; 

        //assume this is the right config
        bFoundConfig = true;

        // Check the requested attributes
        while (eglAttributeList[nAttrib] != EGL_NONE && bFoundConfig)
        {
            if ( eglGetConfigAttrib(display, eglConfigList[nConfig], eglAttributeList[nAttrib], &nAttribValue) == EGL_FALSE )
            {
                LOG_ERROR("eglGetConfigAttrib (2) FAILED\n");
            }
            if (eglAttributeList[nAttrib + 1] != nAttribValue)
            {
                // [Jedi voice] "This is not the config you are looking for"
                bFoundConfig = false;
            }
            nAttrib += 2;
        }

        // If we have not found a matching config, try the next one
        if (!bFoundConfig)
        {
            nConfig++;
        }
    }

    if (!bFoundConfig)
    {
        // None of the configs match our requested attribute list

        // We could panic and return false, or just try to pick the first config 
        // that has a minimal match of attributes. 
        nConfig = 0;
        if ( eglChooseConfig( display, eglAttributeList, &eglConfigList[nConfig], 1, &numConfigs ) == EGL_FALSE )
        {
            LOG_ERROR("eglChooseConfig (2) FAILED\n");
            return false;
        }
    }

	// Create a surface
	surface = eglCreateWindowSurface(display, eglConfigList[nConfig], (EGLNativeWindowType)hWnd, NULL);
	if ( surface == EGL_NO_SURFACE )
	{
		LOG_ERROR("eglCreateWindowSurface failed");
		return EGL_FALSE;
	}

	// Create a GL context
	context = eglCreateContext(display, eglConfigList[nConfig], EGL_NO_CONTEXT, contextAttribs );
	if ( context == EGL_NO_CONTEXT )
	{
		LOG_ERROR("eglCreateContext failed");
		return EGL_FALSE;
	}   
   
	// Make the context current
	if ( !eglMakeCurrent(display, surface, surface, context) )
	{
		LOG_ERROR("eglMakeCurrent failed");
		return EGL_FALSE;
	}
   
	*eglDisplay = display;
	*eglSurface = surface;
	*eglContext = context;
	return EGL_TRUE;
} 




EGLBoolean CreateEGL20Context ( EGLNativeWindowType hWnd, EGLDisplay* eglDisplay,
                              EGLContext* eglContext, EGLSurface* eglSurface,
                              EGLint attribList[])
{
	assert(eglDisplay != 0);
	assert(eglSurface != 0);
	assert(eglContext != 0);

	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLDisplay display;
	EGLContext context;
	EGLSurface surface;
	EGLConfig config;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

	// Get Display
	display = eglGetDisplay(GetDC(hWnd));
	if ( display == EGL_NO_DISPLAY )
	{
		LOG_ERROR("eglGetDisplay failed");
		return EGL_FALSE;
	}

	// Initialize EGL
	if ( !eglInitialize(display, &majorVersion, &minorVersion) )
	{
		LOG_ERROR("eglInitialize failed");
		return EGL_FALSE;
	}
	
	LOG_INFO("EGL version: %d.%d Initialized",majorVersion,minorVersion);
	
	// Get configs
	if ( !eglGetConfigs(display, NULL, 0, &numConfigs) )
	{
		LOG_ERROR("eglGetConfigs failed");
		return EGL_FALSE;
	}

	// Choose config
	if ( !eglChooseConfig(display, attribList, &config, 1, &numConfigs) )
	{
		LOG_ERROR("eglChooseConfig failed");
		return EGL_FALSE;
	}

	// Create a surface
	surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, NULL);
	if ( surface == EGL_NO_SURFACE )
	{
		LOG_ERROR("eglCreateWindowSurface failed");
		return EGL_FALSE;
	}

	// Create a GL context
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );
	if ( context == EGL_NO_CONTEXT )
	{
		LOG_ERROR("eglCreateContext failed");
		return EGL_FALSE;
	}   
   
	// Make the context current
	if ( !eglMakeCurrent(display, surface, surface, context) )
	{
		LOG_ERROR("eglMakeCurrent failed");
		return EGL_FALSE;
	}
   
	*eglDisplay = display;
	*eglSurface = surface;
	*eglContext = context;
	return EGL_TRUE;
} 


/*

void esInitContext ( ESContext *esContext )
{
	YAM2D_START
	assert ( esContext != NULL );
	memset( esContext, 0, sizeof( ESContext) );

}
*/

GLboolean esCreateWindow ( ESContext *esContext, GLint width, GLint height, GLint flags, Version version )
{
	assert( esContext != 0 );
	EGLint attribList[] =
	{
		EGL_RED_SIZE,       8,
		EGL_GREEN_SIZE,     8,
		EGL_BLUE_SIZE,      8,
		EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
		EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
		EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
		EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
		EGL_NONE
	};
   
	if ( esContext == NULL )
	{
		LOG_ERROR("Given ESContext is NULL");
		return GL_FALSE;
	}

	esContext->width = width;
	esContext->height = height;

	if ( !winCreate ( esContext, TEXT("Engine window"), (flags&ES_WINDOW_RESIZEABLE) ? true : false ) )
	{
		return GL_FALSE;
	}

  
	if( version == ES_VERSION_2 )
	{
		if ( !CreateEGL20Context( esContext->hWnd, &esContext->eglDisplay, &esContext->eglContext,
								&esContext->eglSurface,	attribList) )
		{
			return GL_FALSE;
		}
	}
	else if ( version == ES_VERSION_1 )
	{
		if ( !CreateEGL11Context( esContext->hWnd, &esContext->eglDisplay, &esContext->eglContext,
								&esContext->eglSurface,	attribList) )
		{
			return GL_FALSE;
		}
	}
	else
	{
		LOG_ERROR("Invalid OpenGL ES version: %d", version);
	}


	return GL_TRUE;
}
/*
void initInput();

void unititInput();
*/
void esMainLoop ( ESContext *esContext )
{
	core::initInput();
	winLoop( esContext );
	core::unititInput();
}


}

