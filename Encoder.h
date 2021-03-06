/**
 * Encoder performs the reed solomon encoder algorithm
**/
#ifndef INCLUDE_ENCODER_H
#define INCLUDE_ENCODER_H

#include <string>
#include "Package.h"

class Encoder {
  public:
    Package encode(const unsigned int galois_field_exp,
     const vector<unsigned int> prim,
     const vector<unsigned int> gen,
     const vector<unsigned int> msg);

};

#endif
