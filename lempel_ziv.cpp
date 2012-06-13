#include "lempel_ziv.h"
#include <cstdio>

std::string LempelZivEncoder::toBinaryString(unsigned int number, unsigned int n)
{
  std::string new_string("");

  for (unsigned int i = 0; i < n; i++)
  {
    if (number >> i & 0x01)
      new_string.insert(0, "1");
    else new_string.insert(0, "0");
  }

  printf("%d %s\n", number, new_string.c_str());
  return new_string;
}

unsigned int LempelZivEncoder::binStringToInteger(std::string bin_string)
{
  unsigned int num = 0;
  unsigned int j = 0;

  for (int i = bin_string.size() - 1; i >= 0; i--, j++)
    if (bin_string[i] == '1') num = num | 0x01 << j;

  return num;
}
