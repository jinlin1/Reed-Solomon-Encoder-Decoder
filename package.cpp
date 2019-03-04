#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "GaloisFieldArithmetic/GaloisField.h"
#include "GaloisFieldArithmetic/GaloisFieldElement.h"
#include "GaloisFieldArithmetic/GaloisFieldPolynomial.h"
using namespace std;

int main(){
  int i;
  ofstream outfile("en_output.txt");
  for (i=0; i<=message.deg(); i++) {
    outfile << message[i];
    if (i < codeword_length-1) outfile << ",";
  }
  if (i < codeword_length) {
    for (; i < codeword_length; i++) {
      outfile << "0";
      if (i < codeword_length-1) outfile << ",";
    }
  }
  outfile.close();
}
