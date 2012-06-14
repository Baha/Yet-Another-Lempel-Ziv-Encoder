#include "lempel_ziv.h"
#include <cstdio>

/**
 * This methods return a binary string of characters of length n for the
 * number given.
 *
 * @param number The number to be codified in binary.
 * @param n The length of the string it should return.
 */
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

/**
 * This method does the opposite to the method "toBinaryString". It
 * parses a binary string and return the number it represents.
 *
 * @param bin_string The string to be parsed by the method.
 */
unsigned int LempelZivEncoder::binStringToInteger(std::string bin_string)
{
  unsigned int num = 0;
  unsigned int j = 0;

  for (int i = bin_string.size() - 1; i >= 0; i--, j++)
    if (bin_string[i] == '1') num = num | 0x01 << j;

  return num;
}
