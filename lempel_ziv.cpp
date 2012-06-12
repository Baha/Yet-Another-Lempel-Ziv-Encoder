#include "lempel_ziv.h"

std::string LempelZivEncoder::toBinaryString(unsigned int number, unsigned int n)
{
  std::string new_string("");

  for (unsigned int i = 0; i < n; i++)
  {
    if (number >> i && 0x01)
      new_string.insert(0, "1");
    else new_string.insert(0, "0");
  }

  return new_string;
}
