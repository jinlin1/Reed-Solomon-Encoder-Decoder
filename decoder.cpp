#include <iostream>
#include <string>
#include <sstream>
#include <math.h> 
#include "GaloisFieldArithmetic/GaloisField.h"
#include "GaloisFieldArithmetic/GaloisFieldElement.h"
#include "GaloisFieldArithmetic/GaloisFieldPolynomial.h"

// Summation function to be used in Berlekamp Massey algorithm
// Take the summation of the 
// kth connection poly element multipled by the kth - ith sequence value
//
// @param  gf The Galois Field that is used for all elements
// @param  connection_poly Current connection polynomial
// @param  syndromes Array of the syndromes
// @param  current_iteration Current iteration of the Berlekamp Massey algorithm
galois::GaloisFieldElement summation(
    galois::GaloisField* gf,
    galois::GaloisFieldPolynomial connection_poly, 
    galois::GaloisFieldElement* syndromes,
    unsigned int current_iteration) {

  // If the current iteration is less than 1,
  // return a 0 galois field element
  if(current_iteration < 1) {
    return galois::GaloisFieldElement(gf, 0);
  }

  // Initialize the discrepancy to be the syndrome at k
  galois::GaloisFieldElement discrepancy = syndromes[current_iteration];

  // Perform the summation
  for(int i = 1; i <= current_iteration; i++) {
    discrepancy = discrepancy + (connection_poly[i] * syndromes[current_iteration-i]);
  }

  return discrepancy;

}

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
    galois::GaloisFieldElement(&gf, 3),
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
    std::cout << "Syndrome " << i << " :" << syndromes[i] << "\n";
  }

  // Berlekamp Messay Algorithm implementation
  galois::GaloisFieldElement initial_val[1] = {
    galois::GaloisFieldElement(&gf, 1)
  };
  unsigned int len_lfsr = 0; // L
  unsigned int amount_shift = 1; // l
  galois::GaloisFieldElement prev_discrepancy(&gf, 1); // d_m
  galois::GaloisFieldElement current_discrepancy; // d
  galois::GaloisFieldPolynomial connection_poly(&gf, 0, initial_val); // c(x)
  galois::GaloisFieldPolynomial prev_connection_poly(&gf, 0, initial_val); // p(x)
  galois::GaloisFieldPolynomial  gfe_poly;
  galois::GaloisFieldPolynomial  tmp_gfe_poly;

  for(int k = 0; k < parity_length; k++) {
    std::cout << "Berlekamp Massey Iteration " << k << "\n";
    current_discrepancy = syndromes[k] + summation(&gf, connection_poly, syndromes, k); 
    std::cout << "Current discrepancy: " << current_discrepancy << "\n";
    // No change in polynomial
    if(current_discrepancy.poly() == 0) {
      std::cout << "  Detected no change in poly" << "\n";
      amount_shift = amount_shift + 1;
    } else {
      if(2*len_lfsr >= k) {
        std::cout << "  No length change" << "\n";
        gfe_poly = galois::GaloisFieldPolynomial(&gf, 0, initial_val);
        gfe_poly <<= amount_shift;
        gfe_poly[amount_shift] *= (current_discrepancy * prev_discrepancy.inverse());
        connection_poly = connection_poly - (gfe_poly * prev_connection_poly);

        amount_shift = amount_shift + 1;
      } else {
        std::cout << "  Update with length change" << "\n";
        tmp_gfe_poly = galois::GaloisFieldPolynomial(connection_poly);
        gfe_poly = galois::GaloisFieldPolynomial(&gf, 0, initial_val);
        gfe_poly <<= amount_shift;
        gfe_poly[amount_shift] *= (current_discrepancy * prev_discrepancy.inverse());
        connection_poly = connection_poly - (gfe_poly * prev_connection_poly);
        len_lfsr = k - len_lfsr;
        prev_connection_poly = tmp_gfe_poly;
        prev_discrepancy = current_discrepancy;
        amount_shift = 1;
      }
    }
  }

  std::cout << "Connection polynomial after BM Algorithm: " << connection_poly << "\n";

  return 0;
}
