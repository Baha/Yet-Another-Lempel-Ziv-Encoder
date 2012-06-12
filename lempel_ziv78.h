#ifndef _LEMPEL_ZIV_78_H_
#define _LEMPEL_ZIV_78_H_

#include <string>
#include <map>
#include <vector>

#include "lempel_ziv.h"
#include "code_pair.h"

class LempelZiv78Encoder : public LempelZivEncoder
{
  std::map < unsigned int, std::string > dictionary;
  std::map < char , std::string > instant_codes;
public:
  void encode(char *inputFileName);
  bool stringExistsInDict(std::string new_string);
  unsigned int getIndexOfString(std::string new_string);
  void getInstantCodes(char *inputFileName);
  static char sucChar(char char_suc);
  static std::string sucStr(std::string string_suc);
  void doEncoding(char *inputFileName);
};

#endif
