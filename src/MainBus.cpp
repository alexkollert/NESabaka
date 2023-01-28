#include <MainBus.hpp>

MainBus::MainBus()
{
	for (auto& i : ram) i = 0x00;

	cpu6502.connectMainbus(this);
}

uint8_t MainBus::read(uint16_t address)
{
	if (address >= 0x0000 && address <= 0xFFFF)
		return ram[address];

	return 0;
}

void MainBus::write(uint16_t address, uint8_t data)
{
	ram[address] = data;
}