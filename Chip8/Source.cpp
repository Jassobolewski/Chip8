#include <chrono>
#include <iostream>
#include "Platform.h"
#include "Chip8.h"
#include <string> 



int main(int argc, char** argv)
{
	//most code insipried by https://austinmorlan.com/posts/chip8_emulator/ with some changes by me
	if (argc != 4)
	{
		std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n";
		std::exit(EXIT_FAILURE);
	}

	int videoScale = std::stoi(argv[1]);
	int cycleDelay = std::stoi(argv[2]);
	std::string* romFilename = new std::string(argv[3]);
	std::string* windowName = new std::string("Chip 8 - Emulator - Current Rom: ");
	windowName->append(romFilename->c_str());

	Platform platform(windowName->c_str(), VIDEO_WIDTH * videoScale, VIDEO_HEIGHT * videoScale, VIDEO_WIDTH, VIDEO_HEIGHT);

	Chip8 chip8;
	if (chip8.LoadROM(romFilename->c_str()))
	{
		int videoPitch = sizeof(chip8.video[0]) * VIDEO_WIDTH;

		auto lastCycleTime = std::chrono::high_resolution_clock::now();
		bool quit = false;

		while (!quit)
		{
			quit = platform.ProcessInput(chip8.keypad);

			auto currentTime = std::chrono::high_resolution_clock::now();
			float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

			if (dt > cycleDelay)
			{
				lastCycleTime = currentTime;

				chip8.Cycle();

				platform.Update(chip8.video, videoPitch);
			}
		}
	}
	
	delete romFilename;
	delete windowName;
	return 0;
}