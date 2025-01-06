#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "streams.hpp"


void filtered_read(std::istream& in)
{
    std::streambuf* orig = in.rdbuf();
    Gzipped buf(orig);
    in.rdbuf(&buf);
    for (char c; in.get(c);)
        std::cout << c;
    in.rdbuf(orig);
}

int main() {

  
  // std::string line;
  // while(std::getline(std::cin, line)){
  //   std::cout << line;
  // }

  // std::ifstream fin("data/random.txt");
  // Gzipped   gbuf(fin.rdbuf());
  // std::istream  in(&gbuf);
  // std::string line;
  // while(std::getline(in, line)){
  //   std::cout << line;
  // }
  // char a[] = "This is a test sentence to parse";
  // std::istringstream in(std::string(std::begin(a), std::end(a)));
  // std::streambuf* orig = in.rdbuf();
  // Gzipped buf(orig);
  // in.rdbuf(&buf);
    char a[] = "This is a test sentence to parse";
    std::istringstream in(std::string(std::begin(a), std::end(a)));
    filtered_read(in);
  
  
}
