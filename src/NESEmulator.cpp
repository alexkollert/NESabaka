#include <string>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <CPU.hpp>

//typedef struct Statusregister {
//  uint8_t		c : 1; //carry
//  uint8_t		z : 1; //zero
//  uint8_t		i : 1; //interrupt disable
//  uint8_t		d : 1; //decimal mode
//  uint8_t		b : 1; //software interrupt
//  uint8_t		pad : 1;
//  uint8_t		v : 1; //overflow
//  uint8_t		s : 1; //sign
//} Statusregister;


//typedef struct StateCPU
//{
//  uint8_t a;
//  uint8_t x;
//  uint8_t y;
//  struct Statusregister p;
//  uint8_t sp;
//  uint16_t pc;
//  uint8_t *memory;
//  uint8_t int_enable;
//} StateCPU;


void UnimplementedInstruction()
{
  printf("Error: Unimplemented instruction\n");
  exit(1);
}

//int EmulateCPUOp(CPU *nes_cpu)
//{
//  uint16_t sum;
//  uint8_t opcode = nes_cpu->fetchNextOpcode();
//
//  switch (opcode)
//  {
//  case 0x00: //brk
//      //TODO
//    printf("program finished execution.\n");
//    return 1;
//  case 0x0a: //asl
//    printf("shift register a = (%d) left\n", nes_cpu->a);
//    nes_cpu->p.c = (nes_cpu->a & 0x80) >> 7;
//    nes_cpu->a = (nes_cpu->a << 1);
//    nes_cpu->p.z = (nes_cpu->a == 0);
//    nes_cpu->p.s = (nes_cpu->a >> 7) & 1;
//    nes_cpu->pc += 2;
//    break;
//  case 0x20:
//    nes_cpu->pc--;
//    nes_cpu->memory[nes_cpu->sp] = (nes_cpu->pc >> 8) & 0xff;
//    nes_cpu->sp--;
//    nes_cpu->memory[nes_cpu->sp] = nes_cpu->pc & 0xff;
//    nes_cpu->sp--;
//    nes_cpu->pc = ((opcode[1] << 8) & 0xff00) + (opcode[2] & 0xff);
//    printf("jump subroutine to address %04x\n", nes_cpu->pc);
//    break;
//  case 0x30: //bmi
//    if (nes_cpu->p.s)
//    {
//      nes_cpu->pc = (nes_cpu->pc + 2) + (int8_t)opcode[1];
//      printf("branching. new pc is %04x\n", nes_cpu->pc);
//    }
//    else
//    {
//      printf("branch not taken.\n");
//      nes_cpu->pc += 2;
//    }
//    break;
//  case 0x36: //rol
//    uint8_t operand = nes_cpu->memory[opcode[1] + nes_cpu->x];
//    printf("rotate memory operand %d left\n", operand);
//    uint8_t oldCarry = nes_cpu->p.c;
//    nes_cpu->p.c = (operand & 0x80) >> 7;
//    operand = (operand << 1) + oldCarry;
//    nes_cpu->memory[opcode[1] + nes_cpu->x] = operand;
//    nes_cpu->p.z = (operand == 0);
//    nes_cpu->p.s = (operand >> 7) & 1;
//    nes_cpu->pc += 2;
//    break;
//  case 0x65: //adc memory
//    printf("execute adc with memory operand: %d + %d + %d\n", nes_cpu->a, nes_cpu->memory[opcode[1]], nes_cpu->p.c);
//    sum = (uint16_t)nes_cpu->a + (uint16_t)nes_cpu->memory[opcode[1]] + (uint16_t)nes_cpu->p.c;
//    nes_cpu->p.c = (sum > 0xff);
//    nes_cpu->p.v = (nes_cpu->a^sum) & (nes_cpu->memory[opcode[1]]^sum) & 0x80;
//    nes_cpu->a = (uint8_t)sum;
//    nes_cpu->p.z = (nes_cpu->a == 0);
//    nes_cpu->p.s = (nes_cpu->a >> 7) & 1;
//    nes_cpu->pc += 2;
//    break;
//  case 0x69: //adc immediate
//    printf("execute adc with immediate operand: %d + %d + %d\n", nes_cpu->a, opcode[1], nes_cpu->p.c);
//    sum = (uint16_t)nes_cpu->a + (uint16_t)opcode[1] + (uint16_t)nes_cpu->p.c;
//    nes_cpu->p.c = (sum > 0xff);
//    nes_cpu->p.v = (nes_cpu->a^sum) & (opcode[1]^sum) & 0x80;
//    nes_cpu->a = (uint8_t)sum;
//    nes_cpu->p.z = (nes_cpu->a == 0);
//    nes_cpu->p.s = (nes_cpu->a >> 7) & 1;
//    nes_cpu->pc += 2;
//    break;
//  case 0x85: //sta
//    printf("store register a = (%d) into memory\n", nes_cpu->a);
//    nes_cpu->memory[opcode[1]] = nes_cpu->a;
//    nes_cpu->pc += 2;
//    break;
//  case 0x8a: //txa
//    printf("transfer register x = (%d) into register a\n", nes_cpu->x);
//    nes_cpu->a = nes_cpu->x;
//    nes_cpu->p.z = (nes_cpu->a == 0);
//    nes_cpu->p.s = (nes_cpu->a >> 7) & 1;
//    nes_cpu->pc++;
//    break;
//  case 0xa0: //ldy
//    printf("loading %d into register y\n", opcode[1]);
//    nes_cpu->y = opcode[1];
//    nes_cpu->p.z = (nes_cpu->a == 0);
//    nes_cpu->p.s = (nes_cpu->a >> 7) & 1;
//    nes_cpu->pc += 2;
//    break;
//  case 0xa2: //ldx
//    printf("loading %d into register x\n", opcode[1]);
//    nes_cpu->x = opcode[1];
//    nes_cpu->p.z = (nes_cpu->a == 0);
//    nes_cpu->p.s = (nes_cpu->a >> 7) & 1;
//    nes_cpu->pc += 2;
//    break;
//  case 0xa9: //lda
//    printf("loading %d into register a\n", opcode[1]);
//    nes_cpu->a = opcode[1];
//    nes_cpu->p.z = (nes_cpu->a == 0);
//    nes_cpu->p.s = (nes_cpu->a >> 7) & 1;
//    nes_cpu->pc += 2;
//    break;
//  case 0xca: //dex
//    printf("decrement x = (%d) by one\n", nes_cpu->x);
//    nes_cpu->x = nes_cpu->x - 1;
//    nes_cpu->p.z = (nes_cpu->a == 0);
//    nes_cpu->p.s = (nes_cpu->a >> 7) & 1;
//    nes_cpu->pc++;
//    break;
//  case 0xcd: //cmp
//    printf("compare acc with absolute address\n");
//    uint16_t address = (opcode[1] << 8) & (opcode[2]);
//    uint8_t res = nes_cpu->a - nes_cpu->memory[address];
//    nes_cpu->p.c = (res < nes_cpu->a);
//    nes_cpu->p.z = res == 0;
//    nes_cpu->p.s = (res >> 7) & 1;
//    nes_cpu->pc += 2;
//    break;
//  case 0xe9: //sbc
//    printf("execute sbc with immediate operand: %d - %d - %d\n", nes_cpu->a, opcode[1], (1 - nes_cpu->p.c));
//    res = nes_cpu->a - opcode[1] - (1 - nes_cpu->p.c);
//    nes_cpu->p.c = (res < nes_cpu->a);
//    nes_cpu->a = res;
//    nes_cpu->p.z = (nes_cpu->a == 0);
//    nes_cpu->p.s = (nes_cpu->a >> 7) & 1;
//    nes_cpu->pc += 2;
//    break;
//  default:
//    printf("unknown opcode: %02hhX\n", *opcode);
//    return 1;
//  }
//  //state->pc += 1;
//
//  printf("\t");
//  printf("%c", nes_cpu->p.c ? 'c' : '.');
//  printf("%c", nes_cpu->p.z ? 'z' : '.');
//  printf("%c", nes_cpu->p.i ? 'i' : '.');
//  printf("%c", nes_cpu->p.d ? 'd' : '.');
//  printf("%c", nes_cpu->p.b ? 'b' : '.');
//  printf("%c", nes_cpu->p.v ? 'v' : '.');
//  printf("%c  ", nes_cpu->p.s ? 's' : '.');
//  printf("A $%02x X $%02x Y $%02x SP %04x\n\n", nes_cpu->a, nes_cpu->x, nes_cpu->y, nes_cpu->sp);
//
//  return 0;
//}

//void ReadFileIntoMemoryAt(StateCPU *state, char *filename, uint32_t offset)
//{
//  FILE *f = NULL;
//  errno_t err;
//
//  if ((err = fopen_s(&f, filename, "rb")) != 0)
//  {
//    printf("error: Couldn't open %s\n", filename);
//    exit(1);
//  }
//  fseek(f, 0L, SEEK_END);
//  int fsize = ftell(f);
//  fseek(f, 0L, SEEK_SET);
//
//  uint8_t *buffer = &state->memory[offset];
//  fread(buffer, fsize, 1, f);
//  fclose(f);
//}

//StateCPU *InitCPU(void)
//{
//  StateCPU *state = calloc(1, sizeof(StateCPU));
//  state->memory = malloc(0x10000); //16K
//  state->sp = 0x01ff;
//  return state;
//}
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

int main(int argc, char **argv)
{
  std::string nes_binary = "test_binaries\\snake.bin";
  int opt = 0;
  int done = 0;
  //int vblankcycles = 0;

  if (cmdOptionExists(argv, argv + argc, "-p"))
  {
    // Do stuff
  }

  if (cmdOptionExists(argv, argv + argc, "-e"))
  {
    CPU* nes_cpu = new CPU();
    nes_cpu->loadBinary(nes_binary);

    while (done == 0)
    {
      done = nes_cpu->emulateOp();
    }
  }

  
  return 0;
}