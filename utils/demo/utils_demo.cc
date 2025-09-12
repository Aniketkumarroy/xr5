#include "xr5/utils/numbers.h"
#include <iostream>

int main() {
  std::string s;
  xr5::utils::decimalToHex<uint8_t>(-16, s);
  std::cout << s << "\n";
  uint64_t x = 140;
  std::cout << std::hex << x << "\n";
  return 0;
}