#ifndef MAINBUS_HPP
#define MAINBUS_HPP
#include <cstdint>
#include <array>
#include "CPU6502.hpp"
#include <Cartridge.hpp>

#define RAM_SIZE 2048

class MainBus
{
public:
	MainBus();

	uint8_t read(uint16_t address);

	void write(uint16_t address, uint8_t data);

	void insertCartridge(const std::string filename);

	std::array<uint8_t, RAM_SIZE> ram;
	CPU6502 cpu6502;
	Cartridge cartridge;
};
#endif 