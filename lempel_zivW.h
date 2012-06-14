/**
 * @file
 * Defines the structure of the Lempel-Ziv W Encoder subclass.
 */

#ifndef _LEMPEL_ZIV_W_H_
#define _LEMPEL_ZIV_W_H_

#include <cstdlib>
#include <string>
#include <map>
#include <vector>

#include "lempel_ziv.h"
#include "binarizer.h"

/**
 * The "LempelZivWEncoder" is a subclass of the "LempelZivEncoder"
 * class for encoding a file using the LZW algorithm.
 *
 * Because of inheritance, this encoder has a method "encode" which
 * is virtual for the parent's class and allows us to use it in the
 * main function as if it was any class of encoder.
 */
class LempelZivWEncoder : public LempelZivEncoder
{
  /**
   * The "dictionary" attribute is a map object which contains
   * the pairs of index and the corresponding entries.
   */
  std::map < unsigned int, std::string > dictionary;
  /**
   * The "instant_codes" is another map object for storing the
   * symbols in the file and their corresponding instant code.
   */
  std::map < char , std::string > instant_codes;
  /**
   * The "outputs" vector is used for storing the outputs
   * and read them from it later, so we can codify these outputs
   * in a comfortable way.
   *
   * It is a vector that stores pairs of indexes and symbols.
   */
  std::vector < unsigned int > outputs;
public:
  void addFirstIndex(char* inputFileName);
  void doEncoding(char* inputFileName);
  void encode(char* inputFileName);
  void decode(char *inputFileName, char *outputFileName);
  bool stringExistsInDict(std::string);
  unsigned int getIndexOfString(std::string);
};

#endif
