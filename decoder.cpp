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
  // Important note: the far right number is the coefficient of the highest degree
  // while the far left is the coefficient of the lowest degree. 
  // The order is swapped from which it is written.
  // p(x) = 1x^3+0x^2+1x^1+1x^0
  //        1    0    1    1
  unsigned int prim_poly[galois_field_exp+1] = {1,1,0,1};

  // Define the galois field for the individual symbols
  galois::GaloisField gf(galois_field_exp, prim_poly);

  // Create received message 
  galois::GaloisFieldElement gfe[codeword_length] = {
    galois::GaloisFieldElement(&gf, 6),
    galois::GaloisFieldElement(&gf, 5),
    galois::GaloisFieldElement(&gf, 6),
    galois::GaloisFieldElement(&gf, 5),
    galois::GaloisFieldElement(&gf, 3),
    galois::GaloisFieldElement(&gf, 2),
    galois::GaloisFieldElement(&gf, 4),
  };

  // Transform the array of symbols into a polynomial
  galois::GaloisFieldPolynomial polynomial(&gf,codeword_length-1,gfe);

  // Create the alpha values used to generate generator polynomia
  galois::GaloisFieldElement generator_poly_alphas[parity_length] = {
    galois::GaloisFieldElement(&gf, 2),
    galois::GaloisFieldElement(&gf, 4),
    galois::GaloisFieldElement(&gf, 3),
  };

  galois::GaloisFieldElement syndromes[parity_length];

  // Calculate the syndrome polynomial
  // Evaluate the polynomial at each alpha used in the generator polynomial
  for(int i = 0; i < parity_length; i++) {
    syndromes[i] = polynomial(generator_poly_alphas[i]); 
    std::cout << syndromes[i] << "\n";
  }

  return 0;
}
