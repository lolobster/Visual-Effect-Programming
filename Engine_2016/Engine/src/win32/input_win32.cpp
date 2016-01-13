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

#include <core\Input.h>
#include <Windows.h>

#include <es_assert.h>
#include <graphics\OpenGLES\es_util.h>

namespace core
{


// anonymous namespace for internal functions
namespace
{
	class InputWin32
	{
	public:
		bool clicks[3];
		int mouseXValue;
		int mouseYValue;
		int mouseWheelDelta;

		InputWin32()
		{
			mouseXValue = 0;
			mouseYValue = 0;
			mouseWheelDelta = 0;
		}

		eastl::vector< Touch > touches;
	};

	InputWin32* input = 0;

}


void initInput()
{
	input = new InputWin32();
}

void unititInput()
{
	delete input;
	input = 0;
}

void clearInput()
{
	assert(input != 0);
	input->mouseWheelDelta = 0;
}

void mouseWheel(int mouseWheel)
{
	assert(input != 0);
	input->mouseWheelDelta += mouseWheel;
}

void mouseState(bool leftClicked, bool rightClicked, bool middleClicked, int mouseX, int mouseY )
{
	assert(input != 0);
	input->clicks[0] = leftClicked;
	input->clicks[1] = rightClicked;
	input->clicks[2] = middleClicked;
	input->mouseXValue = mouseX;
	input->mouseYValue = mouseY;
}

void touchEventFunc( graphics::ESContext * esContext, core::TouchEventType type, int touchId, int x, int y )
{
	assert(input != 0);
	if( input->touches.size() <= size_t(touchId) )
	{
		input->touches.resize(touchId+1);
	}

	input->touches[touchId].touchId = touchId;
	input->touches[touchId].x = x;
	input->touches[touchId].y = y;
	
	if( type == TOUCH_BEGIN || type == TOUCH_MOVE )
	{
		input->touches[touchId].pressed = true;
	}
	else
	{
		input->touches[touchId].pressed = false;
	}

	if( esContext != 0 && esContext->touchEventFunc != 0 )
	{
		esContext->touchEventFunc(esContext,type,touchId,x,y);
	}
}

bool getMouseButtonState(MouseButtons button)
{
	assert(input != 0);
	return input->clicks[button];
}


int getMouseAxisX()
{
	assert(input != 0);
	return input->mouseXValue;
}

int getMouseAxisY()
{
	assert(input != 0);
	return input->mouseYValue;
}

int getMouseWheelDelta()
{
	assert(input != 0);
	return input->mouseWheelDelta;
}

int getKeyState(KeyCodes keyCode)
{
	return (GetAsyncKeyState(keyCode) & 0x8000) ? 1 : 0;
}


const eastl::vector<Touch>& getActiveTouches()
{
	assert(input != 0);
	return input->touches;
}


}


