#include <iostream>
#include <string>
#include <sstream>
#include "GaloisFieldArithmetic/GaloisField.h"
#include "GaloisFieldArithmetic/GaloisFieldElement.h"
#include "GaloisFieldArithmetic/GaloisFieldPolynomial.h"

int main() {

  const unsigned int symbol_length = 8;
  const unsigned int codeword_length = 4;
  const unsigned int data_length = 2;
  unsigned int parity_length = codeword_length - data_length;

  // Primitive polynomial that acts on the individual symbols 
  unsigned int prim_poly[symbol_length+1] = {1,1,1,0,0,0,0,1,1};

  // Define the galois field for the individual symbols
  galois::GaloisField gf(symbol_length, prim_poly);

  // Create individual symbols as an array of symbols
  galois::GaloisFieldElement gfe[data_length] = {
    galois::GaloisFieldElement(&gf, 255),
    galois::GaloisFieldElement(&gf, 35)
  };

  // Transform the array of symbols into a polynomial
  galois::GaloisFieldPolynomial polynomial(&gf,data_length-1,gfe);

  // Shift the polynomial by the parity length 
  polynomial = polynomial << parity_length;

  std::cout << polynomial;

  return 0;
}
