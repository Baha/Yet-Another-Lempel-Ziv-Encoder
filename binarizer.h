/**
 * @file
 * Defines the classes Binarizer and Debinarizer.
 *
 * These classes are useful for joining the binary
 * codes and reading them in a codified file later.
 */


#ifndef _BINARIZER_H_
#define _BINARIZER_H_

#include <cstdio>
#include <cstring>
#include <iostream>

/**
 * The length of a byte.
 */
#define BYTE_LENGTH 8

/**
 * A function for returning the minimum between two
 * integers.
 */
inline int min(int a, int b)
{
	if (a < b)
		return a;
	return b;
}

/**
 * The Binarizer class is used for building a binary string
 * that we will write into the codified file.
 *
 * We will read the bits from the codes of the characters we
 * read and we will push them into a string of characters so
 * we can join more than one codes into one characters.
 */
class Binarizer
{
protected:
 /** The "binaryCode" string will contain the string with 
	 * the new characters that come from the union of the codes
	 * of the characters we have read from the file.
	 */
	std::string binaryCode;
	/**
	 * The "bits_used" attribute will count the bits we have
	 * already used from the byte that we are trying to compose.
	 */ 
	unsigned int bits_used;
	/**
	 * The "cur_byte" represents a new character that will be
	 * added to the string once we have put 8 bits of code
	 * on it.
	 */
	char cur_byte;
	/**
	 * The "offset" is a number that contains the number of non-used
	 * bits at the last byte of the codified file.
	 */
   unsigned int offset;
public:
	Binarizer();
  int getCodeLength();
	int getOffset();
  std::string getBinaryCode();
	void addStringToCode(const char* code);
	void processString(int cur_shift, int bits_to_process, const char* code);
	void processChar(char bit);
	void finalizeEncoding();
	std::string printCode();
};

/**
 * The Debinarizer class makes the inverse process to binarizing
 * the code.
 *
 * It will read the codified file and will put the bits into a string,
 * so later we can start reading parts of this string in order to get
 * the codes and print them into a new decodified file.
 */ 
class Debinarizer
{
protected:
  /**
	 * The "binaryCode" string is a string that will contain only
	 * 0s or 1s. We will be reading this string for getting the codes
	 * from the codified file.
	 *
	 * Note that a file with N characters to decodify will make this
	 * string to have a length of 8*N characters, so it can fail while
	 * analyzing a large file.
	 */ 
	std::string binaryCode;
	/**
	 * The "tempCode" is an auxiliar string that will contain the bits
	 * for the code that we are analyzing.
	 * 
	 * We will be putting bits into it until we find a code that
	 * matches the string that "tempCode" contains, so we can emit
	 * the corresponding character and reset this string.
	 */
	std::string tempCode;
	/**
	 * "cur_char" is a counter that points the bit to analyze
	 * in the "binaryCode" string.
	 */
	int cur_char;
	/**
	 * The "offset" is a number that contains the number of non-used
	 * bits at the last byte of the codified file.
	 */
	int offset;
public:
	Debinarizer();
	void setOffset(int offset);
	void addCharToString(char character);
	void resetTempCode();
	std::string getStringOfNBits(unsigned int n);
	bool codesLeft();
};

#endif
