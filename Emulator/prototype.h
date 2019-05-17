#pragma once
#include <iostream>
#include "glew.h"
#include "glfw3.h"
class prototype
{
public:
	prototype();
	~prototype();	
	void drawScreen();
	void setPixel(int X, int Y, unsigned char R, unsigned char G, unsigned char B);
	void generateTexture();
private:
	
};

