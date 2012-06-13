#include "binarizer.h"

/**
 * The void constructor for the Binarizer.
 * Resets the attributes from it.
 */
Binarizer::Binarizer()
{
	binaryCode = "";
	bits_used = 0;
	cur_byte = 0;
  offset = 0;
}

/**
 * A getter for the size of the "binaryCode".
 */
int Binarizer::getCodeLength()
{
  finalizeEncoding();
  return binaryCode.size();
}
/**
 * Returns the number of non-used bits for the
 * current byte.
 */
int Binarizer::getOffset()
{
  finalizeEncoding();
  return this->offset;
}

/**
 * A getter for the attribute "binaryCode".
 */
std::string Binarizer::getBinaryCode()
{
  return this->binaryCode;
}

/**
 * This method will be passing bits from the binary
 * string to a method that will merge them into blocks
 * in the "binaryCode" string.
 */
void Binarizer::addStringToCode(const char* code)
{
	int code_length = strlen(code);
	int cur_shift = 0;
	int bits_to_process;

	while (cur_shift != code_length)
	{
		bits_to_process = min(BYTE_LENGTH - bits_used, code_length - cur_shift);
		processString(cur_shift, bits_to_process, code);
		cur_shift += bits_to_process;
	}
}

/**
 * This method calls the "processChar" process in an individual
 * way for each bit we have to process.
 *
 * @param cur_shift The number that indicates which bit of the string we must
 * process.
 *
 * @param bits_to_process The number of bits we have to process.
 *
 * @param code A pointer to the binary string we have to codify.
 */
void Binarizer::processString(int cur_shift, int bits_to_process, const char* code)
{
	for (int i = 0; i < bits_to_process; i++)
	{
		processChar(code[cur_shift]);
		cur_shift++;
	}
}

/**
 * This method can be tagged as "black magic" if you are not familiar
 * with the C operators for bit manipulation.
 *
 * It just puts the n-th bit of a character (given by "bits_used) to
 * 1 if the bit is '1'.
 *
 * If we have processed 8 bits, we add this character to the binary
 * string and we reset the counters.
 *
 * @param bit The bit to codify into a position of the current byte. 
 */
void Binarizer::processChar(char bit)
{
	char mask = 0x01;

	if (bit == '1')
	{
		mask = mask << bits_used;
		cur_byte = cur_byte | mask;
	}

	bits_used++;

	if (bits_used == BYTE_LENGTH)
	{
		binaryCode.push_back(cur_byte);
		cur_byte = 0;
		bits_used = 0;
	}
}

/**
 * We add the last byte if we have used at least
 * 1 bit of it.
 */
void Binarizer::finalizeEncoding()
{
	if (bits_used != 0)
	{
    binaryCode.push_back(cur_byte);
    offset = (BYTE_LENGTH - bits_used);
    bits_used = 0;
  }
}

/**
 * We finalize the encoding and return the binary string.
 */
std::string Binarizer::printCode()
{
	finalizeEncoding();
	return this->binaryCode;
}
