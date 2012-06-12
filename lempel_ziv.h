#ifndef _LEMPEL_ZIV_H_
#define _LEMPEL_ZIV_H_

#include <string>

class LempelZivEncoder
{
public:
  virtual void encode(char *inputFileName) = 0;
  std::string toBinaryString(unsigned int number, unsigned int n);
};

#endif
