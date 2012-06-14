#include <cstdio>
#include <math.h>
#include "lempel_zivW.h"

/**
 * This method inserts the symbols at the beggining of
 * the dictionary by doing a first parsing of the input file.
 *
 * @param inputFileName The name of the input file.
 */
void LempelZivWEncoder::addFirstIndex(char *inputFileName)
{
  FILE *in = fopen(inputFileName, "r");
  char cur_c;
  std::map <char, unsigned int> symbolCounter;
  std::map <char, unsigned int>::iterator it;

  symbolCounter.clear();
  
  cur_c = fgetc(in);

  while (cur_c != EOF)
  {
    it = symbolCounter.find(cur_c);
    if (it == symbolCounter.end())
      symbolCounter[cur_c] = 1;
      else
        symbolCounter[cur_c]++;
    cur_c = fgetc(in);
  }
  symbolCounter[cur_c] = 1;

  int n_simbolos = symbolCounter.size();

  for (it = symbolCounter.begin(); it != symbolCounter.end(); it++)
    dictionary[dictionary.size() + 1] =((*it).first);

  fclose(in);
}

/**
 * This method performs the real encoding process. It opens
 * the files for input and output, and then executes the LZW
 * algorithm, writing the outputs in the output file.
 *
 * @param inputFileName The name of the input file.
 */
void LempelZivWEncoder::doEncoding(char *inputFileName)
{
  FILE *input;
  FILE *output;
  std::string outputFileName;
  char* dotPointer;
  char c;
  std::string x("");
  Binarizer* binarizer = new Binarizer();

  input = fopen(inputFileName, "r");

  if (input == NULL)
  {
    printf("Fichero de entrada no encontrado.\n");
    exit(-1);
  }

  dotPointer = strchr(inputFileName, '.');

  if (dotPointer != NULL)

    *dotPointer = '\0';
    
  outputFileName = inputFileName;
  outputFileName += ".lzw";

  output = fopen(outputFileName.c_str(), "w");
  
  if (output == NULL)
  {
    printf("Fichero de salida no pudo crearse.\n");
    exit(-1);
  }

  c = fgetc(input);

  while (c != EOF)
  {
    if (stringExistsInDict(x + c))
      x = x + c;
    else
    {
        outputs.push_back(getIndexOfString(x));
        printf("%d ", getIndexOfString(x));
        printf(" -- %d -- %s\n", dictionary.size() + 1, (x + c).c_str()); 
        dictionary[dictionary.size() + 1] = x + c;
        x = c;
    }
    c = fgetc(input);
  }

  if (stringExistsInDict(x + c))
    x = x + c;
  else
  {
      outputs.push_back(getIndexOfString(x));
      printf("%d ", getIndexOfString(x));
      printf(" -- %d -- %s\n", dictionary.size() + 1, (x + c).c_str()); 
      dictionary[dictionary.size() + 1] = x + c;
      x = c;
  }
    outputs.push_back(getIndexOfString(x));
    printf("%d ", getIndexOfString(x));
    printf(" -- %d -- %s\n", dictionary.size() + 1, (x).c_str()); 

  // Ahora hay que imprimir las salidas

  unsigned int index_bits = 1;

  while (pow(2, index_bits) < dictionary.size()) index_bits++;
  printf("ds: %d bits: %d\n", dictionary.size(), index_bits);

  for (unsigned int i = 0; i < outputs.size(); i++)
     binarizer->addStringToCode(toBinaryString(outputs[i], index_bits).c_str());

  fprintf(output, "%d %d\n", dictionary.size(), binarizer->getOffset());
  for (unsigned int i = 0; i < dictionary.size(); i++)
    fprintf(output, "%s\n", dictionary[i].c_str());

  std::string code = binarizer->getBinaryCode();

  fprintf(output, "%s", code.c_str());

  fclose(input);
  fclose(output);
}

/**
 * This method is a wrapper for performing the addition
 * of the symbols in the dictionary and then the encoding process.
 *
 * @param inputFileName The name of the input file.
 */
void LempelZivWEncoder::encode(char *inputFileName)
{
  addFirstIndex(inputFileName);
  doEncoding(inputFileName);
}

/**
 * This method is a boolean method for knowing if the given
 * string is already in the dictionary.
 *
 * @param new_string The string we are looking for in the dictionary.
 */
bool LempelZivWEncoder::stringExistsInDict(std::string new_string)
{
  std::map < unsigned int, std::string >::iterator it;

  for (it = dictionary.begin(); it != dictionary.end(); it++)
    if (new_string == (*it).second) return true;
  return false;
}

/**
 * This method gives us the index in the dictionary for a given string.
 *
 * @param new_string The string whose index we are looking for in the dictionary.
 */
unsigned int LempelZivWEncoder::getIndexOfString(std::string new_string)
{
  std::map < unsigned int, std::string >::iterator it;

  for (it = dictionary.begin(); it != dictionary.end(); it++)
    if (new_string == (*it).second) return (*it).first;
}

/**
 * This method performs the decoding process for the LZ78 algorithm. It first
 * obtains the instant codes for the alphabet, then it debinarizes the sequence
 * of bits in the file and then it write the uncodified file in the output file.
 *
 * @param inputFileName The name of the input file.
 * @param outputFileName The name of the output file.
 */ 
void LempelZivWEncoder::decode(char *inputFileName, char *outputFileName)
{
}
