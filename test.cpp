#include <iostream>
#include <string>
#include <sstream>
#include "GaloisFieldArithmetic/GaloisField.h"
#include "GaloisFieldArithmetic/GaloisFieldElement.h"
#include "GaloisFieldArithmetic/GaloisFieldPolynomial.h"

using namespace std;

int main(){
  
  const unsigned int galois_field_exp = 3;
  unsigned int prim_poly[galois_field_exp+1] = {1,1,0,1};
  
  galois::GaloisField gf(galois_field_exp,prim_poly);

  galois::GaloisFieldElement element1(&gf, 1);
  galois::GaloisFieldElement element2(&gf, 2);
  galois::GaloisFieldElement element3(&gf, 3);
  galois::GaloisFieldElement element4(&gf, 4);
  galois::GaloisFieldElement element5(&gf, 5);
  galois::GaloisFieldElement element6(&gf, 6);
  galois::GaloisFieldElement element7(&gf, 7);

  std::cout << element1.inverse() << "\n";
  std::cout << element2.inverse() << "\n";
  std::cout << element3.inverse() << "\n";
  std::cout << element4.inverse() << "\n";
  std::cout << element5.inverse() << "\n";
  std::cout << element6.inverse() << "\n";
  std::cout << element7.inverse() << "\n";
  std::cout << element2 + element3 << "\n";

  std::cout << gf.index(4) << "\n";

}
