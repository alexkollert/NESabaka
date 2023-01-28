#include <NESEmulator.hpp>


bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
  return std::find(begin, end, option) != end;
}

int main(int argc, char** argv)
{
  std::string nes_binary = "test_binaries\\branching.bin";
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
    nes_cpu->reset();
    nes_cpu->loadBinary(nes_binary);

    while (done == 0)
    {
      done = nes_cpu->emulateOp();
    }
  }
}