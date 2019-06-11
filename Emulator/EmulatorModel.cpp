#include "pch.h"
#include "EmulatorModel.h"
#include <vector>
#include <fstream>
#include <ostream>
#include <sys/stat.h>
#include <iostream>
#include <string>
//File Variables
std::string romName;
std::string basePath = "../Emulator/Data/";

//RAM Variables
const int RAM_MAX = 4096;//max ram of 4kb 
const int dataStartAddr = 0x200;//Pointer to the first address for data
unsigned char memory[RAM_MAX];//4kb RAM bank

//Register Variables
const int REGISTER_MAX=16;
unsigned char vRegisters[REGISTER_MAX];//holds 16 8 bit registers v(0-F)
short iRegister;//16 bit I register 
unsigned char delayRegister;//8 bit delay timer register
unsigned char soundRegister;//8 bit sound timer register

short PC;//16 bit program counter
unsigned char SP;//8 bit stack pointer

//Stack Variables
const int STACK_DEPTH = 16;
short stack[STACK_DEPTH];// STACK of 16 16 bit values

//Display Variables
//--------------
std::vector<int> screen;//Screen buffer 
//THIS BUFFER WILL BE PULLED FROM RAM AT A LATER POINT
//-----------------

int screenLogicBit = 0; //super temp for basic animation logic
int updateCounter = 0;
int currentDisplay=0;
//Font Data
std::string fontPath="Font/font.bin";

EmulatorModel::EmulatorModel(std::string romNameIn)
{
	romName = romNameIn;
	screen.resize(screenHeight*screenWidth);
	initialized=loadRomIntoRam();
	initialized = loadFontIntoRam();
	writeRam();
}


EmulatorModel::~EmulatorModel()
{
}

//Main logic of the chip. THis function is called at the frequency described in the controller
void EmulatorModel::tick()
{
	updateCounter++;
	
	if (delayRegister == 0)
	{
		delayRegister = 60;
		clearScreen();
		currentDisplay++;
		currentDisplay %= 4;
		dirtyScreen = true;
		generateDebugScreenBuffer((currentDisplay));
	}

	
}

//is called at 60hz
void EmulatorModel::tickTimers()
{
	delayRegister--;
	soundRegister--;

	delayRegister = fmax(delayRegister, 0);
	soundRegister = fmax(soundRegister, 0);
}


//Zero out RAM
void EmulatorModel::initializeRam()
{
	for (int i = 0; i < RAM_MAX; i++)
	{
		memory[i] = 0;
	}
}

//Reads this object's specified ROM into RAM at the correct address
bool EmulatorModel::loadRomIntoRam()
{
	std::string fullName = basePath+ "/ROMS/" + romName;

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

bool EmulatorModel::loadFontIntoRam()
{
	std::string fullName = basePath + fontPath;

	struct stat results;
	if (stat(fullName.c_str(), &results) == 0)
	{
		std::cout << "Font File Opened | Size: " << results.st_size << "\n";
	}
	else
	{
		std::cout << "File open error. No font.bin found.\n " << fullName;
		return false;//File read error
	}

	std::ifstream myFile(fullName, std::ios::in | std::ios::binary);
	bool status = true;	

	myFile.read((char*)memory, results.st_size);//read all the bytes from ROM to RAM
	if (!myFile) {
		std::cout << "File read error. Only read: " << myFile.gcount() << " bytes.\n";
		status = false;
	}
	myFile.close();


	return status;
}
//returns the address in RAM where sprite for input char is located.
short EmulatorModel::getCharFontAddr(unsigned char in)
{
	if (in > 0xF || in < 0x0)
	{
		return 0;
	}

	return (short)(5*in)*sizeof(unsigned char);
}

void EmulatorModel::writeSpriteToScreenBuffer(int x, int y, short addr)
{
	
	for (int i = 0; i < 5; i++)
	{
		unsigned char byte = memory[addr + i*sizeof(unsigned char)];
		for (int j = 7; j >=0; j--)
		{
			int index = ((y+i) * screenWidth + ((x+(7-j))));
			screen[index] = screen[index] xor ((byte >> j) & 1);
		}
	}
}

void EmulatorModel::setPixel(int x, int y, int val, bool useXor)
{
	if (useXor)
	{
		screen[y*screenWidth + x] = screen[y*screenWidth + x]xor val;
	}
	else
	{
		screen[y*screenWidth + x] = val;
	}
	
}
void EmulatorModel::clearScreen()
{
	for (int i = 0; i < screenHeight; i++)
	{
		for (int j = 0; j < screenWidth; j++)
		{
			setPixel(j, i, 0,false);
		}
	}
}
//simple function for display testing WILL NOT WORK WITH A RUNNING ROM
void EmulatorModel::generateDebugScreenBuffer(int val) {

	if (val == 0)
	{
		setPixel(0, 0, 1,false);

		setPixel(0, screenHeight - 1, 1, false);
		setPixel(0, screenHeight / 2, 1, false);

		setPixel(screenWidth - 1, screenHeight - 1, 1, false);
		setPixel(screenWidth / 2, screenHeight / 2, 1, false);

		setPixel(screenWidth - 1, 0, 1, false);
		setPixel(screenWidth / 2, 0, 1, false);

		setPixel(screenWidth / 2, screenHeight - 1, 1, false);
		setPixel(screenWidth - 1, screenHeight / 2, 1, false);
	}
	else if (val == 1)
	{
		for (int i = 0; i < 0x9; i++)
		{
			writeSpriteToScreenBuffer(5 * i, 0, getCharFontAddr((unsigned char)i));
		}
		for (int i = 0xc; i <= 0xf; i++)
		{
			writeSpriteToScreenBuffer(5 * (i - 0xc), 7, getCharFontAddr((unsigned char)i));
		}
		
	}
	else if (val == 2)
	{
		for (int i = 0; i < screenHeight; i++)
		{
			for (int j = 0; j < screenWidth; j++)
			{
				if (j == screenWidth - 1 || i == screenHeight - 1 || j == 0 || i == 0)
					screen[i*screenWidth + j] = screen[i*screenWidth + j] xor 1;
			}
		}

	}
	else if (val == 3)
	{

		for (int i = 0; i < screenHeight; i++)
		{
			for (int j = 0; j < screenWidth; j++)
			{				
				setPixel(j, i, (j % 2 == screenLogicBit + i % 2 == screenLogicBit), true);
			}
		}

		screenLogicBit++;
		screenLogicBit %= 2;
	}
}

//provide access to the screen buffer
std::vector<int>  EmulatorModel::getScreenBuffer()
{
	return screen;
}

//Writes the current state of ram to a binary file. 
void EmulatorModel::writeRam()
{
	std::ofstream file("RAM.bin", std::ios::out);
		
	file.write((char*)memory, RAM_MAX);
}