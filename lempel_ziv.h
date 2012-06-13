#ifndef _LEMPEL_ZIV_H_
#define _LEMPEL_ZIV_H_

#include <string>

class LempelZivEncoder
{
public:
  virtual void encode(char *inputFileName) = 0;
  virtual void decode(char *inputFileName, char *outputFileName) = 0;
  std::string toBinaryString(unsigned int number, unsigned int n);
  unsigned int binStringToInteger(std::string bin_string);
};

#endif
