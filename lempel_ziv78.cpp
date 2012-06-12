#include <cstdio>
#include <math.h>
#include "lempel_ziv78.h"

char LempelZiv78Encoder::sucChar(char char_suc)
{
  if (char_suc == '1')
    return '0';
  return '1';
}

std::string LempelZiv78Encoder::sucStr(std::string string_suc)
{
  std::string new_string(string_suc);
  new_string.erase(new_string.size() - 1, 1);

  if (string_suc[string_suc.size() - 1] == '1')
  {
    return (sucStr(new_string) + sucChar(string_suc[string_suc.size() - 1]));
  }
  else
    return new_string + sucChar(string_suc[string_suc.size() - 1]);
}

void LempelZiv78Encoder::getInstantCodes(char *inputFileName)
{
  FILE *in = fopen(inputFileName, "r");
  char cur_c;
  std::map <char, unsigned int> symbolCounter;
  std::map <char, unsigned int>::iterator it;
  std::vector < char > alfabeto;

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

  alfabeto.clear();

  for (it = symbolCounter.begin(); it != symbolCounter.end(); it++)
    alfabeto.push_back((*it).first);

  // Ahora tenemos que conseguir la codificacion para los caracteres
  
  int longitud = 1;
  std::vector < unsigned int > longitudes;
  std::vector < std::string > codificaciones;

  while (pow(2, longitud) < n_simbolos) longitud++;

  for (unsigned int i = 0; i < n_simbolos; i++)
    longitudes.push_back(longitud);

  std::string zero_vector;
  
  for (unsigned int i = 0; i < longitud; i++)
    zero_vector.push_back('0');

  codificaciones.push_back(zero_vector);

  for (unsigned int i = 1; i < n_simbolos; i++)
    codificaciones.push_back(sucStr(codificaciones[i - 1]));

  /* for (int i = 0; i < n_simbolos; i++)
    printf("%s\n", codificaciones[i].c_str());*/

  for (unsigned int i = 0; i < n_simbolos; i++)
    instant_codes[alfabeto[i]] = codificaciones[i];

  fclose(in);
}

void LempelZiv78Encoder::doEncoding(char *inputFileName)
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
  outputFileName += ".lz78";

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
      if (stringExistsInDict(x))
      {
        outputs.push_back(std::make_pair(getIndexOfString(x), c));
        printf("%d %c ", getIndexOfString(x), c);
        printf(" -- %d -- %s\n", dictionary.size() + 1, (x + c).c_str()); 
        dictionary[dictionary.size() + 1] = x + c;
        
      }
      else
      {
        outputs.push_back(std::make_pair(0, c));
        dictionary[dictionary.size() + 1] = c;
        printf("0 %c\n", c);
      }
      x = "";
    }
    c = fgetc(input);
  }
  
  if (x.size() != 0)
  {
    // emitimos la salida correspondiente a
    std::string pref_x(x);
    pref_x.erase(x.size() - 1);
    outputs.push_back(std::make_pair(getIndexOfString(pref_x), x[x.size() - 1]));
    printf("%d ", getIndexOfString(x));
    printf(" -- ?? -- %s\n", x.c_str()); 
  }

  // guardamos eof en dicc. y imprimimos su entrada.
  outputs.push_back(std::make_pair(0, c));
  dictionary[dictionary.size() + 1] = c;
  printf("0 %c\n", c);

  // Ahora hay que imprimir las salidas

  unsigned int index_bits = 1;

  while (pow(2, index_bits) < dictionary.size()) index_bits++;
  printf("ds: %d bits: %d\n", dictionary.size(), index_bits);

  for (unsigned int i = 0; i < outputs.size(); i++)
  {
     binarizer->addStringToCode(toBinaryString(outputs[i].first, index_bits).c_str());
     binarizer->addStringToCode(instant_codes[outputs[i].second].c_str());
  }

  fprintf(output, "%d %d\n", dictionary.size(), binarizer->getOffset());
  for (unsigned int i = 0; i < dictionary.size(); i++)
    fprintf(output, "%s\n", dictionary[i].c_str());

  std::string code = binarizer->getBinaryCode();

  fprintf(output, "%s", code.c_str());

  fclose(input);
  fclose(output);
}

void LempelZiv78Encoder::encode(char *inputFileName)
{
  getInstantCodes(inputFileName);
  doEncoding(inputFileName);
}

bool LempelZiv78Encoder::stringExistsInDict(std::string new_string)
{
  std::map < unsigned int, std::string >::iterator it;

  for (it = dictionary.begin(); it != dictionary.end(); it++)
    if (new_string == (*it).second) return true;
  return false;
}

unsigned int LempelZiv78Encoder::getIndexOfString(std::string new_string)
{
  std::map < unsigned int, std::string >::iterator it;

  for (it = dictionary.begin(); it != dictionary.end(); it++)
    if (new_string == (*it).second) return (*it).first;
}
