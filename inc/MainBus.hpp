#ifndef MAINBUS_HPP
#define MAINBUS_HPP
#include <cstdint>
#include <array>
#include "CPU6502.hpp"


class MainBus
{
public:
	MainBus();

	uint8_t read(uint16_t address);

	void write(uint16_t address, uint8_t data);

	std::array<uint8_t, 64 * 1024> ram;
	CPU6502 cpu6502;
};
#endif 