#ifndef _LEMPEL_ZIV_77_H_
#define _LEMPEL_ZIV_77_H_

#include "lempel_ziv.h"

class LempelZiv77Encoder : public LempelZivEncoder
{
  unsigned int search_buffer_bits;
  unsigned int lookahead_buffer_bits;
  unsigned int search_buffer_size;
  unsigned int lookahead_buffer_size;
  char* sliding_window;
public:
  void encode();
};

#endif
