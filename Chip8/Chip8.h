#pragma once
#include <cstdint>
#include <random>
#include <fstream>
#include <ctime> 
#include <iostream>

//Font size and address
constexpr unsigned int FONTSET_SIZE = 80;
constexpr unsigned int START_ADDRESS = 0x200;
constexpr unsigned int FONTSET_START_ADDRESS = 0x50;

//Bit math values 
constexpr unsigned int NNN = 0x0FFFu;//12 bit
constexpr unsigned int VX = 0xF00u;//Highest 4 bit you need to shift 8 to right
constexpr unsigned int VY = 0x0F0u;//Lowest 4 bit shift 4 to right 
constexpr unsigned int KK = 0x00FFu;//8 bit value
constexpr unsigned int VF = 0xF;

//video size
constexpr  unsigned int VIDEO_HEIGHT = 32;
constexpr  unsigned int VIDEO_WIDTH = 64;

//inti values for cpu ascpects
constexpr unsigned int MEMORY_SIZE = 4096;
constexpr unsigned int KEY_COUNT = 16;
constexpr unsigned int REGISTER_COUNT = 16;
constexpr unsigned int STACK_LEVELS = 16;


class Chip8 {//The Chip8 emulator 
public: 

	//guide used to make emulator https://austinmorlan.com/posts/chip8_emulator/ and https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
	//With some of my own changes 

	
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

	//taken from here https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
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


	Chip8();
	~Chip8();
	void Cycle();
	bool LoadROM(char const* filename);

private:


	//Functions for opcode merging
	inline unsigned OPCODE_MERGER_VX(unsigned opcode);
	inline unsigned OPCODE_MERGER_VY(unsigned opcode);
	inline unsigned OPCODE_MERGER_BYTE(unsigned opcode);

	//Techinal refrence >
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
	void OPCODE_5xy0();//Skip next instruction if Vx = Vy.
	void OPCODE_6xkk();//Set Vx = kk. The interpreter puts the value kk into register Vx.
	void OPCODE_7xkk();//Set Vx = Vx + kk. Adds the value kk to the value of register Vx, then stores the result in Vx. 
	void OPCODE_8xy0();//Set Vx = Vy. Stores the value of register Vy in register Vx.
	void OPCODE_8xy1();//Set Vx = Vx OR Vy.
	void OPCODE_8xy2();//Set Vx = Vx AND Vy.
	void OPCODE_8xy3();//Set Vx = Vx XOR Vy.
	void OPCODE_8xy4();//Set Vx = Vx + Vy, set VF = carry.
	void OPCODE_8xy5();//Set Vx = Vx - Vy, set VF = NOT borrow.
	void OPCODE_8xy6();//Set Vx = Vx SHR 1.
	void OPCODE_8xy7();//Set Vx = Vy - Vx, set VF = NOT borrow.
	void OPCODE_8xyE();//Set Vx = Vx SHL 1.
	void OPCODE_9xy0();//Skip next instruction if Vx != Vy.
	void OPCODE_Annn();//Set I = nnn.
	void OPCODE_Bnnn();//Jump to location nnn + V0.
	void OPCODE_Cxkk();//Set Vx = random byte AND kk.
	void OPCODE_Dxyn();//Dxyn Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
	void OPCODE_Ex9E();//Skip next instruction if key with the value of Vx is pressed.
	void OPCODE_ExA1();//Skip next instruction if key with the value of Vx is not pressed.
	void OPCODE_Fx07();//Set Vx = delay timer value.
	void OPCODE_Fx0A();//Wait for a key press, store the value of the key in Vx.
	void OPCODE_Fx15();//Set delay timer = Vx.
	void OPCODE_Fx18();//Set sound timer = Vx.
	void OPCODE_Fx1E();//Set I = I + Vx.
	void OPCODE_Fx29();//Set I = location of sprite for digit Vx.
	void OPCODE_Fx33();//Store BCD representation of Vx in memory locations I, I+1, and I+2.
	void OPCODE_Fx55();//Store registers V0 through Vx in memory starting at location I.
	void OPCODE_Fx65(); //Read registers V0 through Vx from memory starting at location I.

};