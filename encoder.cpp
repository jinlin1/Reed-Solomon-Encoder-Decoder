#include <iostream>
#include <string>
#include <sstream>
#include "GaloisFieldArithmetic/GaloisField.h"

int main() {
  /*
    p(x) = 1x^8+1x^7+0x^6+0x^5+0x^4+0x^3+1x^2+1x^1+1x^0
           1    1    0    0    0    0    1    1    1
  */
  unsigned int prim_poly[9] = {1,1,1,0,0,0,0,1,1};
  /*
    A Galois Field of type GF(2^8)
  */

  galois::GaloisField gf(8, prim_poly);

  return 0;
}
