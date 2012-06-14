#include <cstdio>
#include <math.h>
#include "lempel_ziv78.h"

/**
 * This methods returns the successor char in the alphabet
 * {0,1} (binary alphabet).
 *
 * @param char_suc The character whose successor we want to get.
 */
char LempelZiv78Encoder::sucChar(char char_suc)
{
  if (char_suc == '1')
    return '0';
  return '1';
}

/**
 * This method returns the successor string in the alphabet {0,1}
 * for a given string.
 *
 * @param string_suc The string whose successor we want to get.

 */
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

/**
 * This method parses the file for the first time and gets the
 * number of symbols that appear. It also gets the instant codes
 * for each one of the symbols appeared, storing them on the
 * "instant_codes" attribute.
 *
 * @param inputFileName The name of the input file.
 */
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

  sort(alfabeto.begin(), alfabeto.end());

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

  for (int i = 0; i < n_simbolos; i++)
    printf("%c %s\n", alfabeto[i], instant_codes[alfabeto[i]].c_str());

  fclose(in);
}

/**
 * This method performs the real encoding process. It opens
 * the files for input and output, and then executes the LZ78
 * algorithm, writing the outputs in the output file.
 *
 * @param inputFileName The name of the input file.
 */
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
     //printf("%s %s\n", toBinaryString(outputs[i].first, index_bits).c_str(), instant_codes[outputs[i].second].c_str());
  }

  fprintf(output, "%d %d %d\n", dictionary.size(), binarizer->getCodeLength(), binarizer->getOffset());
  for (unsigned int i = 1; i <= dictionary.size(); i++)
    fprintf(output, "%d %s\n", dictionary[i].size(), dictionary[i].c_str());

  std::string code = binarizer->getBinaryCode();

  for (unsigned int i = 0; i < binarizer->getCodeLength(); i++)
    fprintf(output, "%c", code[i]);

  fclose(input);
  fclose(output);
}

/**
 * This method is a wrapper for performing the instant
 * codes obtention and then the encoding process.
 *
 * @param inputFileName The name of the input file.
 */
void LempelZiv78Encoder::encode(char *inputFileName)
{
  getInstantCodes(inputFileName);
  doEncoding(inputFileName);
}

/**
 * This method is a boolean method for knowing if the given
 * string is already in the dictionary.
 *
 * @param new_string The string we are looking for in the dictionary.
 */
bool LempelZiv78Encoder::stringExistsInDict(std::string new_string)
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
unsigned int LempelZiv78Encoder::getIndexOfString(std::string new_string)
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
void LempelZiv78Encoder::decode(char *inputFileName, char *outputFileName)
{
  FILE *input;
  FILE *output;
  unsigned int dict_size, n_chars, offset;
  std::vector < char > alfabeto;
  Debinarizer *debinarizer = new Debinarizer();

  input = fopen(inputFileName, "r");

  if (input == NULL)
  {
    printf("Fichero de entrada no encontrado.\n");
    exit(-1);
  }

  output = fopen(outputFileName, "w");
  
  if (output == NULL)
  {
    printf("Fichero de salida no pudo crearse.\n");
    exit(-1);
  }
  
  fscanf(input, "%d %d %d\n", &dict_size, &n_chars, &offset);
  
  for (unsigned int i = 0; i < dict_size; i++)
  {
    std::string tmp_entry;
    unsigned int n;
    char c;

    fscanf(input, "%d", &n);
    c = fgetc(input); // borrar blanco

    for (unsigned int j = 0; j < n; j++)
    {
      c = fgetc(input);
      tmp_entry.push_back(c);
    }

    c = fgetc(input); // borrar \n
    if (n == 1)
      alfabeto.push_back(tmp_entry[0]);
    dictionary[i + 1] = tmp_entry;
  }

  sort(alfabeto.begin(), alfabeto.end());

  unsigned int longitud = 1;
  std::vector < unsigned int > longitudes;
  std::vector < std::string > codificaciones;

  while (pow(2, longitud) < alfabeto.size()) longitud++;

  for (unsigned int i = 0; i < alfabeto.size(); i++)
    longitudes.push_back(longitud);

  std::string zero_vector;
  
  for (unsigned int i = 0; i < longitud; i++)
    zero_vector.push_back('0');

  codificaciones.push_back(zero_vector);

  for (unsigned int i = 1; i < alfabeto.size(); i++)
    codificaciones.push_back(sucStr(codificaciones[i - 1]));


  for (unsigned int i = 0; i < alfabeto.size(); i++)
    instant_codes[alfabeto[i]] = codificaciones[i];
 
  for (int i = 0; i < alfabeto.size(); i++)
    printf("%c %s\n", alfabeto[i], instant_codes[alfabeto[i]].c_str());

  // Hasta aqui ya se ha recuperado el alfabeto original y sus codigos instantaneos.

  unsigned int index_bits = 1;

  while (pow(2, index_bits) < dictionary.size()) index_bits++;
  
  unsigned int total_bits = index_bits + longitud;

  printf("ds: %d bits: %d total: %d\n", dictionary.size(), index_bits, total_bits);

  // lectura de binario

  debinarizer->setOffset(offset);

  for (unsigned int i = 0; i < n_chars; i++)
  {
    char c = fgetc(input);
    debinarizer->addCharToString(c);
  }

  while (debinarizer->codesLeft())
  {
    unsigned int cur_index;
    std::string cur_code;
    char cur_symbol;
    std::map < char, std::string >::iterator it;

    cur_index = binStringToInteger(debinarizer->getStringOfNBits(index_bits));
    cur_code = debinarizer->getStringOfNBits(longitud);

    for (it = instant_codes.begin(); it != instant_codes.end(); it++)
      if ((*it).second == cur_code)
      {
        cur_symbol = (*it).first;
        break;
      }

    if (cur_index != 0)
      fprintf(output, "%s%c", dictionary[cur_index].c_str(), cur_symbol);
    else if (cur_symbol != EOF)
      fprintf(output, "%c", cur_symbol);
    else 
      fprintf(output, "\0");

    /*while (cur_index != 0)
    {
      std::string tmp_entry = dictionary[cur_index];
      std::map < unsigned int, std::string >::iterator it;

      tmp_entry.erase(tmp_entry.size() - 1);

      for (it = dictionary.begin(); it != dictionary.end(); it++)
      {
        if ((*it).second == tmp_entry)
        {
          cur_index = (*it).first;
          cur_
        }
      }
    }*/

  }

}
