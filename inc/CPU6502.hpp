#ifndef CPU6502_HPP
#define CPU6502_HPP
#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>
#include <QObject>


class MainBus;

class CPU6502 : public QObject
{
	Q_OBJECT
public:

	typedef struct Statusregister
	{
		uint8_t		c : 1; //carry
		uint8_t		z : 1; //zero
		uint8_t		i : 1; //interrupt disable
		uint8_t		d : 1; //decimal mode
		uint8_t		b : 1; //software interrupt
		uint8_t		pad : 1;
		uint8_t		v : 1; //overflow
		uint8_t		s : 1; //sign
	} Statusregister;

	CPU6502() {}

	void reset();

	void clock();

	void stepInstruction();

	void nmi();

	void irq();

	uint8_t fetch();

	void loadBinary(const std::string& filename);

	void connectMainbus(MainBus* bus);

	std::vector<std::pair<uint16_t, std::string>> disassemble(uint16_t address);

public slots:
	void spacePressed();

signals:
	void cpuChanged(uint16_t pc, uint16_t stkp, uint8_t a, uint8_t x, uint8_t y);

private:

	uint8_t IMP();	uint8_t IMM();
	uint8_t ZP0();	uint8_t ZPX();
	uint8_t ZPY();	uint8_t REL();
	uint8_t ABS();	uint8_t ABX();
	uint8_t ABY();	uint8_t IND();
	uint8_t IZX();	uint8_t IZY();

	uint8_t ADC();	uint8_t AND();	uint8_t ASL();	uint8_t BCC();
	uint8_t BCS();	uint8_t BEQ();	uint8_t BIT();	uint8_t BMI();
	uint8_t BNE();	uint8_t BPL();	uint8_t BRK();	uint8_t BVC();
	uint8_t BVS();	uint8_t CLC();	uint8_t CLD();	uint8_t CLI();
	uint8_t CLV();	uint8_t CMP();	uint8_t CPX();	uint8_t CPY();
	uint8_t DEC();	uint8_t DEX();	uint8_t DEY();	uint8_t EOR();
	uint8_t INC();	uint8_t INX();	uint8_t INY();	uint8_t JMP();
	uint8_t JSR();	uint8_t LDA();	uint8_t LDX();	uint8_t LDY();
	uint8_t LSR();	uint8_t NOP();	uint8_t ORA();	uint8_t PHA();
	uint8_t PHP();	uint8_t PLA();	uint8_t PLP();	uint8_t ROL();
	uint8_t ROR();	uint8_t RTI();	uint8_t RTS();	uint8_t SBC();
	uint8_t SEC();	uint8_t SED();	uint8_t SEI();	uint8_t STA();
	uint8_t STX();	uint8_t STY();	uint8_t TAX();	uint8_t TAY();
	uint8_t TSX();	uint8_t TXA();	uint8_t TXS();	uint8_t TYA();

	uint8_t XXX();

	uint8_t read(uint16_t address);

	void write(uint16_t address, uint8_t data);

	struct INSTRUCTION
	{
		std::string name;
		uint8_t(CPU6502::*f)(void) = nullptr;
		uint8_t(CPU6502::*addrmode)(void) = nullptr;
		uint8_t cycles = 0;
	};

	INSTRUCTION instructions[256] =
	{
		{ "BRK", &CPU6502::BRK, &CPU6502::IMM, 7 }, { "ORA", &CPU6502::ORA, &CPU6502::IZX, 6 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 8 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 3 }, { "ORA", &CPU6502::ORA, &CPU6502::ZP0, 3 }, { "ASL", &CPU6502::ASL, &CPU6502::ZP0, 5 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 5 }, { "PHP", &CPU6502::PHP, &CPU6502::IMP, 3 }, { "ORA", &CPU6502::ORA, &CPU6502::IMM, 2 }, { "ASL", &CPU6502::ASL, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 2 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 4 }, { "ORA", &CPU6502::ORA, &CPU6502::ABS, 4 }, { "ASL", &CPU6502::ASL, &CPU6502::ABS, 6 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 6 },
		{ "BPL", &CPU6502::BPL, &CPU6502::REL, 2 }, { "ORA", &CPU6502::ORA, &CPU6502::IZY, 5 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 8 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 4 }, { "ORA", &CPU6502::ORA, &CPU6502::ZPX, 4 }, { "ASL", &CPU6502::ASL, &CPU6502::ZPX, 6 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 6 }, { "CLC", &CPU6502::CLC, &CPU6502::IMP, 2 }, { "ORA", &CPU6502::ORA, &CPU6502::ABY, 4 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 7 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 4 }, { "ORA", &CPU6502::ORA, &CPU6502::ABX, 4 }, { "ASL", &CPU6502::ASL, &CPU6502::ABX, 7 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 7 },
		{ "JSR", &CPU6502::JSR, &CPU6502::ABS, 6 }, { "AND", &CPU6502::AND, &CPU6502::IZX, 6 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 8 }, { "BIT", &CPU6502::BIT, &CPU6502::ZP0, 3 }, { "AND", &CPU6502::AND, &CPU6502::ZP0, 3 }, { "ROL", &CPU6502::ROL, &CPU6502::ZP0, 5 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 5 }, { "PLP", &CPU6502::PLP, &CPU6502::IMP, 4 }, { "AND", &CPU6502::AND, &CPU6502::IMM, 2 }, { "ROL", &CPU6502::ROL, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 2 }, { "BIT", &CPU6502::BIT, &CPU6502::ABS, 4 }, { "AND", &CPU6502::AND, &CPU6502::ABS, 4 }, { "ROL", &CPU6502::ROL, &CPU6502::ABS, 6 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 6 },
		{ "BMI", &CPU6502::BMI, &CPU6502::REL, 2 }, { "AND", &CPU6502::AND, &CPU6502::IZY, 5 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 8 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 4 }, { "AND", &CPU6502::AND, &CPU6502::ZPX, 4 }, { "ROL", &CPU6502::ROL, &CPU6502::ZPX, 6 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 6 }, { "SEC", &CPU6502::SEC, &CPU6502::IMP, 2 }, { "AND", &CPU6502::AND, &CPU6502::ABY, 4 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 7 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 4 }, { "AND", &CPU6502::AND, &CPU6502::ABX, 4 }, { "ROL", &CPU6502::ROL, &CPU6502::ABX, 7 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 7 },
		{ "RTI", &CPU6502::RTI, &CPU6502::IMP, 6 }, { "EOR", &CPU6502::EOR, &CPU6502::IZX, 6 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 8 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 3 }, { "EOR", &CPU6502::EOR, &CPU6502::ZP0, 3 }, { "LSR", &CPU6502::LSR, &CPU6502::ZP0, 5 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 5 }, { "PHA", &CPU6502::PHA, &CPU6502::IMP, 3 }, { "EOR", &CPU6502::EOR, &CPU6502::IMM, 2 }, { "LSR", &CPU6502::LSR, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 2 }, { "JMP", &CPU6502::JMP, &CPU6502::ABS, 3 }, { "EOR", &CPU6502::EOR, &CPU6502::ABS, 4 }, { "LSR", &CPU6502::LSR, &CPU6502::ABS, 6 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 6 },
		{ "BVC", &CPU6502::BVC, &CPU6502::REL, 2 }, { "EOR", &CPU6502::EOR, &CPU6502::IZY, 5 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 8 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 4 }, { "EOR", &CPU6502::EOR, &CPU6502::ZPX, 4 }, { "LSR", &CPU6502::LSR, &CPU6502::ZPX, 6 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 6 }, { "CLI", &CPU6502::CLI, &CPU6502::IMP, 2 }, { "EOR", &CPU6502::EOR, &CPU6502::ABY, 4 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 7 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 4 }, { "EOR", &CPU6502::EOR, &CPU6502::ABX, 4 }, { "LSR", &CPU6502::LSR, &CPU6502::ABX, 7 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 7 },
		{ "RTS", &CPU6502::RTS, &CPU6502::IMP, 6 }, { "ADC", &CPU6502::ADC, &CPU6502::IZX, 6 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 8 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 3 }, { "ADC", &CPU6502::ADC, &CPU6502::ZP0, 3 }, { "ROR", &CPU6502::ROR, &CPU6502::ZP0, 5 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 5 }, { "PLA", &CPU6502::PLA, &CPU6502::IMP, 4 }, { "ADC", &CPU6502::ADC, &CPU6502::IMM, 2 }, { "ROR", &CPU6502::ROR, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 2 }, { "JMP", &CPU6502::JMP, &CPU6502::IND, 5 }, { "ADC", &CPU6502::ADC, &CPU6502::ABS, 4 }, { "ROR", &CPU6502::ROR, &CPU6502::ABS, 6 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 6 },
		{ "BVS", &CPU6502::BVS, &CPU6502::REL, 2 }, { "ADC", &CPU6502::ADC, &CPU6502::IZY, 5 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 8 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 4 }, { "ADC", &CPU6502::ADC, &CPU6502::ZPX, 4 }, { "ROR", &CPU6502::ROR, &CPU6502::ZPX, 6 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 6 }, { "SEI", &CPU6502::SEI, &CPU6502::IMP, 2 }, { "ADC", &CPU6502::ADC, &CPU6502::ABY, 4 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 7 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 4 }, { "ADC", &CPU6502::ADC, &CPU6502::ABX, 4 }, { "ROR", &CPU6502::ROR, &CPU6502::ABX, 7 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 7 },
		{ "???", &CPU6502::NOP, &CPU6502::IMP, 2 }, { "STA", &CPU6502::STA, &CPU6502::IZX, 6 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 6 }, { "STY", &CPU6502::STY, &CPU6502::ZP0, 3 }, { "STA", &CPU6502::STA, &CPU6502::ZP0, 3 }, { "STX", &CPU6502::STX, &CPU6502::ZP0, 3 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 3 }, { "DEY", &CPU6502::DEY, &CPU6502::IMP, 2 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 2 }, { "TXA", &CPU6502::TXA, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 2 }, { "STY", &CPU6502::STY, &CPU6502::ABS, 4 }, { "STA", &CPU6502::STA, &CPU6502::ABS, 4 }, { "STX", &CPU6502::STX, &CPU6502::ABS, 4 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 4 },
		{ "BCC", &CPU6502::BCC, &CPU6502::REL, 2 }, { "STA", &CPU6502::STA, &CPU6502::IZY, 6 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 6 }, { "STY", &CPU6502::STY, &CPU6502::ZPX, 4 }, { "STA", &CPU6502::STA, &CPU6502::ZPX, 4 }, { "STX", &CPU6502::STX, &CPU6502::ZPY, 4 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 4 }, { "TYA", &CPU6502::TYA, &CPU6502::IMP, 2 }, { "STA", &CPU6502::STA, &CPU6502::ABY, 5 }, { "TXS", &CPU6502::TXS, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 5 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 5 }, { "STA", &CPU6502::STA, &CPU6502::ABX, 5 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 5 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 5 },
		{ "LDY", &CPU6502::LDY, &CPU6502::IMM, 2 }, { "LDA", &CPU6502::LDA, &CPU6502::IZX, 6 }, { "LDX", &CPU6502::LDX, &CPU6502::IMM, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 6 }, { "LDY", &CPU6502::LDY, &CPU6502::ZP0, 3 }, { "LDA", &CPU6502::LDA, &CPU6502::ZP0, 3 }, { "LDX", &CPU6502::LDX, &CPU6502::ZP0, 3 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 3 }, { "TAY", &CPU6502::TAY, &CPU6502::IMP, 2 }, { "LDA", &CPU6502::LDA, &CPU6502::IMM, 2 }, { "TAX", &CPU6502::TAX, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 2 }, { "LDY", &CPU6502::LDY, &CPU6502::ABS, 4 }, { "LDA", &CPU6502::LDA, &CPU6502::ABS, 4 }, { "LDX", &CPU6502::LDX, &CPU6502::ABS, 4 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 4 },
		{ "BCS", &CPU6502::BCS, &CPU6502::REL, 2 }, { "LDA", &CPU6502::LDA, &CPU6502::IZY, 5 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 5 }, { "LDY", &CPU6502::LDY, &CPU6502::ZPX, 4 }, { "LDA", &CPU6502::LDA, &CPU6502::ZPX, 4 }, { "LDX", &CPU6502::LDX, &CPU6502::ZPY, 4 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 4 }, { "CLV", &CPU6502::CLV, &CPU6502::IMP, 2 }, { "LDA", &CPU6502::LDA, &CPU6502::ABY, 4 }, { "TSX", &CPU6502::TSX, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 4 }, { "LDY", &CPU6502::LDY, &CPU6502::ABX, 4 }, { "LDA", &CPU6502::LDA, &CPU6502::ABX, 4 }, { "LDX", &CPU6502::LDX, &CPU6502::ABY, 4 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 4 },
		{ "CPY", &CPU6502::CPY, &CPU6502::IMM, 2 }, { "CMP", &CPU6502::CMP, &CPU6502::IZX, 6 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 8 }, { "CPY", &CPU6502::CPY, &CPU6502::ZP0, 3 }, { "CMP", &CPU6502::CMP, &CPU6502::ZP0, 3 }, { "DEC", &CPU6502::DEC, &CPU6502::ZP0, 5 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 5 }, { "INY", &CPU6502::INY, &CPU6502::IMP, 2 }, { "CMP", &CPU6502::CMP, &CPU6502::IMM, 2 }, { "DEX", &CPU6502::DEX, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 2 }, { "CPY", &CPU6502::CPY, &CPU6502::ABS, 4 }, { "CMP", &CPU6502::CMP, &CPU6502::ABS, 4 }, { "DEC", &CPU6502::DEC, &CPU6502::ABS, 6 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 6 },
		{ "BNE", &CPU6502::BNE, &CPU6502::REL, 2 }, { "CMP", &CPU6502::CMP, &CPU6502::IZY, 5 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 8 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 4 }, { "CMP", &CPU6502::CMP, &CPU6502::ZPX, 4 }, { "DEC", &CPU6502::DEC, &CPU6502::ZPX, 6 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 6 }, { "CLD", &CPU6502::CLD, &CPU6502::IMP, 2 }, { "CMP", &CPU6502::CMP, &CPU6502::ABY, 4 }, { "NOP", &CPU6502::NOP, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 7 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 4 }, { "CMP", &CPU6502::CMP, &CPU6502::ABX, 4 }, { "DEC", &CPU6502::DEC, &CPU6502::ABX, 7 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 7 },
		{ "CPX", &CPU6502::CPX, &CPU6502::IMM, 2 }, { "SBC", &CPU6502::SBC, &CPU6502::IZX, 6 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 8 }, { "CPX", &CPU6502::CPX, &CPU6502::ZP0, 3 }, { "SBC", &CPU6502::SBC, &CPU6502::ZP0, 3 }, { "INC", &CPU6502::INC, &CPU6502::ZP0, 5 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 5 }, { "INX", &CPU6502::INX, &CPU6502::IMP, 2 }, { "SBC", &CPU6502::SBC, &CPU6502::IMM, 2 }, { "NOP", &CPU6502::NOP, &CPU6502::IMP, 2 }, { "???", &CPU6502::SBC, &CPU6502::IMP, 2 }, { "CPX", &CPU6502::CPX, &CPU6502::ABS, 4 }, { "SBC", &CPU6502::SBC, &CPU6502::ABS, 4 }, { "INC", &CPU6502::INC, &CPU6502::ABS, 6 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 6 },
		{ "BEQ", &CPU6502::BEQ, &CPU6502::REL, 2 }, { "SBC", &CPU6502::SBC, &CPU6502::IZY, 5 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 8 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 4 }, { "SBC", &CPU6502::SBC, &CPU6502::ZPX, 4 }, { "INC", &CPU6502::INC, &CPU6502::ZPX, 6 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 6 }, { "SED", &CPU6502::SED, &CPU6502::IMP, 2 }, { "SBC", &CPU6502::SBC, &CPU6502::ABY, 4 }, { "NOP", &CPU6502::NOP, &CPU6502::IMP, 2 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 7 }, { "???", &CPU6502::NOP, &CPU6502::IMP, 4 }, { "SBC", &CPU6502::SBC, &CPU6502::ABX, 4 }, { "INC", &CPU6502::INC, &CPU6502::ABX, 7 }, { "???", &CPU6502::XXX, &CPU6502::IMP, 7 },
	};

	uint8_t a = 0;
	uint8_t x = 0;;
	uint8_t y = 0;;
	struct Statusregister p {};
	uint8_t sp = 0xFD;;
	uint16_t pc = 0;;
	uint8_t int_enable = 0;;

	uint8_t fetched = 0;
	uint16_t addr_abs = 0x0000;
	uint8_t addr_rel = 0x00;
	uint8_t opcode = 0x00;
	uint8_t cycles = 0;
	std::string disassembled;
	MainBus* bus = nullptr;

};
#endif

