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
    unsigned int current_iteration,
    unsigned int len_lfsr) {

  // Initialize the discrepancy to be the syndrome at k
  galois::GaloisFieldElement discrepancy = syndromes[current_iteration];

  // Return discrepancy if length of lfsr is less than 1
  if(len_lfsr < 1) {
    return discrepancy;
  }

  // Perform the summation
  for(int i = 1; i <= len_lfsr; i++) {
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
    galois::GaloisFieldElement(&gf, 3),
    galois::GaloisFieldElement(&gf, 5),
    galois::GaloisFieldElement(&gf, 3),
    galois::GaloisFieldElement(&gf, 2),
    galois::GaloisFieldElement(&gf, 4),
  };

  // Transform the array of symbols into a polynomial
  galois::GaloisFieldPolynomial polynomial(&gf,codeword_length-1,gfe);

  std::cout << "Received polynomial: " << polynomial << "\n";

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
    current_discrepancy = summation(&gf, connection_poly, syndromes, k, len_lfsr); 
    std::cout << "Current discrepancy: " << current_discrepancy << "\n";
    // No change in polynomial
    if(current_discrepancy.poly() == 0) {
      std::cout << "  Detected no change in poly" << "\n";
      amount_shift = amount_shift + 1;
    } else {
      if((2*len_lfsr) > k) {
        std::cout << "  No length change" << "\n";
        gfe_poly = galois::GaloisFieldPolynomial(&gf, 0, initial_val);
        gfe_poly <<= amount_shift;
        gfe_poly[amount_shift] *= (current_discrepancy * prev_discrepancy.inverse());
        connection_poly = connection_poly + (gfe_poly * prev_connection_poly);

        amount_shift = amount_shift + 1;
      } else {
        std::cout << "  Update with length change" << "\n";
        tmp_gfe_poly = galois::GaloisFieldPolynomial(connection_poly);
        gfe_poly = galois::GaloisFieldPolynomial(&gf, 0, initial_val);
        gfe_poly <<= amount_shift;
        gfe_poly[amount_shift] *= (current_discrepancy * prev_discrepancy.inverse());
        connection_poly = connection_poly + (gfe_poly * prev_connection_poly);
        len_lfsr = k + 1 - len_lfsr;
        prev_connection_poly = tmp_gfe_poly;
        prev_discrepancy = current_discrepancy;
        amount_shift = 1;
      }
    }
    std::cout << "Connection polynomial after iteration " << k << ": " << connection_poly << "\n";
  }

  // Connection polynomial after BM Algoritm is the error locator polynomial
  std::cout << "Error locator polynomial: " << connection_poly << "\n";

  unsigned int root = 0;
  galois::GaloisFieldElement error_locator_root;
  galois::GaloisFieldElement error_locator_roots[connection_poly.deg()];

  // Perform Chien search brute force algorithm to find all possible roots for the connection polynomial
  for(int i = 1; i <= codeword_length; i++) {
    error_locator_root = connection_poly(galois::GaloisFieldElement(&gf, i)); 
    std::cout << "Chien search " << i << " :" << error_locator_root << "\n";
    if(error_locator_root.poly() == 0) {
      error_locator_roots[root] = galois::GaloisFieldElement(&gf, i);
      std::cout << "Found a root : " << error_locator_roots[root] << "\n";
      root = root + 1;
    }
  }

  // Generate error evaluator polynomial / error magnitude polynomial
  // Same concept but different names depending on textbook
  galois::GaloisFieldPolynomial gfe_poly2 = galois::GaloisFieldPolynomial(&gf, 0, initial_val);
  gfe_poly2 <<= parity_length;
  galois::GaloisFieldPolynomial syndromes_poly(&gf, parity_length-1, syndromes);
  galois::GaloisFieldPolynomial error_evaluator_poly = (syndromes_poly * connection_poly) % gfe_poly2;
  std::cout << "Error evaluator polynomial: " << error_evaluator_poly << "\n";

  // Perform the Forney Algorithm
  galois::GaloisFieldPolynomial connection_deriv_poly = connection_poly.derivative();
  std::cout << "Derivative of Error locator polynomial: " << connection_deriv_poly << "\n"; 


  galois::GaloisFieldPolynomial error_poly;
  galois::GaloisFieldPolynomial error_poly_term;
  galois::GaloisFieldElement error_magnitude[connection_poly.deg()];
  for(int i = 0; i < root; i++) {
    // Calculate the magnitude of the error
    error_magnitude[i] = (error_evaluator_poly(error_locator_roots[i])) / connection_deriv_poly(error_locator_roots[i]) ;
    std::cout << "Error magnitude value: " << error_magnitude[i] << "\n"; 
    error_poly_term = galois::GaloisFieldPolynomial(&gf, 0, initial_val);
    error_poly_term <<= gf.index(error_locator_roots[i].inverse());
    error_poly_term[gf.index(error_locator_roots[i].inverse())] *= error_magnitude[i];


    // Generate the error polynomial
    if(i == 0) {
      error_poly = error_poly_term;
    } else {
      error_poly += error_poly_term;
    }

    std::cout << "Error polynomial term: " << error_poly_term << "\n";
  }

  // Add the error polynomial to the message polynomial to get the original encoded message
  std::cout << "Error polynomial: " << error_poly << "\n";
  std::cout << "Decoded message: " << polynomial + error_poly << "\n"; 
  
  return 0;
}
