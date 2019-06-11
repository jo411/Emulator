#pragma once
#include <vector>
class EmulatorModel
{
public:
	int screenWidth = 64;
	int screenHeight = 32;
	bool initialized = false;

	EmulatorModel(std::string ROMPath);
	~EmulatorModel();
	
	bool dirtyScreen = true;

	void tickTimers();
	void clearScreen();

	void tick();
	void generateDebugScreenBuffer(int val);
	std::vector<int> getScreenBuffer();
	void initializeRam();
	bool loadRomIntoRam();
	bool loadFontIntoRam();
	void setPixel(int x, int y, int val, bool useXor);
	short getCharFontAddr(unsigned char in);
	void writeSpriteToScreenBuffer(int x, int y, short addr);
	void writeRam();
};

