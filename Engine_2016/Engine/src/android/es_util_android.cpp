/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

//BEGIN_INCLUDE(all)
#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>

#include <GLES2/GL2.h>
#include <GLES2/gl2ext.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <es_util.h>
#include <es_assert.h>
#include <core/ElapsedTimer.h>

#include <core/log.h>


struct android_app* g_androidState = 0;


//extern const char* g_pInternalDataPath;


static bool initDone = false; 
static bool windowCreated = false;
static bool shallQuit = false;

//extern "C" void  app_dummy();

int main ( int argc, char *argv[] );

namespace Engine
{
	
	static const char*const tag = "es_util_android.cpp";

	void eglCheckError(const char*const funcName)
	{
		Engine::printInfo( tag, "eglCheckError: %s", funcName );
		EGLint err = eglGetError();

		if( err != EGL_SUCCESS )
		{
			switch( err )
			{
			case EGL_NOT_INITIALIZED:
				Engine::printError( tag, "egl error: EGL_NOT_INITIALIZED\n" );
				break;

			case EGL_BAD_ACCESS:
				Engine::printError( tag, "egl error: EGL_BAD_ACCESS\n" );
				break;

			case EGL_BAD_ALLOC:
				Engine::printError( tag, "egl error: EGL_BAD_ALLOC\n" );
				break;

			case EGL_BAD_ATTRIBUTE:
				Engine::printError( tag, "egl error: EGL_BAD_ATTRIBUTE\n" );
				break;
        
			case EGL_BAD_CONTEXT:
				Engine::printError( tag, "egl error: EGL_BAD_CONTEXT\n" );
				break;
			        
			case EGL_BAD_CONFIG:
				Engine::printError( tag, "egl error: EGL_BAD_CONFIG\n" );
				break;

			case EGL_BAD_CURRENT_SURFACE:
				Engine::printError( tag, "egl error: EGL_BAD_CURRENT_SURFACE\n" );
				break;

			case EGL_BAD_DISPLAY:
				Engine::printError( tag, "egl error: EGL_BAD_DISPLAY\n" );
				break;
			case EGL_BAD_SURFACE:
				Engine::printError( tag, "egl error: EGL_BAD_SURFACE\n" );
				break;
			case EGL_BAD_MATCH:
				Engine::printError( tag, "egl error: EGL_BAD_MATCH\n" );
				break;
			case EGL_BAD_PARAMETER:
				Engine::printError( tag, "egl error: EGL_BAD_PARAMETER\n" );
				break;
			case EGL_BAD_NATIVE_PIXMAP:
				Engine::printError( tag, "egl error: EGL_BAD_NATIVE_PIXMAP\n" );
				break;
			case EGL_BAD_NATIVE_WINDOW:
				Engine::printError( tag, "egl error: EGL_BAD_NATIVE_WINDOW\n" );
				break;
			case EGL_CONTEXT_LOST:
				Engine::printError( tag, "egl error: EGL_CONTEXT_LOST\n" );
				break;
			default:
				{
					Engine::printError( tag, "egl error: %X\n", err );
				}
			}
		}
		assert( err == EGL_SUCCESS );
	}



	
	void touchEventFunc( ESContext* esContext, TouchEventType type, int touchId, int x, int y );



/**
 * Initialize an EGL context for the current display.
 */
static bool engine_init_display(ESContext* engine)
{
    // initialize OpenGL ES and EGL
	Engine::printInfo( tag, "engine_init_display enter" );
	EGLint attribList[] =
	{
		EGL_RED_SIZE,       8,
		EGL_GREEN_SIZE,     8,
		EGL_BLUE_SIZE,      8,
		EGL_ALPHA_SIZE,     (engine->windowCreateFlags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
		EGL_DEPTH_SIZE,     (engine->windowCreateFlags & ES_WINDOW_DEPTH) ? 16 : EGL_DONT_CARE,
//		EGL_STENCIL_SIZE,   (engine->windowCreateFlags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
//		EGL_SAMPLE_BUFFERS, (engine->windowCreateFlags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
		EGL_NONE
	};
   

    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglCheckError("eglGetDisplay");

    eglInitialize(display, 0, 0);
	eglCheckError("eglInitialize");

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribList, &config, 1, &numConfigs);
	eglCheckError("eglChooseConfig");

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
	eglCheckError("eglGetConfigAttrib");

    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
	eglCheckError("eglCreateWindowSurface");

	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
    context = eglCreateContext(display, config, NULL, contextAttribs);
	eglCheckError("eglCreateContext");

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) 
	{
		eglCheckError("eglMakeCurrent");
		//LOG_ERROR("Unable to eglMakeCurrent");
        return false;
    }
	eglCheckError("eglMakeCurrent");

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglCheckError("eglQuerySurface");

    eglQuerySurface(display, surface, EGL_HEIGHT, &h);
	eglCheckError("eglQuerySurface");

	
  Engine::printInfo( tag, "engine_init_display almost leave" );

	engine->eglDisplay = display;
	engine->eglContext = context;
	engine->eglSurface = surface;
    engine->width = w;
    engine->height = h;
  
    // Initialize GL state.
   // glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  Engine::printInfo( tag, "engine_init_display leave" );
    return true;
}

GLboolean esCreateWindow ( ESContext *esContext, GLint width, GLint height, GLint flags, Version version  )
//GLboolean esCreateWindow ( ESContext *esContext, const char* title, GLint width, GLint height, GLint flags, Version version )
{
	Engine::printLog( tag, "esCreateWindow" );
	assert( esContext != 0 );
	esContext->windowCreateFlags = flags;
	esContext->version = version;
}


/**
 * Just the current frame in the display.
 */
static void engine_draw_frame(ESContext* engine) 
{
	if (engine->eglDisplay == NULL)
	{
        // No display.
        return;
    }

	if( engine->drawFunc != 0 )
		engine->drawFunc(engine);

	eglSwapBuffers(engine->eglDisplay, engine->eglSurface);
	//eglCheckError("eglSwapBuffers");
}

/**
 * Tear down the EGL context currently associated with the display.
 */
static void engine_term_display(ESContext* engine) 
{
    if (engine->eglDisplay != EGL_NO_DISPLAY) 
	{
        eglMakeCurrent(engine->eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if (engine->eglContext != EGL_NO_CONTEXT) 
		{
            eglDestroyContext(engine->eglDisplay, engine->eglContext);
        }

        if (engine->eglSurface != EGL_NO_SURFACE) 
		{
            eglDestroySurface(engine->eglDisplay, engine->eglSurface);
        }

        eglTerminate(engine->eglDisplay);
    }
  
	engine->eglDisplay = EGL_NO_DISPLAY;
    engine->eglContext = EGL_NO_CONTEXT;
    engine->eglSurface = EGL_NO_SURFACE;
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) 
{
    ESContext* engine = (ESContext*)app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) 
	{
		Engine::TouchEventType eventType;
		if (AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_DOWN)
		{
			eventType = Engine::TOUCH_BEGIN;
		}
		else if (AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_UP)
		{
			eventType = Engine::TOUCH_END;
		}
		else if (AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_MOVE)
		{
			eventType = Engine::TOUCH_MOVE;
		}
		else if (AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_CANCEL)
		{
			eventType = Engine::TOUCH_CANCEL;
		}
		else 
		{
			return 0;
		}

		// A pressed gesture has started, the motion contains the initial starting location.
		for( int i=0; i<AMotionEvent_getPointerCount(event); ++i )
		{
			int pointerId = AMotionEvent_getPointerId(event,i);
			float x =  AMotionEvent_getX(event,i);
			float y =  AMotionEvent_getY(event,i);
			touchEventFunc( engine, eventType, pointerId, x, y );
		}

		return 1;
	}
	else if(AInputEvent_getType(event) == AINPUT_SOURCE_TRACKBALL)
	{
	}
	else if(AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY)
	{
	}
	else
	{
	}
  

    return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd)
{
    ESContext* engine = (ESContext*)app->userData;
    switch (cmd)
	{
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.  Do so.
  //          engine->app->savedState = malloc(sizeof(struct saved_state));
   //         *((struct saved_state*)engine->app->savedState) = engine->state;
    //        engine->app->savedStateSize = sizeof(struct saved_state);
			
			Engine::printLog( tag, "engine_handle_cmd APP_CMD_SAVE_STATE" );

            break;

        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (engine->app->window != NULL) 
			{
				Engine::printLog( tag, "engine_handle_cmd APP_CMD_INIT_WINDOW" );

				if( engine == 0 )
				{
					Engine::printLog( tag, "engine_handle_cmd APP_CMD_INIT_WINDOW Engine NULL!!" );
				}
				else
				{
					
					engine_init_display(engine);
					windowCreated = true;
				}
//                engine_draw_frame(engine);
            }
            break;

        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
			if( initDone == true )
			{
				
				Engine::printLog( tag, "engine_handle_cmd APP_CMD_TERM_WINDOW" );
				engine->deinitFunc(engine);
				initDone = false;
				shallQuit = true;
			}
			engine_term_display(engine);
            break;

        case APP_CMD_GAINED_FOCUS:
            // When our app gains focus, we start monitoring the accelerometer.
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                        engine->accelerometerSensor, (1000L/60)*1000);
				
				Engine::printLog( tag, "engine_handle_cmd APP_CMD_GAINED_FOCUS" );
            }
            break;

        case APP_CMD_LOST_FOCUS:
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
			
			Engine::printLog( tag, "engine_handle_cmd APP_CMD_LOST_FOCUS" );
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
            }
            // Also stop animating.
        //    engine->animating = 0;
            engine_draw_frame(engine);
            break;
    }

	Engine::printLog( tag, "engine_handle_cmd leave" );
}




void esInitContext_android( ESContext *esContext )
{
	Engine::printLog( tag, "esInitContext_android enter" );
	g_androidState->userData = esContext;
	esContext->app = g_androidState;
	g_androidState->onAppCmd = engine_handle_cmd;
    g_androidState->onInputEvent = engine_handle_input;

	esContext->sensorManager = ASensorManager_getInstance();
    esContext->accelerometerSensor = ASensorManager_getDefaultSensor(esContext->sensorManager, ASENSOR_TYPE_ACCELEROMETER);
    esContext->sensorEventQueue = ASensorManager_createEventQueue(esContext->sensorManager, g_androidState->looper, LOOPER_ID_USER, NULL, NULL);
}


void esMainLoop( ESContext *esContext )
{
	
	Engine::printLog( tag, "esMainLoop enter" );
	ElapsedTimer timer;
	timer.reset();
	initDone = false;
	windowCreated = false;
	while (shallQuit == false) 
	{
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // We loop until all events are read, then continue to draw the next frame of animation.
        while ((ident=ALooper_pollAll( 0, NULL, &events, (void**)&source)) >= 0) 
		{
			// Process this event.
            if (source != NULL)
			{
                source->process(g_androidState, source);
            }

            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER) 
			{
                if (esContext->accelerometerSensor != NULL)  
				{
                    ASensorEvent event;
                    while (ASensorEventQueue_getEvents(esContext->sensorEventQueue, &event, 1) > 0) 
					{
              /*          LOGI("accelerometer: x=%f y=%f z=%f",
                                event.acceleration.x, event.acceleration.y,
                                event.acceleration.z);*/
                    }
                }
            }

            // Check if we are exiting.
            if (g_androidState->destroyRequested != 0)
			{
                engine_term_display(esContext);
                return;
            }
        }

		
		float deltaTime = timer.getTime();
		timer.reset();

		//	LOG("init done");
		if( windowCreated && !initDone && esContext && esContext->initFunc )
		{
			Engine::printLog( tag, "main calling initFunc" );
			esContext->initFunc ( esContext );
			initDone = true;
		}
				
		if( initDone && shallQuit == false )
		{
			if( deltaTime > 0.0f )
			{
				esContext->updateFunc ( esContext, deltaTime );
	//			clearInput();
			}	

		   // if (engine.animating)
			{
				// Done with events; draw next animation frame.
			 /*   engine.state.angle += .01f;
				if (engine.state.angle > 1) {
					engine.state.angle = 0;
				}
				*/
				// Drawing is throttled to the screen update rate, so there
				// is no need to do timing here.
				engine_draw_frame(esContext);
			}
		}
    }




}



}



/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
extern "C" void android_main(struct android_app* state) 
{
	Engine::printLog( Engine::tag, "android_main enter" );
	g_androidState = state;
	state->userData = 0;
    /*ESContext engine;
	*/
    // Make sure glue isn't stripped.
   // app_dummy();

	//char args*[1] = { "", "" };

	main(0,0);
   // memset(&engine, 0, sizeof(engine));
   // state->userData = &engine;

   
    // Prepare to monitor accelerometer
  
   /* if (state->savedState != NULL) {
        // We are starting with a previous saved state; restore from it.
        engine.state = *(struct saved_state*)state->savedState;
    }*/

    // loop waiting for stuff to do.

   
}

//END_INCLUDE(all)
