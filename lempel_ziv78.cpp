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
  FILE *in = fopen(inputFileName, "r");
  char c;
  std::string x("");

  c = fgetc(in);

  while (c != EOF)
  {
    if (stringExistsInDict(x + c))
      x = x + c;
    else
    {
      if (stringExistsInDict(x))
      {
        printf("%d %c ", getIndexOfString(x), c);
        printf(" -- %d -- %s\n", dictionary.size() + 1, (x + c).c_str()); 
        dictionary[dictionary.size() + 1] = x + c;
        
      }
      else
      {
        dictionary[dictionary.size() + 1] = c;
        printf("0 %c\n", c);
      }
      x = "";
    }
    c = fgetc(in);
  }
  
  if (x.size() != 0)
  {
    // emitimos la salida correspondiente a
    printf("%d ", getIndexOfString(x));
    printf(" -- ?? -- %s\n", x.c_str()); 
  }

  // guardamos eof en dicc. y imprimimos su entrada.
  dictionary[dictionary.size() + 1] = c;
  printf("0 %c\n", c);


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
