#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include "GaloisFieldArithmetic/GaloisField.h"
#include "GaloisFieldArithmetic/GaloisFieldElement.h"
#include "GaloisFieldArithmetic/GaloisFieldPolynomial.h"
using namespace std;

int encode(const unsigned int galois_field_exp,
	   const vector<unsigned int> prim,
	   const vector<unsigned int> gen,
	   const vector<unsigned int> msg) {
  const unsigned int codeword_length = pow(2,galois_field_exp) - 1;
  const unsigned int data_length = msg.size();
  const unsigned int parity_length = gen.size();

  // Primitive polynomial that acts on the individual symbols 
  // Important note: the far right number is the coefficient of the highest degree
  // while the far left is the coefficient of the lowest degree. 
  // The order is swapped from which it is written.
  // p(x) = 1x^3+0x^2+1x^1+1x^0
  //        1    0    1    1
  unsigned int prim_poly[prim.size()];
  for (int i=0; i < prim.size(); i++) {
    prim_poly[i] = prim.at(i);
  }
    
  // Define the galois field for the individual symbols
  galois::GaloisField gf(galois_field_exp, prim_poly);

  // Create individual symbols as an array of symbols
  galois::GaloisFieldElement msg_gfe[data_length];
  for (int i=0; i<data_length;i++) {
    msg_gfe[i] = galois::GaloisFieldElement(&gf, msg.at(i));
  }

  // Transform the array of symbols into a polynomial
  galois::GaloisFieldPolynomial msg_polynomial(&gf,data_length-1,msg_gfe);

  // Shift the polynomial by the parity length 
  msg_polynomial = msg_polynomial << parity_length;

  cout << "Shifted message polynomial: " << msg_polynomial << "\n";

 
  vector<galois::GaloisFieldElement(*)> gen_gfe;
  vector<galois::GaloisFieldPolynomial> gen_polynomial;

  for (int i=0; i < parity_length; i++) {
    galois::GaloisFieldElement gfei[2] = {galois::GaloisFieldElement(&gf, gen.at(i)),
					  galois::GaloisFieldElement(&gf, 1)};
    gen_gfe.push_back(gfei);
    gen_polynomial.push_back(galois::GaloisFieldPolynomial(&gf, 1, gen_gfe.at(i)));
    cout << "Generator polynomial term" << i+1 << ": " << gen_polynomial.at(i) << "\n";
  }

  galois::GaloisFieldPolynomial generator_polynomial = gen_polynomial.at(0);

  for (int i=1; i < gen_polynomial.size(); i++) {
    generator_polynomial *= gen_polynomial.at(i);
  }

  cout << "Generator polynomial: " << generator_polynomial << "\n";

  galois::GaloisFieldPolynomial parity_polynomial = msg_polynomial % generator_polynomial; 

  cout << "Parity polynomial: " << parity_polynomial << "\n";

  galois::GaloisFieldPolynomial message = msg_polynomial + parity_polynomial;

  cout << "Encoded message polynomial: " << message << "\n";

  int i;
  ofstream outfile("en_output.txt");
  for (i=0; i<=message.deg(); i++) {
    outfile << message[i];
  }
  if (i < codeword_length) {
    for (; i < codeword_length; i++) {
      outfile << "0";
    }
  }
  outfile.close();
  
  return 0;
}

int main() {
  ifstream infile("en_input.txt");
  string temp;
  vector<unsigned int> primpoly,genpoly,msg;
  unsigned int gfe;

  //galois field size
  if (getline(infile, temp)) {
    gfe = temp.at(0) - '0';
  } else { return -1; }

  //primitive polynomial
  if (getline(infile, temp)) {
    for (int i=0; i<temp.length(); i++) {
      primpoly.push_back(temp.at(i) - '0');
    }
  } else { return -1; }

  //generator polynomial
  if (getline(infile, temp)) {
    for (int i=0; i<temp.length(); i++) {
      genpoly.push_back(temp.at(i) - '0');
    }
  } else { return -1; }

  //the message
  if (getline(infile, temp)) {
    for (int i=0; i<temp.length(); i++) {
      msg.push_back(temp.at(i) - '0');
    }
  } else { return -1; }

  encode(gfe, primpoly, genpoly, msg);  

  infile.close();
  return 0;
}
