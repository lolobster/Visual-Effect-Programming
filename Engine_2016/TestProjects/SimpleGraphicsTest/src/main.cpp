// Include OpenGL ES Engine utils
#include <graphics/OpenGLES/es_util.h>
#include <core/Ref.h>
#include <core/RefCounter.h>
#include <core/Input.h>
#include <core/Globals.h>
#include <core/CommandlineArgs.h>

#include "TriangleScene.h"
#include "QuadScene.h"
#include "TestScene.h"
#include "SimpleMeshScene.h"
#include "PlimPlomScene.h"

// Current scene and max num of scenes
core::Ref<Scene> m_currentScene = 0;
static const int NUM_SCENES = 5;
static int sceneIndex = 0; // Current scene index.

// Initialize engine globals (memory manager)
static const int KILOBYTES = 1024;
static const int MEGABYTES = KILOBYTES*KILOBYTES;
core::RefCounter refs(10*MEGABYTES, 10*MEGABYTES, 100*KILOBYTES);
ENGINE_SINGLETON_CLASS_DECLARATION(core::Globals);
	

#if defined(ANDROID)
Engine::ElapsedTimer timer;
#endif

void changeCurrentScene(int index)
{
#if defined(ANDROID)
	timer.reset();
#endif
	
	refs.printAllocations();
	m_currentScene = 0;
	refs.printAllocations();
	assert( index >= 0 && index<NUM_SCENES);
	switch( index )
	{
	case 0: m_currentScene = new PlimPlomScene(); break;
	case 1: m_currentScene = new QuadScene(); break;
	case 2: m_currentScene = new TestScene(); break;
	case 3: m_currentScene = new SimpleMeshScene(); break;
	case 4: m_currentScene = new TriangleScene(); break;
	default:
		break;
	}
}


void update(graphics::ESContext* esContext, float deltaTime);


// Initialize the app
bool init(graphics::ESContext *esContext)
{
	changeCurrentScene(sceneIndex);
	update(esContext,0.0f);
	return true;
}


// Deinitialize the app
void deinit(graphics::ESContext *esContext)
{
	m_currentScene = 0;
}


// Draw app
void draw(graphics::ESContext *esContext)
{
	if( m_currentScene )
		m_currentScene->render(esContext);
}


// update app
void update(graphics::ESContext* esContext, float deltaTime)
{
#if defined(_WIN32)
	bool nowPressed = GetAsyncKeyState(VK_SPACE) ? true : false;

	static bool prevPressed = false;
	if( !nowPressed && prevPressed )
	{
		sceneIndex = (sceneIndex+1) % NUM_SCENES;
		changeCurrentScene(sceneIndex);
	}

	prevPressed = nowPressed;
#endif

#if defined(ANDROID)
	if( timer.getTime() > 10.0 )
	{
		sceneIndex = (sceneIndex+1) % NUM_SCENES;
		changeCurrentScene(sceneIndex);
	}
#endif	

	if( m_currentScene )
		m_currentScene->update(esContext,deltaTime);
}


int main ( int argc, char *argv[] )
{
	graphics::ESContext esContext;
	graphics::esInitContext(&esContext);
	graphics::esCreateWindow(&esContext, 1280, 720, graphics::ES_WINDOW_DEFAULT);
   
	graphics::esRegisterInitFunc(&esContext, init);
	graphics::esRegisterDrawFunc(&esContext, draw);
	graphics::esRegisterUpdateFunc(&esContext, update);
	graphics::esRegisterDeinitFunc(&esContext, deinit);
	
	graphics::esMainLoop(&esContext);
	return 0;
}



