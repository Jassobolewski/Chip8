#pragma once
#include <cstdint>
#include <random>
#include <fstream>
#include <ctime> 
#include <iostream>

constexpr  unsigned int FONTSET_SIZE = 80;
constexpr  unsigned int START_ADDRESS = 0x200;
constexpr  unsigned int FONTSET_START_ADDRESS = 0x50;
constexpr  unsigned int NNN = 0x0FFFu;//12 bit
constexpr  unsigned int VX = 0xF00u;//Highest 4 bit you need to shift 8 to right
constexpr  unsigned int VY = 0x0F0u;//Lowest 4 bit shift 4 to right 
constexpr  unsigned int KK = 0x00FFu;//8 bit value
constexpr  unsigned int VF = 0xF;
constexpr  unsigned int VIDEO_HEIGHT = 32;
constexpr  unsigned int VIDEO_WIDTH = 64;

constexpr unsigned int MEMORY_SIZE = 4096;
constexpr unsigned int KEY_COUNT = 16;
constexpr unsigned int REGISTER_COUNT = 16;
constexpr unsigned int STACK_LEVELS = 16;


class Chip8 {
public: 

	uint8_t registers[16]{};//16 registers 
	uint8_t* memory;//8 bit memory
	uint16_t index{};//index register
	uint16_t pc{};//Program counter
	uint16_t* stack;//CPU stack
	uint8_t sp{};// 8 bit stack pointer
	uint8_t delayTimer{};//60 hz delay timer
	uint8_t soundTimer{};//sound
	uint8_t* keypad;//16 input keys
	uint32_t video[64 * 32]{};//monochrome video display
	uint16_t opcode;//Current Cpu instruction (op code)


	uint8_t fontset[FONTSET_SIZE] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	std::mt19937 randomGenerator;
	std::uniform_int_distribution<size_t> randomByte;


	//http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#00EE
	/*
	nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
	n or nibble - A 4-bit value, the lowest 4 bits of the instruction
	x - A 4-bit value, the lower 4 bits of the high byte of the instruction
	y - A 4-bit value, the upper 4 bits of the low byte of the instruction
	kk or byte - An 8-bit value, the lowest 8 bits of the instruction
	*/

	//36 Instructions 

	void OPCODE_00E0();//Clear the display.
	void OPCODE_00EE();//Return from a subroutine.
	void OPCODE_1nnn();//Jump to location nnn.
	void OPCODE_2nnn(); //Call subroutine at nnn.
	void OPCODE_3xkk(); //Skip next instruction if Vx = kk.
	void OPCODE_4xkk();//Skip next instruction if Vx != kk.
	void OPCODE_5xy0();
	void OPCODE_6xkk();
	void OPCODE_7xkk();
	void OPCODE_8xy0();
	void OPCODE_8xy1();
	void OPCODE_8xy2();
	void OPCODE_8xy3();
	void OPCODE_8xy4();
	void OPCODE_8xy5();
	void OPCODE_8xy6();
	void OPCODE_8xy7();
	void OPCODE_8xyE();
	void OPCODE_9xy0();
	void OPCODE_Annn();
	void OPCODE_Bnnn();
	void OPCODE_Cxkk();
	void OPCODE_Dxyn();
	void OPCODE_Ex9E();
	void OPCODE_ExA1();
	void OPCODE_Fx07();
	void OPCODE_Fx0A();
	void OPCODE_Fx15();
	void OPCODE_Fx18();
	void OPCODE_Fx1E();
	void OPCODE_Fx29();
	void OPCODE_Fx33();//Store BCD representation of Vx in memory locations I, I+1, and I+2.
	void OPCODE_Fx55();//Store registers V0 through Vx in memory starting at location I.
	void OPCODE_Fx65(); //Read registers V0 through Vx from memory starting at location I.
	Chip8();
	~Chip8();
	void Cycle();
	bool LoadROM(char const* filename);
};