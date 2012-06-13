#include <cstring>

#include "lempel_ziv.h"
//#include "lempel_ziv77.h"
#include "lempel_ziv78.h"
#include "lempel_zivW.h"

#define LZ77 0
#define LZ78 1
#define LZW 2

int main(int argc, char* argv[])
{
  int encoder_type = LZ78;
  LempelZivEncoder *encoder = 0;
  
  if (argc == 4)
  {
    if (!strcmp(argv[1], "-lz77"))
      encoder_type = LZ77;
    if (!strcmp(argv[1], "-lz78"))
      encoder_type = LZ78;
    if (!strcmp(argv[1], "-lzw"))
      encoder_type = LZW;
  }

  switch (encoder_type)
  {
    /*case LZ77:
      encoder = new LempelZiv77Encoder();
      break;*/
    case LZ78:
      encoder = new LempelZiv78Encoder();
      break;
    case LZW:
      encoder = new LempelZivWEncoder();
      break;
  }

  if (argc == 3)
    encoder->decode(argv[1], argv[2]);
  else
    encoder->decode(argv[2], argv[3]);

  return 0;
}
