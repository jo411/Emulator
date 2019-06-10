#pragma once
#include <vector>
class EmulatorModel
{
public:
	int emulatedScreenWidth = 64;
	int emulatedScreenHeight = 32;
	bool initialized = false;

	EmulatorModel(std::string ROMPath);
	~EmulatorModel();

	void tick();
	void generateScreenBuffer();
	std::vector<int> getScreenBuffer();
	void initializeRam();
	bool loadROMIntoRAM();
	void writeRam();
};

