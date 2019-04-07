/**
 * Encoder performs the reed solomon encoder algorithm
**/
#ifndef INCLUDE_ENCODER_H
#define INCLUDE_ENCODER_H

#include <string>
#include "Package.h"

class Encoder {
  public:
    Package setup(const std::string& gfeStr,
    const std::string& primpolyStr,
    const std::string& genpolyStr,
    const std::string& msgStr);

  private:
    Package encode(const unsigned int galois_field_exp,
     const vector<unsigned int> prim,
     const vector<unsigned int> gen,
     const vector<unsigned int> msg);

};

#endif
