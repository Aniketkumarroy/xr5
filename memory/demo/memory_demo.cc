#include "xr5/memory/dram.h"
#include <iostream>

int main() {
  std::array<xr5::types::Byte, 64> p = {56};
  p.fill(5);
  for (int i = 0; i < p.size(); i++)
    std::cout << static_cast<int>(p[i]) << "\n";
  return 0;
}