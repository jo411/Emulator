#pragma once
#include <vector>
class EmulatorModel
{
public:
	int emulatedScreenWidth = 64;
	int emulatedScreenHeight = 32;

	EmulatorModel(const char* ROMPath);
	~EmulatorModel();

	void tick();
	void generateScreenBuffer();
	std::vector<int> getScreenBuffer();

};

