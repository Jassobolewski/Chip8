#include "Chip8.h"




Chip8::Chip8():randomGenerator((unsigned int)time(nullptr))
{
	memory = new uint8_t[MEMORY_SIZE];
	stack = new uint16_t[STACK_LEVELS];
	keypad = new uint8_t[KEY_COUNT];

	opcode = 0;
	pc = START_ADDRESS;//init program counter

	// Load fonts into memory
	for (unsigned int i = 0; i < FONTSET_SIZE; ++i)
	{
		memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}

	randomByte = std::uniform_int_distribution<size_t>(0u, 255u);//RNG init
}

Chip8::~Chip8()
{
	delete[] memory;
	delete[] stack;
	delete[] keypad;
}


void Chip8::Cycle()
{
	// Fetch
	opcode = (memory[pc] << 8u) | memory[pc + 1];

	// Increment the PC before we execute anything
	pc += 2;

	// Decode and Execute
	switch (opcode & 0xF000)
	{

	case 0x0000:
		switch (opcode & 0x000F)
		{
		case 0x0000: // 0x00E0: Clears the screen      
			OPCODE_00E0();
			break;
		case 0x000E: // 0x00EE: Returns from subroutine          
			OPCODE_00EE();
			break;
		default:
			std::cout << "Unknown opcode [0x0000]\n"  << opcode;
			break;
		}
		break;
	case 0x1000:
		OPCODE_1nnn();
		break;
	case 0x2000:
		OPCODE_2nnn();
	case 0x3000:
		OPCODE_3xkk();
		break;
	case 0x4000:
		OPCODE_4xkk();
		break;
	case 0x5000:
		OPCODE_5xy0();
		break;
	case 0x6000:
		OPCODE_6xkk();
		break;
	case 0x7000:
		OPCODE_7xkk();
		break;
	case 0x8000:
		switch (opcode & 0x000F)
		{
			case 0:
				OPCODE_8xy0();
				break;
			case 1:
				OPCODE_8xy1();
				break;
			case 2:
				OPCODE_8xy2();
				break;
			case 3:
				OPCODE_8xy3();
				break;
			case 4:
				OPCODE_8xy4();
				break;
			case 5:
				OPCODE_8xy5();
				break;
			case 6:
				OPCODE_8xy6();
				break;
			case 7:
				OPCODE_8xy7();
				break;
			case 0x00E:
				OPCODE_8xyE();
				break;
			default:
				std::cout << "Unknown opcode [0x8000]\n" << opcode;
				break;
		}
		break;
	case 0x9000:
		OPCODE_9xy0();
		break;
	case 0xA000:
		OPCODE_Annn();
		break;
	case 0xB000:
		OPCODE_Bnnn();
		break;
	case 0xC000 :
		OPCODE_Cxkk();
		break;
	case 0xD000:
		OPCODE_Dxyn();
		break;
	case 0xE000:
		switch (opcode & 0x00FF)
		{
		case 0x009E:
			OPCODE_Ex9E();
			break;
		case 0x00A1:
			OPCODE_ExA1();
			break;
		default:
			std::cout << "Unknown opcode [0xE000]\n" << opcode;
			break;
		}
		break;
	case 0xF000:
		switch (opcode & 0x00FF)
		{
		case 0x0007:
			OPCODE_Fx07();
			break;
		case 0x0015:
			OPCODE_Fx15();
			break;
		case 0x0018:
			OPCODE_Fx18();
			break;
		case 0x001E:
			OPCODE_Fx1E();
			break;
		case 0x0029:
			OPCODE_Fx29();
			break;
		case 0x0033:
			OPCODE_Fx33();
			break;
		case 0x0055:
			OPCODE_Fx55();
			break;
		case 0x0065:
			OPCODE_Fx65();
			break;
		default:
			std::cout << "Unknown opcode [0xF000]:\n" << opcode;
			break;
		}
		break;
	default:
		std::cout << "Unknown opcode:\n" << opcode;
		break;
	}

	// Decrement the delay timer if it's been set
	if (delayTimer > 0)
	{
		--delayTimer;
	}

	// Decrement the sound timer if it's been set
	if (soundTimer > 0)
	{
		--soundTimer;
	}
}

bool Chip8::LoadROM(char const* filename)
{
	// Open the file as a stream of binary and move the file pointer to the end
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (file.is_open())
	{
		// Get size of file and allocate a buffer to hold the contents
		std::streampos size = file.tellg();
		char* buffer = new char[size];

		// Go back to the beginning of the file and fill the buffer
		file.seekg(0, std::ios::beg);
		file.read(buffer, size);
		file.close();

		// Load the ROM contents into the Chip8's memory, starting at 0x200
		if (sizeof(buffer) > (sizeof(memory) - START_ADDRESS))
		{
			std::cout << "Error: Rom exeeds memory limit" << std::endl;
		}
		else
		{
			for (long i = 0; i < size; i++)
			{
				memory[START_ADDRESS + i] = buffer[i];
			}
		}
		// Free the buffer
		delete[] buffer;
		return true;
	}
	else
	{
		std::cout << "Invaild file" << std::endl;
		return false;
	}

}


inline void Chip8::OPCODE_00E0()
{
	memset(video, 0, sizeof(video));
}


inline void Chip8::OPCODE_00EE()//The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
{
	--sp;
	pc = stack[sp];
}

inline void Chip8::OPCODE_1nnn()//The interpreter sets the program counter to nnn (4096 byte)
{
	//std::cout << "1NNN" << std::endl;
	pc = opcode & NNN;
}

inline void Chip8::OPCODE_2nnn()//The interpreter increments the stack pointer, then puts the current PC on the top of the stack.The PC is then set to nnn.
{
	//std::cout << "2NNN" << std::endl;
	stack[sp] = pc;
	++sp;
	pc = opcode & NNN;
}

inline void Chip8::OPCODE_3xkk()
{
	//std::cout << "3KKK" << std::endl;
	uint8_t Vx = (opcode & VX) >> 8u;
	uint8_t byte = opcode & KK;

	if (registers[Vx] == byte)
	{
		pc += 2;
	}

}

inline void Chip8::OPCODE_4xkk()
{
	uint8_t Vx = (opcode & VX) >> 8u;
	uint8_t byte = (opcode & KK);

	if (registers[Vx] != byte)
	{
		pc += 2;
	}

}

inline void Chip8::OPCODE_5xy0()
{
	uint8_t Vx = (opcode & VX) >> 8u;
	uint8_t Vy = (opcode & VY) >> 4u;
	if (registers[Vx] == registers[Vy])
	{
		pc += 2;
	}

}

inline void Chip8::OPCODE_6xkk()
{
	uint8_t Vx = (opcode & VX) >> 8u;
	registers[Vx] = (opcode & KK);
}

inline void Chip8::OPCODE_7xkk()
{
	uint8_t Vx = (opcode & VX) >> 8u;
	uint8_t byte = (opcode & KK);
	registers[Vx] += byte;
}

inline void Chip8::OPCODE_8xy0()
{
	uint8_t Vx = (opcode & VX) >> 8u;
	uint8_t Vy = (opcode & VY) >> 4u;
	registers[Vx] = registers[Vy];
}

inline void Chip8::OPCODE_8xy1()
{
	uint8_t Vx = (opcode & VX) >> 8u;
	uint8_t Vy = (opcode & VY) >> 4u;
	registers[Vx] = (registers[Vx] | registers[Vy]);
}

inline void Chip8::OPCODE_8xy2()
{
	uint8_t Vx = (opcode & VX) >> 8u;
	uint8_t Vy = (opcode & VY) >> 4u;
	registers[Vx] = (registers[Vx] & registers[Vy]);
}

inline void Chip8::OPCODE_8xy3()
{
	uint8_t Vx = (opcode & VX) >> 8u;
	uint8_t Vy = (opcode & VY) >> 4u;
	registers[Vx] = (registers[Vx] ^ registers[Vy]);
}

inline void Chip8::OPCODE_8xy4()
{
	uint8_t Vx = (opcode & VX) >> 8u;
	uint8_t Vy = (opcode & VY) >> 4u;
	auto sum = registers[Vx] + registers[Vy];

	if (sum > 255)
	{
		registers[VF] = 1;
	}
	else
	{
		registers[VF] = 0;
	}

	registers[Vx] = sum & KK;//0xFFu
}

inline void Chip8::OPCODE_8xy5()
{
	uint8_t Vx = (opcode & VX) >> 8u;
	uint8_t Vy = (opcode & VY) >> 4u;
	auto sum = registers[Vx] + registers[Vy];
	
	if (registers[Vx] > registers[Vy])
	{
		registers[VF] = 1;
	}
	else
	{
		registers[VF] = 0;
	}

	registers[Vx] -= registers[Vy];
}

inline void Chip8::OPCODE_8xy6()
{
	uint8_t Vx = (opcode & VX) >> 8u;
	registers[VF] = (registers[Vx] & 0x1u);

	registers[Vx] >>= 1;

}

inline void Chip8::OPCODE_8xy7()
{
	uint8_t Vx = (opcode & VX) >> 8u;
	uint8_t Vy = (opcode & VY) >> 4u;

	if (registers[Vy] > registers[Vx])
	{
		registers[VF] = 1;
	}
	else
	{
		registers[VF] = 0;
	}

	registers[Vx] = registers[Vy] - registers[Vx];

}

inline void Chip8::OPCODE_8xyE()
{
	uint8_t Vx = (opcode & VX) >> 8u;
	registers[VF] = (registers[Vx] & 0x80u) >> 7u;

	registers[Vx] <<= 1;

}

inline void Chip8::OPCODE_9xy0()
{
	uint8_t Vx = (opcode & VX) >> 8u;
	uint8_t Vy = (opcode & VY) >> 4u;

	if (registers[Vx] != registers[Vy])
	{
		pc += 2;
	}

}

inline void Chip8::OPCODE_Annn()
{
	index = (opcode & NNN);
}

inline void Chip8::OPCODE_Bnnn()
{
	pc = (registers[0] + (opcode & NNN));
}

inline void Chip8::OPCODE_Cxkk()
{
	uint8_t Vx = (opcode & VX) >> 8u;
	uint8_t byte = (opcode & KK);
	registers[Vx] = (uint8_t)(randomByte(randomGenerator) & byte);
}

void Chip8::OPCODE_Dxyn()
{
	uint8_t Vx = (opcode & VX) >> 8u;
	uint8_t Vy = (opcode & VY) >> 4u;
	uint8_t height = opcode & 0x000Fu;

	// Wrap if going beyond screen boundaries
	uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
	uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

	registers[VF] = 0;

	for (unsigned int row = 0; row < height; ++row)
	{
		uint8_t spriteByte = memory[index + row];

		for (unsigned int col = 0; col < 8; ++col)
		{
			uint8_t spritePixel = spriteByte & (0x80u >> col);
			uint32_t* screenPixel = &video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

			// Sprite pixel is on
			if (spritePixel)
			{
				// Screen pixel also on - collision
				if (*screenPixel == 0xFFFFFFFF)
				{
					registers[VF] = 1;
				}

				// Effectively XOR with the sprite pixel
				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}

}

inline void Chip8::OPCODE_Ex9E()
{
	uint8_t Vx = (opcode & VX) >> 8u;

	uint8_t key = registers[Vx];

	if (keypad[key])
	{
		pc += 2;
	}

}

inline void Chip8::OPCODE_ExA1()
{
	uint8_t Vx = (opcode & VX) >> 8u;

	uint8_t key = registers[Vx];

	if (!keypad[key])
	{
		pc += 2;
	}

}

inline void Chip8::OPCODE_Fx07()
{
	uint8_t Vx = (opcode & VX) >> 8u;

	registers[Vx] = delayTimer;

}

void Chip8::OPCODE_Fx0A()
{
	uint8_t Vx = (opcode & VX) >> 8u;

	if (keypad[0])
	{
		registers[Vx] = 0;
	}
	else if (keypad[1])
	{
		registers[Vx] = 1;
	}
	else if (keypad[2])
	{
		registers[Vx] = 2;
	}
	else if (keypad[3])
	{
		registers[Vx] = 3;
	}
	else if (keypad[4])
	{
		registers[Vx] = 4;
	}
	else if (keypad[5])
	{
		registers[Vx] = 5;
	}
	else if (keypad[6])
	{
		registers[Vx] = 6;
	}
	else if (keypad[7])
	{
		registers[Vx] = 7;
	}
	else if (keypad[8])
	{
		registers[Vx] = 8;
	}
	else if (keypad[9])
	{
		registers[Vx] = 9;
	}
	else if (keypad[10])
	{
		registers[Vx] = 10;
	}
	else if (keypad[11])
	{
		registers[Vx] = 11;
	}
	else if (keypad[12])
	{
		registers[Vx] = 12;
	}
	else if (keypad[13])
	{
		registers[Vx] = 13;
	}
	else if (keypad[14])
	{
		registers[Vx] = 14;
	}
	else if (keypad[15])
	{
		registers[Vx] = 15;
	}
	else
	{
		pc -= 2;
	}

}

inline void Chip8::OPCODE_Fx15()
{
	uint8_t Vx = (opcode & VX) >> 8u;

	delayTimer = registers[Vx];

}

inline void Chip8::OPCODE_Fx18()
{
	uint8_t Vx = (opcode & VX) >> 8u;

	soundTimer = registers[Vx];

}

inline void Chip8::OPCODE_Fx1E()
{
	uint8_t Vx = (opcode & VX) >> 8u;

	index += registers[Vx];

}

inline void Chip8::OPCODE_Fx29()
{

	uint8_t Vx = (opcode & VX) >> 8u;
	uint8_t digit = registers[Vx];

	index = FONTSET_START_ADDRESS + (5 * digit);

}

inline void Chip8::OPCODE_Fx33()
{
	uint8_t Vx = (opcode & VX) >> 8u;
	uint8_t value = registers[Vx];

	// Ones-place
	memory[index + 2] = value % 10;
	// Tens-place
	memory[index + 1] = (value / 10) % 10;
	// Hundreds-place
	memory[index] = (value / 100) % 10;

}

inline void Chip8::OPCODE_Fx55()
{
	uint8_t Vx = (opcode & VX) >> 8u;

	for (uint8_t i = 0; i <= Vx; ++i)
	{
		memory[index + i] = registers[i];
	}

}

inline void Chip8::OPCODE_Fx65()
{
	uint8_t Vx = (opcode & VX) >> 8u;

	for (uint8_t i = 0; i <= Vx; ++i)
	{
		registers[i] = memory[index + i];
	}

}
