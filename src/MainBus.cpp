#include <MainBus.hpp>

MainBus::MainBus()
{
	for (auto& i : ram) i = 0x00;

	cpu6502.connectMainbus(this);
	cartridge.connectMainbus(this);


}

uint8_t MainBus::read(uint16_t address)
{
	if (address >= 0x0000 && address <= 0x1FFF) // 2K ram + mirrors
		return ram[address & 0x800];
	else if (address >= 0x4020 && address <= 0xFFFF) //Cartridge space
		return cartridge.prg_rom[(address - 0x8000) % 0x3FFF];

	return 0;
}

void MainBus::write(uint16_t address, uint8_t data)
{
	ram[address] = data;
}

void MainBus::insertCartridge(const std::string filename)
{
	struct Header_iNes
	{
		int constant;
		uint8_t prgRom_size;
		uint8_t chr_size;
		uint8_t flags6;
		uint8_t flags7;
		uint8_t flags8;
		uint8_t flags9;
		uint8_t flags10;
		uint8_t pad1;
		uint8_t pad2;
		uint8_t pad3;
		uint8_t pad4;
		uint8_t pad5;
	} header_ines;

	char buffer[0xF000];
	std::ifstream file;
	file.open("test_binaries\\" + filename, std::ios::in | std::ios::binary);
	if (file.is_open())
	{
		file.read((char*)&header_ines, sizeof(header_ines));
		cartridge.prg_rom_size = (int)header_ines.prgRom_size * 16384;
		if (cartridge.prg_rom_size > 16384)
			cartridge.isNROM256 = true;

		file.read(buffer, cartridge.prg_rom_size);
	}
	else
	{
		//error opening file
		return;
	}

	uint8_t mapper = (header_ines.flags6 >> 4) | (header_ines.flags7 & 0xF0);
	switch (mapper)
	{
	case 0:
		cartridge.prg_rom.assign(buffer, buffer + cartridge.prg_rom_size);
		break;
	}
}