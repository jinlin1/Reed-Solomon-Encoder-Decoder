#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

//parses a string deliminated with ','
vector<unsigned int> parse(string instr) {
  vector<unsigned int> ret;
  stringstream ss(instr);
  string temp;
  while (ss.good()) {
    getline(ss,temp,',');
    ret.push_back(stoi(temp));
  }
  return ret;
}

//testing purposes: prints out content of a vector deliminated with '/'
void vecprint(vector<unsigned int> vec) {
  for (int i=0; i<vec.size(); i++) {
    cout << vec[i];
    if (i < vec.size()-1) cout << "/";
  } cout << endl;
}

//takes a single argument, a filename
//text file must follow a strict format:
// x - single digit int for the size of the galois field
// xxxx - a binary string signifying the primitive polynomial
// x,x,x,x - comma deliminated string signifying the generator polynomial
// x,x,x,x - comma deliminated string signifying the message
int main(int argc, char** argv){
  if (argc == 1) cout << "No File Specified." << endl;
  else if (argc < 2) cout << "Too Many Arguments Given." << endl;
 
  ifstream infile(argv[1]);
  string temp;
  
  //galois field size
  unsigned int gfe;
  if (getline(infile, temp)) gfe = temp.at(0) - '0';
  else { return -1; }
  
  //primitive polynomial
  vector<unsigned int> primpoly;
  if (getline(infile, temp)) {
    for (int i=0; i<temp.length(); i++) {
      primpoly.push_back(temp.at(i) - '0');
    }
  } else { return -1; }

  //generator polynomial
  getline(infile,temp);
  vector<unsigned int> genpoly = parse(temp);

  //message
  getline(infile,temp);
  vector<unsigned int> msg = parse(temp);

  infile.close();
}
