#include <cstring>

#include "lempel_ziv.h"
#include "lempel_ziv77.h"

#define LZ77 0
#define LZ78 1
#define LZW 2

int main(int argc, char* argv[])
{
  int encoder_type = LZ77;
  LempelZivEncoder *encoder = 0;
  
  if (argc == 3)
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
    case LZ77:
      encoder = new LempelZiv77Encoder();
      break;
    /*case LZ78:
      encoder = new LempelZiv78Encoder();
      break;
    case LZW:
      encoder = new LempelZivWEncoder();
      break;
    */
  }

  encoder->encode();

  return 0;
}
