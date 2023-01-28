#include "CPU6502.hpp"
#include "MainBus.hpp"

void CPU6502::reset()
{
	a = 0;
	x = 0;
	y = 0;
	p = {}; //TODO
	sp = 0xFD;

	addr_abs = 0xFFFC;
	uint16_t lo = read(addr_abs);
	uint16_t hi = read(addr_abs + 1);
	pc = (hi << 8) | lo;

	fetched = 0;
	addr_abs = 0x0000;
	addr_rel = 0x00;
	opcode = 0x00;
	cycles = 0;

	cycles = 8;
}

void CPU6502::clock()
{
	if (cycles == 0)
	{
		opcode = read(pc++);
		
		cycles = instructions[opcode].cycles;

		uint8_t cycle_am = (this->*instructions[opcode].addrmode)();

		uint8_t cycle_op = (this->*instructions[opcode].f)();

		cycles += cycle_am & cycle_op;
	}
	cycles--;
}

void CPU6502::nmi()
{
}

void CPU6502::irq()
{
}

uint8_t CPU6502::fetch()
{
	if (instructions[opcode].addrmode != &CPU6502::IMP)
		fetched = bus->read(addr_abs);
	return fetched;
}

void CPU6502::loadBinary(const std::string& filename)
{

}

void CPU6502::connectMainbus(MainBus* bus)
{
	this->bus = bus;
}

uint8_t CPU6502::read(uint16_t address)
{
	return bus->read(address);
}

void CPU6502::write(uint16_t address, uint8_t data)
{
	bus->write(address, data);
}



uint8_t CPU6502::ADC()
{
	fetch();
	uint16_t sum = (uint16_t)a + (uint16_t)fetched + (uint16_t)p.c;
	p.c = sum & 0x100;
	p.z = a == 0;
	p.v = (((uint16_t)a ^ sum) & (~((uint16_t)a ^ (uint16_t)fetched)) & 0x0080);
	p.s = sum & 0x80;
	a = sum;
	return 1;
}

uint8_t CPU6502::AND()
{
	fetch();
	a = a & fetched;
	p.z = a == 0;
	p.s = a & 0x80;
	return 1;
}

uint8_t CPU6502::ASL()
{
	p.c = a & 0x80;
	a = a << 1;
	p.z = a == 0;
	p.s = a & 0x80;
	return 0;
}

uint8_t CPU6502::BCC()
{

	if (p.c == 0)
	{
		pc += addr_rel;
		return 1;
	}
	return 0;
}

uint8_t CPU6502::BCS()
{
	if (p.c)
	{
		pc += addr_rel;
		return 1;
	}
	return 0;
}

uint8_t CPU6502::BEQ()
{
	if (p.z)
	{
		pc += addr_rel;
		return 1;
	}
	return 0;
}

uint8_t CPU6502::BIT()
{
	fetch();
	uint8_t tmp = a & fetched;
	p.z = tmp == 0;
	p.v = fetched & 0x40;
	p.s = fetched & 0x80;
	return 0;
}

uint8_t CPU6502::BMI()
{
	if (p.s)
	{
		pc += addr_rel;
		return 1;
	}
	return 0;
}

uint8_t CPU6502::BNE()
{
	if (p.z == 0)
	{
		pc += addr_rel;
		return 1;
	}
	return 0;
}

uint8_t CPU6502::BPL()
{
	if (p.s == 0)
	{
		pc += addr_rel;
		return 1;
	}
	return 0;
}

uint8_t CPU6502::BRK()
{
	return uint8_t();
}

uint8_t CPU6502::BVC()
{
	if (p.v == 0)
	{
		pc += addr_rel;
		return 1;
	}
	return 0;
}

uint8_t CPU6502::BVS()
{
	if (p.v)
	{
		pc += addr_rel;
		return 1;
	}
	return 0;
}

uint8_t CPU6502::CLC()
{
	p.c = 0;
	return 0;
}

uint8_t CPU6502::CLD()
{
	p.d = 0;
	return 0;
}

uint8_t CPU6502::CLI()
{
	p.i = 0;
	return 0;
}

uint8_t CPU6502::CLV()
{
	p.v = 0;
	return 0;
}

uint8_t CPU6502::CMP()
{
	fetch();
	uint8_t tmp = a - fetched;
	p.c = a >= fetched;
	p.z = a == fetched;
	p.s = tmp & 0x80;
	return 1;
}

uint8_t CPU6502::CPX()
{
	fetch();
	uint8_t tmp = x - fetched;
	p.c = x >= fetched;
	p.z = x == fetched;
	p.s = tmp & 0x80;
	return 0;
}

uint8_t CPU6502::CPY()
{
	fetch();
	uint8_t tmp = y - fetched;
	p.c = y >= fetched;
	p.z = y == fetched;
	p.s = tmp & 0x80;
	return 0;
}

uint8_t CPU6502::DEC()
{
	fetch();
	uint8_t tmp = fetched - 1;
	p.z = tmp == 0;
	p.s = tmp & 0x80;
	write(addr_abs, tmp);
	return 0;
}

uint8_t CPU6502::DEX()
{
	--x;
	p.z = x == 0;
	p.s = x & 0x80;
	return 0;
}

uint8_t CPU6502::DEY()
{
	--y;
	p.z = y == 0;
	p.s = y & 0x80;
	return 0;
}

uint8_t CPU6502::EOR()
{
	fetch();
	a = a ^ fetched;
	p.z = a == 0;
	p.s = a & 0x80;
	return 1;
}

uint8_t CPU6502::INC()
{
	fetch();
	uint8_t tmp = fetched + 1;
	p.z = tmp == 0;
	p.s = tmp & 0x80;
	write(addr_abs, tmp);
	return 0;
}

uint8_t CPU6502::INX()
{
	++x;
	p.z = x == 0;
	p.s = x & 0x80;
	return 0;
}

uint8_t CPU6502::INY()
{
	++y;
	p.z = y == 0;
	p.s = y & 0x80;
	return 0;
}

uint8_t CPU6502::JMP()
{ //TODO special case
	pc = addr_abs;
	return 0;
}

uint8_t CPU6502::JSR()
{
	write(sp++, pc);
	write(sp++, pc >> 8);
	pc = addr_abs;
	return 0;
}

uint8_t CPU6502::LDA()
{
	fetch();
	p.z = fetched == 0;
	p.s = fetched & 0x80;
	a = fetched;
	return 1;
}

uint8_t CPU6502::LDX()
{
	fetch();
	p.z = fetched == 0;
	p.s = fetched & 0x80;
	x = fetched;
	return 1;
}

uint8_t CPU6502::LDY()
{
	fetch();
	p.z = fetched == 0;
	p.s = fetched & 0x80;
	y = fetched;
	return 1;
}

uint8_t CPU6502::LSR()
{
	fetch();
	p.c = fetched & 1;
	uint8_t tmp = fetched >> 1;
	p.z = tmp == 0;
	p.s = tmp & 0x80;
	if (opcode == 0x4A)
		a = tmp;
	else
		write(addr_abs, tmp);
	return 0;
}

uint8_t CPU6502::NOP()
{
	return 0;
}

uint8_t CPU6502::ORA()
{
	fetch();
	a = a | fetched;
	p.z = a == 0;
	p.s = a & 0x80;
	return uint8_t();
}

uint8_t CPU6502::PHA()
{
	return uint8_t();
}

uint8_t CPU6502::PHP()
{
	return uint8_t();
}

uint8_t CPU6502::PLA()
{
	return uint8_t();
}

uint8_t CPU6502::PLP()
{
	return uint8_t();
}

uint8_t CPU6502::ROL()
{
	return uint8_t();
}

uint8_t CPU6502::ROR()
{
	return uint8_t();
}

uint8_t CPU6502::RTI()
{
	return uint8_t();
}

uint8_t CPU6502::RTS()
{
	return uint8_t();
}

uint8_t CPU6502::SBC()
{
	return uint8_t();
}

uint8_t CPU6502::SEC()
{
	return uint8_t();
}

uint8_t CPU6502::SED()
{
	return uint8_t();
}

uint8_t CPU6502::SEI()
{
	return uint8_t();
}

uint8_t CPU6502::STA()
{
	return uint8_t();
}

uint8_t CPU6502::STX()
{
	return uint8_t();
}

uint8_t CPU6502::STY()
{
	return uint8_t();
}

uint8_t CPU6502::TAX()
{
	return uint8_t();
}

uint8_t CPU6502::TAY()
{
	return uint8_t();
}

uint8_t CPU6502::TSX()
{
	return uint8_t();
}

uint8_t CPU6502::TXA()
{
	return uint8_t();
}

uint8_t CPU6502::TXS()
{
	return uint8_t();
}

uint8_t CPU6502::TYA()
{
	return uint8_t();
}

uint8_t CPU6502::XXX()
{
	return 0;
}

uint8_t CPU6502::IMP()
{
	fetched = a;
	return 0;
}

uint8_t CPU6502::IMM()
{
	addr_abs = pc++;
	return 0;
}

uint8_t CPU6502::ZP0()
{
	addr_abs = read(pc++);
	return 0;
}

uint8_t CPU6502::ZPX()
{
	addr_abs = read(pc++) + x;
	return 0;
}

uint8_t CPU6502::ZPY()
{
	addr_abs = read(pc++) + y;
	return 0;
}

uint8_t CPU6502::REL()
{
	addr_rel = read(pc++);
	if (addr_rel & 0x80)
		addr_rel |= 0xFF00;

	return 0;
}

uint8_t CPU6502::ABS()
{
	uint16_t lo = read(pc++);
	uint16_t hi = read(pc++);
	addr_abs = (hi << 8) | lo;
	return 0;
}

uint8_t CPU6502::ABX()
{
	uint16_t lo = read(pc++);
	uint16_t hi = read(pc++);
	addr_abs = ((hi << 8) | lo) + x;
	return 0;
}

uint8_t CPU6502::ABY()
{
	uint16_t lo = read(pc++);
	uint16_t hi = read(pc++);
	addr_abs = ((hi << 8) | lo) + y;
	return 0;
}

uint8_t CPU6502::IND()
{
	uint16_t lo = read(pc++);
	uint16_t hi = read(pc++);
	addr_abs = (hi << 8) | lo;

	lo = read(addr_abs);
	hi = read(addr_abs + 1);
	addr_abs = (hi << 8) | lo;

	return 0;
}

uint8_t CPU6502::IZX()
{
	addr_abs = read(pc++) + x;
	addr_abs = read(addr_abs);

	return 0;
}

uint8_t CPU6502::IZY()
{
	addr_abs = read(pc++) + y;
	addr_abs = read(addr_abs);

	return 0;
}
