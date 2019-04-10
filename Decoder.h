/**
 * Decoder performs the reed solomon decoder algorithm
**/
#ifndef INCLUDE_DECODER_H
#define INCLUDE_DECODER_H

#include <string>
#include "GaloisFieldArithmetic/GaloisField.h"
#include "GaloisFieldArithmetic/GaloisFieldElement.h"
#include "GaloisFieldArithmetic/GaloisFieldPolynomial.h"
#include "Package.h"

class Decoder {
  public:
    Package decode(const unsigned int galois_field_exp,
     const vector<unsigned int> prim,
     const vector<unsigned int> gen,
     const vector<unsigned int> msg);

  private:
    galois::GaloisFieldElement summation(galois::GaloisField* gf,
      galois::GaloisFieldPolynomial connection_poly,
      galois::GaloisFieldElement* syndromes,
      unsigned int current_iteration,
      unsigned int len_lfsr);

};

#endif
