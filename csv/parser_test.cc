#include "parser.h"

#include <iostream>
#include <string>
#include <vector>

using std::vector;
using std::string;

int main(int argc, char** argv) {
  vector<string> result;
  string s("abc, def , \"a, bc\" ");
  common::CSV::Parse(s, result);

  std::cout << s << std::endl;
  for (auto& i : result) {
    std::cout << i << " END" << std::endl;
  }

  if (argc >= 2) {
    string r;
    common::CSV::Parse(argv[1], &r);
    std::cout << r;
  }
  return 0;
}
