#ifndef _LEMPEL_ZIV_H_
#define _LEMPEL_ZIV_H_

class LempelZivEncoder
{
public:
  virtual void encode(char *inputFileName) = 0;
};

#endif
