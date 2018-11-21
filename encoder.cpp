#include <iostream>
#include <string>
#include <sstream>
#include <math.h> 
#include "GaloisFieldArithmetic/GaloisField.h"
#include "GaloisFieldArithmetic/GaloisFieldElement.h"
#include "GaloisFieldArithmetic/GaloisFieldPolynomial.h"

int main() {

  const unsigned int galois_field_exp = 3;
  const unsigned int codeword_length = pow(2,3) - 1;
  const unsigned int data_length = 4;
  unsigned int parity_length = codeword_length - data_length;

  // Primitive polynomial that acts on the individual symbols 
  unsigned int prim_poly[galois_field_exp+1] = {1,0,1,1};

  // Define the galois field for the individual symbols
  galois::GaloisField gf(galois_field_exp, prim_poly);

  // Create individual symbols as an array of symbols
  galois::GaloisFieldElement gfe[data_length] = {
    galois::GaloisFieldElement(&gf, 5),
    galois::GaloisFieldElement(&gf, 3),
    galois::GaloisFieldElement(&gf, 2),
    galois::GaloisFieldElement(&gf, 4)
  };

  // Transform the array of symbols into a polynomial
  galois::GaloisFieldPolynomial polynomial(&gf,data_length-1,gfe);

  // Shift the polynomial by the parity length 
  polynomial = polynomial << parity_length;

  std::cout << polynomial;

  return 0;
}
