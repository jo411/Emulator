// Emulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "glew.h"
#include "glfw3.h"

#include "prototype.h"
unsigned int g_windowWidth = 1300;
unsigned int g_windowHeight = 640;
const char* g_windowName = "Emulator";

GLFWwindow* g_window;

prototype* tester;

float getTime()
{
	return (float)glfwGetTime();
}

void glfwErrorCallback(int error, const char* description)
{
	std::cerr << "GLFW Error " << error << ": " << description << std::endl;
	exit(1);
}

void glfwKeyCallback(GLFWwindow* p_window, int p_key, int p_scancode, int p_action, int p_mods)
{
	if (p_key == GLFW_KEY_ESCAPE && p_action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(g_window, GL_TRUE);
	}
}

void initWindow()
{
	// initialize GLFW
	glfwSetErrorCallback(glfwErrorCallback);
	if (!glfwInit())
	{
		std::cerr << "GLFW Error: Could not initialize GLFW library" << std::endl;
		exit(1);
	}

	g_window = glfwCreateWindow(g_windowWidth, g_windowHeight, g_windowName, NULL, NULL);
	if (!g_window)
	{
		glfwTerminate();
		std::cerr << "GLFW Error: Could not initialize window" << std::endl;
		exit(1);
	}
	glfwSetWindowAttrib(g_window, GLFW_RESIZABLE, false);
	// callbacks
	glfwSetKeyCallback(g_window, glfwKeyCallback);

	// Make the window's context current
	glfwMakeContextCurrent(g_window);

	// turn on VSYNC
	glfwSwapInterval(1);
}

void initGL()
{
	glClearColor(0.f, 0.f, 0.f, 1.0f);
}

void render()
{
	tester->drawScreen();
}

void logic()
{

}

void renderLoop()
{
	while (!glfwWindowShouldClose(g_window))
	{		
		logic();

		// clear buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		render();

		// Swap front and back buffers
		glfwSwapBuffers(g_window);

		// Poll for and process events
		glfwPollEvents();
	}
}

void reshapeCallback(GLFWwindow* window, int width, int height)
{
	int widthTemp, heightTemp;
	glfwGetWindowSize(window, &widthTemp, &heightTemp);
	glViewport(0, 0, widthTemp, heightTemp);
}

int main(int argc, char *argv[])
{
	tester = new prototype();

	glfwSetWindowSizeCallback(g_window, reshapeCallback);

	initWindow();
	initGL();
	renderLoop();
}
