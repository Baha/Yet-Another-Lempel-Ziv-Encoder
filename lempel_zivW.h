#ifndef _LEMPEL_ZIV_W_H_
#define _LEMPEL_ZIV_W_H_

#include <cstdlib>
#include <string>
#include <map>
#include <vector>

#include "lempel_ziv.h"
#include "binarizer.h"

class LempelZivWEncoder : public LempelZivEncoder
{
  std::map < unsigned int, std::string > dictionary;
  std::map < char , std::string > instant_codes;
  std::vector < unsigned int > outputs;
public:
  void addFirstIndex(char* inputFileName);
  void doEncoding(char* inputFileName);
  void encode(char* inputFileName);
  bool stringExistsInDict(std::string);
  unsigned int getIndexOfString(std::string);
};

#endif
