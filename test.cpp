#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "GaloisFieldArithmetic/GaloisField.h"
#include "GaloisFieldArithmetic/GaloisFieldElement.h"
#include "GaloisFieldArithmetic/GaloisFieldPolynomial.h"

using namespace std;


int main(){
  //  cout << "hello galois field world" << endl;
  //  cout << "Enter a message" << endl;
  ifstream infile("input.txt");
  string temp;
  stringstream ss;
  int num;
  while (getline(infile, temp)) {
    ss << temp;
    while (ss >> temp) {
      if (stringstream(temp) >> num) {
	cout << num << endl;
      } else {
	cout << temp << " is not an int" << endl;
      }
    }
    ss.clear();
  }

  infile.close();
  
  return 0;
}
