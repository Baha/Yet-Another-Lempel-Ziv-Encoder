#include <cstdio>
#include <math.h>
#include "lempel_zivW.h"

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

void LempelZivWEncoder::encode(char *inputFileName)
{
  addFirstIndex(inputFileName);
  doEncoding(inputFileName);
}

bool LempelZivWEncoder::stringExistsInDict(std::string new_string)
{
  std::map < unsigned int, std::string >::iterator it;

  for (it = dictionary.begin(); it != dictionary.end(); it++)
    if (new_string == (*it).second) return true;
  return false;
}

unsigned int LempelZivWEncoder::getIndexOfString(std::string new_string)
{
  std::map < unsigned int, std::string >::iterator it;

  for (it = dictionary.begin(); it != dictionary.end(); it++)
    if (new_string == (*it).second) return (*it).first;
}
