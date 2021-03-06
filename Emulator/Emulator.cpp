// Emulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "glew.h"
#include "glfw3.h"
#include "OpenGLDisplay.h"
#include "EmulatorModel.h"
#include <string>
unsigned int g_windowWidth = 1300;
unsigned int g_windowHeight = 640;



const char* g_windowName = "Emulator";

GLFWwindow* g_window;

OpenGLDisplay* display;

EmulatorModel* emu;


float clockSpeed = 500;//logic update rate in hz
float clockTimer = 0;

float timerRate = 60;//rate that timers have to tick at

					 
//Probably shouldn't use a set fps, rather update anytime the screen is changed

float timerTimer = 0;

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



void render(float deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	display->drawScreen(emu->getScreenBuffer());
	glfwSwapBuffers(g_window);
}
void logic(float deltaTime)
{	
	emu->tick();
	if (emu->dirtyScreen)
	{
		render(deltaTime);
	}
}

void renderLoop()
{
	float lastTime = getTime();
	float now = lastTime + 1;
	float deltaTime = 0;

	while (!glfwWindowShouldClose(g_window))
	{			
		now = getTime();
		deltaTime = now - lastTime;
		lastTime = now;

		clockTimer += deltaTime;
		timerTimer += deltaTime;

		if (clockTimer >= clockSpeed)
		{
			clockTimer = 0;
			logic(deltaTime);
		}	
		
		if (timerTimer >= timerRate)
		{
			timerTimer = 0;
			emu->tickTimers();
		}
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
	std::string romName = "BLINKY";
	emu = new EmulatorModel(romName);
	if (!emu->initialized)
	{
		std::cout << "Initialization Failed: Exiting.....";
		return 1;
	}

	display = new OpenGLDisplay(emu->screenHeight, emu->screenWidth);	
	glfwSetWindowSizeCallback(g_window, reshapeCallback);

	clockSpeed = 1 / clockSpeed;//convert from hz to seconds
	timerRate = 1 / timerRate;//convert from hz to seconds

	//initializations
	initWindow();
	initGL();
	renderLoop();//begin render loop
}
