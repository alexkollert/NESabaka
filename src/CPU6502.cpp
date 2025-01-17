#include "CPU6502.hpp"
#include "MainBus.hpp"
#include <sstream>
#include <iomanip>

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

	emit cpuChanged(pc, sp, a, x, y);
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

void CPU6502::spacePressed()
{
	stepInstruction();
}

void CPU6502::stepInstruction()
{
	while (cycles != 0) clock();
	clock();
	emit cpuChanged(pc, sp, a, x, y);
}

void CPU6502::nmi()
{
}

void CPU6502::irq()
{
	if (p.i  == 0)
	{

	}
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
		cycles++;
		addr_abs = pc + (int8_t)addr_rel;
		
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) //page crossed?
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

uint8_t CPU6502::BCS()
{
	if (p.c)
	{
		cycles++;
		addr_abs = pc + (int8_t)addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) //page crossed?
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

uint8_t CPU6502::BEQ()
{
	if (p.z)
	{
		cycles++;
		addr_abs = pc + (int8_t)addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) //page crossed?
			cycles++;

		pc = addr_abs;
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
		cycles++;
		addr_abs = pc + (int8_t)addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) //page crossed?
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

uint8_t CPU6502::BNE()
{
	if (p.z == 0)
	{
		cycles++;
		addr_abs = pc + (int8_t)addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) //page crossed?
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

uint8_t CPU6502::BPL()
{
	if (p.s == 0)
	{
		cycles++;
		addr_abs = pc + (int8_t)addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) //page crossed?
			cycles++;

		pc = addr_abs;
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
		cycles++;
		addr_abs = pc + (int8_t)addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) //page crossed?
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

uint8_t CPU6502::BVS()
{
	if (p.v)
	{
		cycles++;
		addr_abs = pc + (int8_t)addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) //page crossed?
			cycles++;

		pc = addr_abs;
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
	uint16_t address = --sp | 0x0100;
	write(address, a);
	return 0;
}

uint8_t CPU6502::PHP()
{
	uint8_t statusRegister;
	memcpy(&statusRegister, &p, 1);
	uint16_t tmp = --sp | 0x0100;
	write(tmp, statusRegister);
	return 0;
}

uint8_t CPU6502::PLA()
{
	uint16_t address = sp++ | 0x0100;
	a = read(address);
	p.z = a == 0;
	p.s = a & 0x80;
	return 0;
}

uint8_t CPU6502::PLP()
{
	uint16_t address = sp++ | 0x0100;
	uint8_t statusRegister = read(address);
	memcpy(&p, &statusRegister, 1);
	return 0;
}

uint8_t CPU6502::ROL()
{
	fetch();
	uint16_t tmp = (fetched << 1) | p.c;

	if (instructions[opcode].addrmode == &CPU6502::IMP)
		a = tmp;
	else
		write(addr_abs, tmp);

	p.z = a == 0;
	p.s = tmp & 0x80;
	p.c = tmp & 0x0100;

	return 0;
}

uint8_t CPU6502::ROR()
{
	fetch();
	uint8_t newCarry = fetched & 1;
	uint16_t tmp = (fetched >> 1) | (p.c << 7);

	if (instructions[opcode].addrmode == &CPU6502::IMP)
		a = tmp;
	else
		write(addr_abs, tmp);

	p.z = a == 0;
	p.s = tmp & 0x80;
	p.c = newCarry;

	return 0;
}

uint8_t CPU6502::RTI()
{
	uint16_t address = sp++ | 0x0100;
	uint8_t statusRegister = read(address);
	memcpy(&p, &statusRegister, 1);

	address = sp++ | 0x0100;
	pc = read(address);

	return 0;
}

uint8_t CPU6502::RTS()
{
	uint16_t address = sp++ | 0x0100;
	pc = read(address);
	return 0;
}

uint8_t CPU6502::SBC()
{
	fetch();
	uint16_t tmp = fetched ^ 0xFF;
	uint16_t res = (uint16_t)a + tmp + (uint16_t)1 + (uint16_t)p.c;

	p.c = res & 0x100 ? 0 : 1;
	p.z = a == 0;
	p.v = (((uint16_t)a ^ res) & (~((uint16_t)a ^ (uint16_t)fetched)) & 0x0080);
	p.s = res & 0x80;
	a = res;
	return 1;
}

uint8_t CPU6502::SEC()
{
	p.c = 1;
	return 0;
}

uint8_t CPU6502::SED()
{
	p.d = 1;
	return 0;
}

uint8_t CPU6502::SEI()
{
	p.i = 1;
	return 0;
}

uint8_t CPU6502::STA()
{
	fetch();
	write(addr_abs, a);
	return 0;
}

uint8_t CPU6502::STX()
{
	fetch();
	write(addr_abs, x);
	return 0;
}

uint8_t CPU6502::STY()
{
	fetch();
	write(addr_abs, y);
	return 0;
}

uint8_t CPU6502::TAX()
{
	x = a;
	p.z = x == 0;
	p.s = x & 0x80;
	return 0;
}

uint8_t CPU6502::TAY()
{
	y = a;
	p.z = y == 0;
	p.s = y & 0x80;
	return 0;
}

uint8_t CPU6502::TSX()
{
	x = sp;
	p.z = x == 0;
	p.s = x & 0x80;
	return 0;
}

uint8_t CPU6502::TXA()
{
	a = x;
	p.z = a == 0;
	p.s = a & 0x80;
	return 0;
}

uint8_t CPU6502::TXS()
{
	sp = x;
	return 0;
}

uint8_t CPU6502::TYA()
{
	a = y;
	p.z = a == 0;
	p.s = a & 0x80;
	return 0;
}

uint8_t CPU6502::XXX()
{
	return 0;
}

uint8_t CPU6502::IMP()
{
#ifdef _DEBUG
	disassembled = "";
#endif

	fetched = a;
	return 0;
}

uint8_t CPU6502::IMM()
{
	addr_abs = pc++;
#ifdef _DEBUG
	std::stringstream stream;
	stream << "#" << std::setw(2) << std::setfill('0') << std::hex << std::to_string(read(addr_abs));
	disassembled = stream.str();
#endif
	return 0;
}

uint8_t CPU6502::ZP0()
{
	addr_abs = read(pc++);
#ifdef _DEBUG
	std::stringstream stream;
	stream << "$" << std::setw(2) << std::setfill('0') << std::hex << addr_abs;
	disassembled = stream.str();
#endif
	return 0;
}

uint8_t CPU6502::ZPX()
{
#ifdef _DEBUG
	std::stringstream stream;
	stream << "$" << std::setw(2) << std::setfill('0') << std::hex << read(pc) << ",X";
	disassembled = stream.str();
#endif
	addr_abs = read(pc++) + x;
	return 0;
}

uint8_t CPU6502::ZPY()
{
#ifdef _DEBUG
	std::stringstream stream;
	stream << "$" << std::setw(2) << std::setfill('0') << std::hex << read(pc) << ",Y";
	disassembled = stream.str();
#endif
	addr_abs = read(pc++) + y;
	return 0;
}

uint8_t CPU6502::REL()
{
 	addr_rel = read(pc++);
	if (addr_rel & 0x80)
		addr_rel |= 0xFF00;

#ifdef _DEBUG
	std::stringstream stream;
	stream << std::to_string((int8_t)addr_rel);
	disassembled = stream.str();
#endif

	return 0;
}

uint8_t CPU6502::ABS()
{
	uint16_t lo = read(pc++);
	uint16_t hi = read(pc++);
	addr_abs = (hi << 8) | lo;
#ifdef _DEBUG
	std::stringstream stream;
	stream << "$" << std::setw(4) << std::setfill('0') << std::hex << addr_abs;
	disassembled = stream.str();
#endif
	return 0;
}

uint8_t CPU6502::ABX()
{
	uint16_t lo = read(pc++);
	uint16_t hi = read(pc++);
	addr_abs = ((hi << 8) | lo) + x;
#ifdef _DEBUG
	std::stringstream stream;
	stream << "$" << std::setw(4) << std::setfill('0') << std::hex << addr_abs;
	disassembled = stream.str();
#endif
	return 0;
}

uint8_t CPU6502::ABY()
{
	uint16_t lo = read(pc++);
	uint16_t hi = read(pc++);
	addr_abs = ((hi << 8) | lo) + y;
#ifdef _DEBUG
	std::stringstream stream;
	stream << "$" << std::setw(4) << std::setfill('0') << std::hex << addr_abs;
	disassembled = stream.str();
#endif
	return 0;
}

uint8_t CPU6502::IND()
{
	uint16_t lo = read(pc++);
	uint16_t hi = read(pc++);
	addr_abs = (hi << 8) | lo;

#ifdef _DEBUG
	std::stringstream stream;
	stream << "($" << std::setw(4) << std::setfill('0')  << std::hex << addr_abs << ")";
	disassembled = stream.str();
#endif

	lo = read(addr_abs);
	hi = read(addr_abs + 1);
	addr_abs = (hi << 8) | lo;

	return 0;
}

uint8_t CPU6502::IZX()
{
#ifdef _DEBUG
	std::stringstream stream;
	stream << "($" << std::setw(2) << std::setfill('0') << std::hex << read(pc) << ",X)";
	disassembled = stream.str();
#endif
	addr_abs = read(pc++) + x;
	addr_abs = read(addr_abs);

	return 0;
}

uint8_t CPU6502::IZY()
{
#ifdef _DEBUG
	std::stringstream stream;
	stream << "($" << std::setw(2) << std::setfill('0') << std::hex << read(pc) << ",X)";
	disassembled = stream.str();
#endif
	addr_abs = read(pc++) + y;
	addr_abs = read(addr_abs);

	return 0;
}
