/**
 * Parse loads different polynomials from a file 
**/
#ifndef INCLUDE_PARSE_H
#define INCLUDE_PARSE_H

#include <string>
#include <vector>
using namespace std;

class Parse {
  public:
    Parse(string gfe, string primpoly, string genpoly, string msg);

    unsigned int getGfe();
    vector<unsigned int> getPrimpoly();
    vector<unsigned int> getGenpoly();
    vector<unsigned int> getMsg();
    vector<unsigned int> parseDelim(string instr);

  private:
    unsigned int gfe;
    vector<unsigned int> primpoly;
    vector<unsigned int> genpoly;
    vector<unsigned int> msg;

};

#endif
