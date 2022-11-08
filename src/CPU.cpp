#include "CPU.hpp"

#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>

const std::map<uint8_t, CPU::Opcode*> CPU::s_opcodes = {
    {0x69,new Opcode(0x69, "ADC", addressing_mode_e::IMMED, 2)}, /* ADC */
    {0x65,new Opcode(0x65, "ADC", addressing_mode_e::ZEROP, 3)},
    {0x75,new Opcode(0x75, "ADC", addressing_mode_e::ZEPIX, 4)},
    {0x6D,new Opcode(0x6D, "ADC", addressing_mode_e::ABSOL, 4)},
    {0x7D,new Opcode(0x7D, "ADC", addressing_mode_e::ABSIX, 4)},
    {0x79,new Opcode(0x79, "ADC", addressing_mode_e::ABSIY, 4)},
    {0x61,new Opcode(0x61, "ADC", addressing_mode_e::INDIN, 6)},
    {0x71,new Opcode(0x71, "ADC", addressing_mode_e::ININD, 5)},
    {0x29,new Opcode(0x29, "AND", addressing_mode_e::IMMED, 2)}, /* AND */
    {0x25,new Opcode(0x25, "AND", addressing_mode_e::ZEROP, 3)},
    {0x35,new Opcode(0x35, "AND", addressing_mode_e::ZEPIX, 4)},
    {0x2D,new Opcode(0x2D, "AND", addressing_mode_e::ABSOL, 4)},
    {0x3D,new Opcode(0x3D, "AND", addressing_mode_e::ABSIX, 4)},
    {0x39,new Opcode(0x39, "AND", addressing_mode_e::ABSIY, 4)},
    {0x21,new Opcode(0x21, "AND", addressing_mode_e::INDIN, 6)},
    {0x31,new Opcode(0x31, "AND", addressing_mode_e::ININD, 5)},
    {0x0A,new Opcode(0x0A, "ASL", addressing_mode_e::ACCUM, 2)}, /* ASL */
    {0x06,new Opcode(0x06, "ASL", addressing_mode_e::ZEROP, 5)},
    {0x16,new Opcode(0x16, "ASL", addressing_mode_e::ZEPIX, 6)},
    {0x0E,new Opcode(0x0E, "ASL", addressing_mode_e::ABSOL, 6)},
    {0x1E,new Opcode(0x1E, "ASL", addressing_mode_e::ABSIX, 7)},
    {0x90,new Opcode(0x90, "BCC", addressing_mode_e::RELAT, 2)}, /* BCC */
    {0xB0,new Opcode(0xB0, "BCS", addressing_mode_e::RELAT, 2)}, /* BCS */
    {0xF0,new Opcode(0xF0, "BEQ", addressing_mode_e::RELAT, 2)}, /* BEQ */
    {0x24,new Opcode(0x24, "BIT", addressing_mode_e::ZEROP, 3)}, /* BIT */
    {0x2C,new Opcode(0x2C, "BIT", addressing_mode_e::ABSOL, 4)},
    {0x30,new Opcode(0x30, "BMI", addressing_mode_e::RELAT, 2)}, /* BMI */
    {0xD0,new Opcode(0xD0, "BNE", addressing_mode_e::RELAT, 2)}, /* BNE */
    {0x10,new Opcode(0x10, "BPL", addressing_mode_e::RELAT, 2)}, /* BPL */
    {0x00,new Opcode(0x00, "BRK", addressing_mode_e::IMPLI, 7)}, /* BRK */
    {0x50,new Opcode(0x50, "BVC", addressing_mode_e::RELAT, 2)}, /* BVC */
    {0x70,new Opcode(0x70, "BVS", addressing_mode_e::RELAT, 2)}, /* BVS */
    {0x18,new Opcode(0x18, "CLC", addressing_mode_e::IMPLI, 2)}, /* CLC */
    {0xD8,new Opcode(0xD8, "CLD", addressing_mode_e::IMPLI, 2)}, /* CLD */
    {0x58,new Opcode(0x58, "CLI", addressing_mode_e::IMPLI, 2)}, /* CLI */
    {0xB8,new Opcode(0xB8, "CLV", addressing_mode_e::IMPLI, 2)}, /* CLV */
    {0xC9,new Opcode(0xC9, "CMP", addressing_mode_e::IMMED, 2)}, /* CMP */
    {0xC5,new Opcode(0xC5, "CMP", addressing_mode_e::ZEROP, 3)},
    {0xD5,new Opcode(0xD5, "CMP", addressing_mode_e::ZEPIX, 4)},
    {0xCD,new Opcode(0xCD, "CMP", addressing_mode_e::ABSOL, 4)},
    {0xDD,new Opcode(0xDD, "CMP", addressing_mode_e::ABSIX, 4)},
    {0xD9,new Opcode(0xD9, "CMP", addressing_mode_e::ABSIY, 4)},
    {0xC1,new Opcode(0xC1, "CMP", addressing_mode_e::INDIN, 6)},
    {0xD1,new Opcode(0xD1, "CMP", addressing_mode_e::ININD, 5)},
    {0xE0,new Opcode(0xE0, "CPX", addressing_mode_e::IMMED, 2)}, /* CPX */
    {0xE4,new Opcode(0xE4, "CPX", addressing_mode_e::ZEROP, 3)},
    {0xEC,new Opcode(0xEC, "CPX", addressing_mode_e::ABSOL, 4)},
    {0xC0,new Opcode(0xC0, "CPY", addressing_mode_e::IMMED, 2)}, /* CPY */
    {0xC4,new Opcode(0xC4, "CPY", addressing_mode_e::ZEROP, 3)},
    {0xCC,new Opcode(0xCC, "CPY", addressing_mode_e::ABSOL, 4)},
    {0xC6,new Opcode(0xC6, "DEC", addressing_mode_e::ZEROP, 5)}, /* DEC */
    {0xD6,new Opcode(0xD6, "DEC", addressing_mode_e::ZEPIX, 6)},
    {0xCE,new Opcode(0xCE, "DEC", addressing_mode_e::ABSOL, 6)},
    {0xDE,new Opcode(0xDE, "DEC", addressing_mode_e::ABSIX, 7)},
    {0xCA,new Opcode(0xCA, "DEX", addressing_mode_e::IMPLI, 2)}, /* DEX */
    {0x88,new Opcode(0x88, "DEY", addressing_mode_e::IMPLI, 2)}, /* DEY */
    {0x49,new Opcode(0x49, "EOR", addressing_mode_e::IMMED, 2)}, /* EOR */
    {0x45,new Opcode(0x45, "EOR", addressing_mode_e::ZEROP, 3)},
    {0x55,new Opcode(0x55, "EOR", addressing_mode_e::ZEPIX, 4)},
    {0x4D,new Opcode(0x4D, "EOR", addressing_mode_e::ABSOL, 4)},
    {0x5D,new Opcode(0x5D, "EOR", addressing_mode_e::ABSIX, 4)},
    {0x59,new Opcode(0x59, "EOR", addressing_mode_e::ABSIY, 4)},
    {0x41,new Opcode(0x41, "EOR", addressing_mode_e::INDIN, 6)},
    {0x51,new Opcode(0x51, "EOR", addressing_mode_e::ININD, 5)},
    {0xE6,new Opcode(0xE6, "INC", addressing_mode_e::ZEROP, 5)}, /* INC */
    {0xF6,new Opcode(0xF6, "INC", addressing_mode_e::ZEPIX, 6)},
    {0xEE,new Opcode(0xEE, "INC", addressing_mode_e::ABSOL, 6)},
    {0xFE,new Opcode(0xFE, "INC", addressing_mode_e::ABSIX, 7)},
    {0xE8,new Opcode(0xE8, "INX", addressing_mode_e::IMPLI, 2)}, /* INX */
    {0xC8,new Opcode(0xC8, "INY", addressing_mode_e::IMPLI, 2)}, /* INY */
    {0x4C,new Opcode(0x4C, "JMP", addressing_mode_e::ABSOL, 3)}, /* JMP */
    {0x6C,new Opcode(0x6C, "JMP", addressing_mode_e::INDIA, 5)},
    {0x20,new Opcode(0x20, "JSR", addressing_mode_e::ABSOL, 6)}, /* JSR */
    {0xA9,new Opcode(0xA9, "LDA", addressing_mode_e::IMMED, 2)}, /* LDA */
    {0xA5,new Opcode(0xA5, "LDA", addressing_mode_e::ZEROP, 3)},
    {0xB5,new Opcode(0xB5, "LDA", addressing_mode_e::ZEPIX, 4)},
    {0xAD,new Opcode(0xAD, "LDA", addressing_mode_e::ABSOL, 4)},
    {0xBD,new Opcode(0xBD, "LDA", addressing_mode_e::ABSIX, 4)},
    {0xB9,new Opcode(0xB9, "LDA", addressing_mode_e::ABSIY, 4)},
    {0xA1,new Opcode(0xA1, "LDA", addressing_mode_e::INDIN, 6)},
    {0xB1,new Opcode(0xB1, "LDA", addressing_mode_e::ININD, 5)},
    {0xA2,new Opcode(0xA2, "LDX", addressing_mode_e::IMMED, 2)}, /* LDX */
    {0xA6,new Opcode(0xA6, "LDX", addressing_mode_e::ZEROP, 3)},
    {0xB6,new Opcode(0xB6, "LDX", addressing_mode_e::ZEPIY, 4)},
    {0xAE,new Opcode(0xAE, "LDX", addressing_mode_e::ABSOL, 4)},
    {0xBE,new Opcode(0xBE, "LDX", addressing_mode_e::ABSIY, 4)},
    {0xA0,new Opcode(0xA0, "LDY", addressing_mode_e::IMMED, 2)}, /* LDY */
    {0xA4,new Opcode(0xA4, "LDY", addressing_mode_e::ZEROP, 3)},
    {0xB4,new Opcode(0xB4, "LDY", addressing_mode_e::ZEPIX, 4)},
    {0xAC,new Opcode(0xAC, "LDY", addressing_mode_e::ABSOL, 4)},
    {0xBC,new Opcode(0xBC, "LDY", addressing_mode_e::ABSIX, 4)},
    {0x4A,new Opcode(0x4A, "LSR", addressing_mode_e::ACCUM, 2)}, /* LSR */
    {0x46,new Opcode(0x46, "LSR", addressing_mode_e::ZEROP, 5)},
    {0x56,new Opcode(0x56, "LSR", addressing_mode_e::ZEPIX, 6)},
    {0x4E,new Opcode(0x4E, "LSR", addressing_mode_e::ABSOL, 6)},
    {0x5E,new Opcode(0x5E, "LSR", addressing_mode_e::ABSIX, 7)},
    {0xEA,new Opcode(0xEA, "NOP", addressing_mode_e::IMPLI, 2)}, /* NOP */
    {0x09,new Opcode(0x09, "ORA", addressing_mode_e::IMMED, 2)}, /* ORA */
    {0x05,new Opcode(0x05, "ORA", addressing_mode_e::ZEROP, 3)},
    {0x15,new Opcode(0x15, "ORA", addressing_mode_e::ZEPIX, 4)},
    {0x0D,new Opcode(0x0D, "ORA", addressing_mode_e::ABSOL, 4)},
    {0x1D,new Opcode(0x1D, "ORA", addressing_mode_e::ABSIX, 4)},
    {0x19,new Opcode(0x19, "ORA", addressing_mode_e::ABSIY, 4)},
    {0x01,new Opcode(0x01, "ORA", addressing_mode_e::INDIN, 6)},
    {0x11,new Opcode(0x11, "ORA", addressing_mode_e::ININD, 5)},
    {0x48,new Opcode(0x48, "PHA", addressing_mode_e::IMPLI, 3)}, /* PHA */
    {0x08,new Opcode(0x08, "PHP", addressing_mode_e::IMPLI, 3)}, /* PHP */
    {0x68,new Opcode(0x68, "PLA", addressing_mode_e::IMPLI, 4)}, /* PLA */
    {0x28,new Opcode(0x28, "PLP", addressing_mode_e::IMPLI, 4)}, /* PLP */
    {0x2A,new Opcode(0x2A, "ROL", addressing_mode_e::ACCUM, 2)}, /* ROL */
    {0x26,new Opcode(0x26, "ROL", addressing_mode_e::ZEROP, 5)},
    {0x36,new Opcode(0x36, "ROL", addressing_mode_e::ZEPIX, 6)},
    {0x2E,new Opcode(0x2E, "ROL", addressing_mode_e::ABSOL, 6)},
    {0x3E,new Opcode(0x3E, "ROL", addressing_mode_e::ABSIX, 7)},
    {0x6A,new Opcode(0x6A, "ROR", addressing_mode_e::ACCUM, 2)}, /* ROR */
    {0x66,new Opcode(0x66, "ROR", addressing_mode_e::ZEROP, 5)},
    {0x76,new Opcode(0x76, "ROR", addressing_mode_e::ZEPIX, 6)},
    {0x6E,new Opcode(0x6E, "ROR", addressing_mode_e::ABSOL, 6)},
    {0x7E,new Opcode(0x7E, "ROR", addressing_mode_e::ABSIX, 7)},
    {0x40,new Opcode(0x40, "RTI", addressing_mode_e::IMPLI, 6)}, /* RTI */
    {0x60,new Opcode(0x60, "RTS", addressing_mode_e::IMPLI, 6)}, /* RTS */
    {0xE9,new Opcode(0xE9, "SBC", addressing_mode_e::IMMED, 2)}, /* SBC */
    {0xE5,new Opcode(0xE5, "SBC", addressing_mode_e::ZEROP, 3)},
    {0xF5,new Opcode(0xF5, "SBC", addressing_mode_e::ZEPIX, 4)},
    {0xED,new Opcode(0xED, "SBC", addressing_mode_e::ABSOL, 4)},
    {0xFD,new Opcode(0xFD, "SBC", addressing_mode_e::ABSIX, 4)},
    {0xF9,new Opcode(0xF9, "SBC", addressing_mode_e::ABSIY, 4)},
    {0xE1,new Opcode(0xE1, "SBC", addressing_mode_e::INDIN, 6)},
    {0xF1,new Opcode(0xF1, "SBC", addressing_mode_e::ININD, 5)},
    {0x38,new Opcode(0x38, "SEC", addressing_mode_e::IMPLI, 2)}, /* SEC */
    {0xF8,new Opcode(0xF8, "SED", addressing_mode_e::IMPLI, 2)}, /* SED */
    {0x78,new Opcode(0x78, "SEI", addressing_mode_e::IMPLI, 2)}, /* SEI */
    {0x85,new Opcode(0x85, "STA", addressing_mode_e::ZEROP, 3)}, /* STA */
    {0x95,new Opcode(0x95, "STA", addressing_mode_e::ZEPIX, 4)},
    {0x8D,new Opcode(0x8D, "STA", addressing_mode_e::ABSOL, 4)},
    {0x9D,new Opcode(0x9D, "STA", addressing_mode_e::ABSIX, 4)},
    {0x99,new Opcode(0x99, "STA", addressing_mode_e::ABSIY, 4)},
    {0x81,new Opcode(0x81, "STA", addressing_mode_e::INDIN, 6)},
    {0x91,new Opcode(0x91, "STA", addressing_mode_e::ININD, 5)},
    {0x86,new Opcode(0x86, "STX", addressing_mode_e::ZEROP, 3)}, /* STX */
    {0x96,new Opcode(0x96, "STX", addressing_mode_e::ZEPIY, 4)},
    {0x8E,new Opcode(0x8E, "STX", addressing_mode_e::ABSOL, 4)},
    {0x84,new Opcode(0x84, "STY", addressing_mode_e::ZEROP, 3)}, /* STY */
    {0x94,new Opcode(0x94, "STY", addressing_mode_e::ZEPIX, 4)},
    {0x8C,new Opcode(0x8C, "STY", addressing_mode_e::ABSOL, 4)},
    {0xAA,new Opcode(0xAA, "TAX", addressing_mode_e::IMPLI, 2)}, /* TAX */
    {0xA8,new Opcode(0xA8, "TAY", addressing_mode_e::IMPLI, 2)}, /* TAY */
    {0xBA,new Opcode(0xBA, "TSX", addressing_mode_e::IMPLI, 2)}, /* TSX */
    {0x8A,new Opcode(0x8A, "TXA", addressing_mode_e::IMPLI, 2)}, /* TXA */
    {0x9A,new Opcode(0x9A, "TXS", addressing_mode_e::IMPLI, 2)}, /* TXS */
    {0x98,new Opcode(0x98, "TYA", addressing_mode_e::IMPLI, 2)} /* TYA */
};

char CPU::_fetchNextOpcode()
{
  return memory[pc];
}

uint16_t CPU::getTwoBytesFromMemory(int pcOffset)
{
  //((opcode[1] << 8) & 0xff00) + (opcode[2] & 0xff);
  return ((uint16_t)memory[(pc + pcOffset) << 8]) + ((uint16_t)memory[pc + pcOffset + 1]);
}

uint8_t CPU::getByteFromMemory(int pcOffset)
{
  return memory[pc + pcOffset];
}

int CPU::emulateOp()
{
  uint16_t res = 0;
  uint16_t address = 0;
  uint8_t operand = 0;
  uint8_t oldCarry = 0;
  uint16_t sum;
  uint8_t opcode = _fetchNextOpcode();

  switch (opcode)
  {
  case 0x00: //brk
      //TODO
    printf("program finished execution.\n");
    return 1;
  case 0x0a: //asl
    printf("shift register a = (%d) left\n", a);
    p.c = (a & 0x80) >> 7;
    a = (a << 1);
    p.z = (a == 0);
    p.s = (a >> 7) & 1;
    pc += 2;
    break;
  case 0x20:
    pc--;
    memory[sp] = (pc >> 8) & 0xff;
    sp--;
    memory[sp] = pc & 0xff;
    sp--;
    pc = getTwoBytesFromMemory(1);
    printf("jump subroutine to address %04x\n", pc);
    break;
  case 0x30: //bmi
    if (p.s)
    {
      pc = (pc + 2) + (int8_t)getByteFromMemory(1);
      printf("branching. new pc is %04x\n", pc);
    }
    else
    {
      printf("branch not taken.\n");
      pc += 2;
    }
    break;
  case 0x36: //rol
    operand = memory[getByteFromMemory(1) + x];
    printf("rotate memory operand %d left\n", operand);
    oldCarry = p.c;
    p.c = (operand & 0x80) >> 7;
    operand = (operand << 1) + oldCarry;
    memory[getByteFromMemory(1) + x] = operand;
    p.z = (operand == 0);
    p.s = (operand >> 7) & 1;
    pc += 2;
    break;
  case 0x65: //adc memory
    printf("execute adc with memory operand: %d + %d + %d\n", a, memory[getByteFromMemory(1)], p.c);
    sum = ((uint16_t)a) + ((uint16_t)memory[getByteFromMemory(1)]) + ((uint16_t)p.c);
    p.c = (sum > 0xff);
    p.v = (a^sum) & (memory[getByteFromMemory(1)] ^ sum) & 0x80;
    a = (uint8_t)sum;
    p.z = (a == 0);
    p.s = (a >> 7) & 1;
    pc += 2;
    break;
  case 0x69: //adc immediate
    printf("execute adc with immediate operand: %d + %d + %d\n", a, getByteFromMemory(1), p.c);
    sum = ((uint16_t)a) + ((uint16_t)getByteFromMemory(1)) + ((uint16_t)p.c);
    p.c = (sum > 0xff);
    p.v = (a^sum) & (getByteFromMemory(1) ^ sum) & 0x80;
    a = (uint8_t)sum;
    p.z = (a == 0);
    p.s = (a >> 7) & 1;
    pc += 2;
    break;
  case 0x85: //sta
    printf("store register a = (%d) into memory\n", a);
    memory[getByteFromMemory(1)] = a;
    pc += 2;
    break;
  case 0x8a: //txa
    printf("transfer register x = (%d) into register a\n", x);
    a = x;
    p.z = (a == 0);
    p.s = (a >> 7) & 1;
    pc++;
    break;
  case 0xa0: //ldy
    printf("loading %d into register y\n", getByteFromMemory(1));
    y = getByteFromMemory(1);
    p.z = (a == 0);
    p.s = (a >> 7) & 1;
    pc += 2;
    break;
  case 0xa2: //ldx
    printf("loading %d into register x\n", getByteFromMemory(1));
    x = getByteFromMemory(1);
    p.z = (a == 0);
    p.s = (a >> 7) & 1;
    pc += 2;
    break;
  case 0xa9: //lda
    printf("loading %d into register a\n", getByteFromMemory(1));
    a = getByteFromMemory(1);
    p.z = (a == 0);
    p.s = (a >> 7) & 1;
    pc += 2;
    break;
  case 0xca: //dex
    printf("decrement x = (%d) by one\n", x);
    x = x - 1;
    p.z = (a == 0);
    p.s = (a >> 7) & 1;
    pc++;
    break;
  case 0xcd: //cmp
    printf("compare acc with absolute address\n");
    address = (getByteFromMemory(1) << 8) & getByteFromMemory(2);
    res = a - memory[address];
    p.c = (res < a);
    p.z = res == 0;
    p.s = (res >> 7) & 1;
    pc += 2;
    break;
  case 0xe9: //sbc
    printf("execute sbc with immediate operand: %d - %d - %d\n", a, getByteFromMemory(1), (1 - p.c));
    res = a - getByteFromMemory(1) - (1 - p.c);
    p.c = (res < a);
    a = res;
    p.z = (a == 0);
    p.s = (a >> 7) & 1;
    pc += 2;
    break;
  default:
    printf("unknown opcode: %02hhX\n", opcode);
    return 1;
  }
  //state->pc += 1;

  printf("\t");
  printf("%c", p.c ? 'c' : '.');
  printf("%c", p.z ? 'z' : '.');
  printf("%c", p.i ? 'i' : '.');
  printf("%c", p.d ? 'd' : '.');
  printf("%c", p.b ? 'b' : '.');
  printf("%c", p.v ? 'v' : '.');
  printf("%c  ", p.s ? 's' : '.');
  printf("A $%02x X $%02x Y $%02x SP %04x\n\n", a, x, y, sp);

  return 0;
}

void CPU::loadBinary(const std::string& filename, uint32_t offset)
{
  //FILE *f = NULL;
  ///errno_t err;
  std::ifstream fs(filename, std::ifstream::binary | std::ifstream::ate);

  if (!fs.is_open())
  {
    std::cout << "could not open file:" << filename << std::endl;
    throw std::exception("exception while opening file!");
  }
  
  std::size_t size = fs.tellg();

  if (size > 0x10000)
  {
    std::cout << "file is too big!" << filename << std::endl;
    throw std::exception("file is too big!");
  }

  fs.seekg(0, std::ios::beg);
  fs.read(memory, size);
  fs.close();

  /*std::filebuf* fb = fs.rdbuf();
  std::size_t size = fb->pubseekoff(0, std::ifstream::end, std::ifstream::in);
  fb->pubseekpos(0, std::ifstream::in);*/

  /*fseek(f, 0L, SEEK_END);
  const int fsize = ftell(f);
  fseek(f, 0L, SEEK_SET);

  uint8_t *buffer = &(memory[offset]);
  fread(buffer, fsize, 1, f);
  fclose(f);*/
}

void CPU::printDisassembly(const std::string& filename)
{
  int byte_count = 0;
  std::stringstream out;
  std::ifstream fs;
  char* tmp;
  //uint8_t opcode = _fetchNextOpcode();

  fs.open(filename);
//  if (fs.is_open())
//  {
//    while (fs.read(tmp, 1).good())//(!feof(input_file) && ((options.org + byte_count) <= 0xFFFFu) && (byte_count < options.max_num_bytes)) {
//      size_t bytes_read = fread(&buffer[options.org + byte_count], 1, 1, input_file);
//    byte_count += bytes_read;
//  }
//}
//
//  while ((pc <= 0xFFFFu)) //&& ((pc - options.org) < byte_count)) {
//  { 
//    //disassemble(tmpstr, buffer, &options, &pc);
//    fprintf(stdout, "%s\n", tmpstr);
//    pc++;
//  }
}
