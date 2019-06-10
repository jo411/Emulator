#include "pch.h"
#include "EmulatorModel.h"
#include <vector>
#include <fstream>
#include <ostream>
#include <sys/stat.h>
#include <iostream>
#include <string>

std::string romName;
std::string basePath = "../Emulator/Data/";

const int RAM_MAX = 4096;//max ram of 4kb 
const int dataStartAddr = 0x200;
unsigned char memory[RAM_MAX];//4kb ram bank

std::vector<int> screen;
int screenLogicBit = 0; //super temp for basic animation logic


EmulatorModel::EmulatorModel(std::string romNameIn)
{
	romName = romNameIn;
	screen.resize(emulatedScreenHeight*emulatedScreenWidth);
	initialized=loadROMIntoRAM();

}


EmulatorModel::~EmulatorModel()
{
}

void EmulatorModel::tick()
{
	generateScreenBuffer();
}

void EmulatorModel::initializeRam()
{
	for (int i = 0; i < RAM_MAX; i++)
	{
		memory[i] = 0;
	}
}
bool EmulatorModel::loadROMIntoRAM()
{
	std::string fullName = basePath + romName;

	struct stat results;
	if (stat(fullName.c_str(), &results) == 0)
	{
		std::cout << "File Opened | Size: " << results.st_size<<"\n";
	}
	else
	{
		std::cout << "File open error. This file may not exist.\n "<<fullName;
		return false;//File read error
	}
	if (results.st_size > (RAM_MAX - dataStartAddr))
	{
		std::cout << "Loaded ROM exceeds memory maximum\n";
		return false;
	}

	std::ifstream myFile(fullName,std::ios::in | std::ios::binary);
	bool status = true;
	unsigned char* romStartAddr = memory + dataStartAddr;

	myFile.read((char*)romStartAddr,results.st_size);//read all the bytes from ROM to RAM
	if (!myFile) {
		std::cout << "File read error. Only read: " << myFile.gcount()<< " bytes.\n";
		status = false;
	}
	myFile.close();
	

	return status;
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

void EmulatorModel::writeRam()
{
	std::ofstream file("RAM.bin", std::ios::out);
		
	file.write((char*)memory, RAM_MAX);
}