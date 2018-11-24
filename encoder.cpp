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
  const unsigned int data_length = 3;
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

  // Create individual symbols as an array of symbols
  galois::GaloisFieldElement gfe[data_length] = {
    galois::GaloisFieldElement(&gf, 5),
    galois::GaloisFieldElement(&gf, 3),
    galois::GaloisFieldElement(&gf, 2)
  };

  // Transform the array of symbols into a polynomial
  galois::GaloisFieldPolynomial polynomial(&gf,data_length-1,gfe);

  // Shift the polynomial by the parity length 
  polynomial = polynomial << parity_length;

  std::cout << "Shifted message polynomial: " << polynomial << "\n";

  galois::GaloisFieldElement gfe1[2] = {
    galois::GaloisFieldElement(&gf, 2),
    galois::GaloisFieldElement(&gf, 1),
  };

  galois::GaloisFieldElement gfe2[2] = {
    galois::GaloisFieldElement(&gf, 4),
    galois::GaloisFieldElement(&gf, 1),
  };

  galois::GaloisFieldElement gfe3[2] = {
    galois::GaloisFieldElement(&gf, 3),
    galois::GaloisFieldElement(&gf, 1),
  };

  galois::GaloisFieldElement gfe4[2] = {
    galois::GaloisFieldElement(&gf, 6),
    galois::GaloisFieldElement(&gf, 1),
  };

  galois::GaloisFieldPolynomial polynomial1(&gf,1,gfe1);
  galois::GaloisFieldPolynomial polynomial2(&gf,1,gfe2);
  galois::GaloisFieldPolynomial polynomial3(&gf,1,gfe3);
  galois::GaloisFieldPolynomial polynomial4(&gf,1,gfe4);

  std::cout << "Generator polynomial term 1: " << polynomial1 << "\n";
  std::cout << "Generator polynomial term 2: " << polynomial2 << "\n";
  std::cout << "Generator polynomial term 3: " << polynomial3 << "\n";
  std::cout << "Generator polynomial term 4: " << polynomial4 << "\n";

  galois::GaloisFieldPolynomial generator_polynomial = (polynomial1 * polynomial2 * polynomial3 * polynomial4);

  std::cout << "Generator polynomial: " << generator_polynomial << "\n";

  galois::GaloisFieldPolynomial parity_polynomial = polynomial % generator_polynomial; 

  std::cout << "Parity polynomial: " << parity_polynomial << "\n";

  galois::GaloisFieldPolynomial message = polynomial + parity_polynomial;

  std::cout << "Encoded messgae polynomial: " << message << "\n";

  return 0;
}
