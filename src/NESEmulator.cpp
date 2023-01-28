#include <NESEmulator.hpp>
#include <MainBus.hpp>
#include <sstream>


void UnimplementedInstruction()
{
	throw std::exception("Unimplemented or unknown instruction");
}

char* getCmdOption(char** begin, char** end, const std::string& option)
{
  char** itr = std::find(begin, end, option);
  if (itr != end && ++itr != end)
  {
    return *itr;
  }
  return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
  return std::find(begin, end, option) != end;
}

void print_disassembly(const std::string& filename)
{
  
}

NESEmulator::NESEmulator()
{
}

void NESEmulator::run(std::string rom_path)
{

}



int main()
{
    MainBus nes;

	// Load Program (assembled at https://www.masswerk.at/6502/assembler.html)
    /*
      *=$8000
	  LDX #10
	  STX $0000
	  LDX #3
	  STX $0001
	  LDY $0000
	  LDA #0
	  CLC
	  loop
	  ADC $0001
	  DEY
	  BNE loop
	  STA $0002
	  NOP
	  NOP
	  NOP
    */
	std::stringstream ss;
	ss << "A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA";
	uint16_t offset = 0x8000;

	while(!ss.eof())
	{
		std::string b;
		ss >> b;
		nes.ram[offset++] = std::stoul(b, nullptr, 16);
	}

	nes.ram[0xFFFC] = 0x00;
	nes.ram[0xFFFD] = 0x80;

	nes.cpu6502.reset();
}
