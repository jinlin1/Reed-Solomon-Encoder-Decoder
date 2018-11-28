#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
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
  vector<vector<int>> num;
  char c;
  
  // parses ints from strings
  /*
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
  */

  for (int j = 0; getline(infile,temp); j++) {
    for (int i = 0; i< temp.length(); i++) {
      num[j][i] = temp.at(i) - '0';
    }
  }
  
  infile.close();
  
  return 0;
}
