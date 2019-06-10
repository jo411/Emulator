#include "pch.h"
#include "EmulatorModel.h"
#include <vector>


std::vector<int> screen;
int screenLogicBit = 0; //super temp for basic animation logic
const char* ROMPath;

EmulatorModel::EmulatorModel(const char* ROMPathIn)
{
	ROMPath = ROMPathIn;
	screen.resize(emulatedScreenHeight*emulatedScreenWidth);
}


EmulatorModel::~EmulatorModel()
{
}

void EmulatorModel::tick()
{
	generateScreenBuffer();
}

void EmulatorModel::generateScreenBuffer() {

	for (int i = 0; i < emulatedScreenHeight; i++)
	{
		for (int j = 0; j < emulatedScreenWidth; j++)
		{
			screen[i*emulatedScreenWidth + j] = (j % 2 == screenLogicBit + i % 2 == screenLogicBit);
		}
	}

	screenLogicBit++;
	screenLogicBit %= 2;
}

std::vector<int>  EmulatorModel::getScreenBuffer()
{
	return screen;
}