/**
 * @file
 * Defines the structure of the Lempel-Ziv encoders.
 */

#ifndef _LEMPEL_ZIV_H_
#define _LEMPEL_ZIV_H_

#include <string>

/**
 * The "LempelZivEncoder" is an abstract class for the building
 * the LZ77, LZ78 and LZW encoders.
 *
 * It has two virtual methods without implemention for performing
 * the encoding and decoding process.
 */
class LempelZivEncoder
{
public:
  /**
   * A virtual method with no implementation for performing the 
   * encoding process.
   *
   * @param inputFileName The name of the input file.
   */
  virtual void encode(char *inputFileName) = 0;
  /**
   * A virtual method with no implementation for performing the 
   * decoding process.
   *
   * @param inputFileName The name of the input file.
   * @param outputFileName The name of the output file.
   */
  virtual void decode(char *inputFileName, char *outputFileName) = 0;
  std::string toBinaryString(unsigned int number, unsigned int n);
  unsigned int binStringToInteger(std::string bin_string);
};

#endif
