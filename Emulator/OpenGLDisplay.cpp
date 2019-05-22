#include "pch.h"
#include "OpenGLDisplay.h"
#include <vector>


int displayHeight;
int displayWidth;
std::vector< unsigned char > display;


OpenGLDisplay::OpenGLDisplay(int height, int width)
{
	displayHeight = height;
	displayWidth = width;
	display.resize(displayHeight*displayWidth * 3);
}


OpenGLDisplay::~OpenGLDisplay()
{
	free(&display);
}


void OpenGLDisplay::setPixel(int x, int y, unsigned char R, unsigned char G, unsigned char B)
{
	unsigned int location = (x + (y * displayWidth)) * 3;
	display[location + 0] = R; // R
	display[location + 1] = G; // G
	display[location + 2] = B; // B
}


void OpenGLDisplay::bitmapToColorBuffer(const std::vector<int>& screenBuffer)
{
	for (int i = 0; i < displayHeight; i++)
	{
		for (int j = 0; j < displayWidth; j++)
		{
			int value = screenBuffer[(i*displayWidth + j )];
			unsigned char color = value * 255;
			setPixel(j, i, color, color, color);
		}
	}
}

void OpenGLDisplay::drawScreen(const std::vector<int>& screenBuffer)
{
	bitmapToColorBuffer(screenBuffer);

	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D
	(
		GL_TEXTURE_2D, 0,
		GL_RGB, displayWidth, displayHeight, 0,
		GL_RGB, GL_UNSIGNED_BYTE, &display[0]
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glEnable(GL_TEXTURE_2D);


	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(-1, -1); glVertex2f(-1, -1);
	glTexCoord2f(-1, 1); glVertex2f(-1, 1);
	glTexCoord2f(1, 1); glVertex2f(1, 1);
	glTexCoord2f(1, -1); glVertex2f(1, -1);

	glEnd();

	glFlush();
}
