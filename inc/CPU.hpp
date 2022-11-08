#ifndef CPU_hpp
#define CPU_hpp
#include <cstdint>
#include <cstdlib>
#include <map>
#include <string>

typedef union
{
  struct
  {
#ifdef LSB_FIRST
    fakenes_uint8_t low, high;
#else
    fakenes_uint8_t high, low;
#endif
  } bytes;

 // fakenes_uint16_t word;

} pair_t;

class CPU
{
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

  /* The 6502's 13 addressing modes */
  enum class addressing_mode_e { 
    IMMED = 0, /* Immediate */
    ABSOL, /* Absolute */
    ZEROP, /* Zero Page */
    IMPLI, /* Implied */
    INDIA, /* Indirect Absolute */
    ABSIX, /* Absolute indexed with X */
    ABSIY, /* Absolute indexed with Y */
    ZEPIX, /* Zero page indexed with X */
    ZEPIY, /* Zero page indexed with Y */
    INDIN, /* Indexed indirect (with X) */
    ININD, /* Indirect indexed (with Y) */
    RELAT, /* Relative */
    ACCUM /* Accumulator */
  };


  class Opcode
  {
  public:
    Opcode(const uint8_t opcode, const std::string& mnemonic, const addressing_mode_e am, unsigned int cycles)
     : number(opcode), mnemonic(mnemonic), addressing_mode(am), cycles(cycles)
    {}

    uint8_t number;
    std::string mnemonic;
    addressing_mode_e addressing_mode;
    unsigned int cycles;
  };

  CPU() : a(0), x(0), y(0), p(), sp(0x01ff), pc(0), int_enable(0)
  {
    //memory = (uint8_t*)malloc(0x10000); //16K
    memory = new char[0x10000]; //16K
  }

  int emulateOp();
  void loadBinary(const std::string& filename, uint32_t offset = 0);
  void printDisassembly(const std::string& filename);

  static const std::map<uint8_t, Opcode*> s_opcodes;

private:

  char _fetchNextOpcode();
  uint16_t getTwoBytesFromMemory(int pcOffset);
  uint8_t getByteFromMemory(int pcOffset);

  uint8_t a;
  uint8_t x;
  uint8_t y;
  struct Statusregister p;
  uint8_t sp;
  uint16_t pc;
  char* memory;
  uint8_t int_enable;
};

#endif