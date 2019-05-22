#pragma once
#include "glew.h"
#include "glfw3.h"
#include <vector>
class OpenGLDisplay
{
public:
	OpenGLDisplay(int height, int width);
	~OpenGLDisplay();

	void drawScreen(const std::vector<int>& screenBuffer);
	void setPixel(int X, int Y, unsigned char R, unsigned char G, unsigned char B);	
	void bitmapToColorBuffer(const std::vector<int>& screenBuffer);
};

