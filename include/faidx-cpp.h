//#ifndef FAIDX_H
// #define FAIDX_H
#pragma once

#include <iostream>

#include <htslib/faidx.h>
class FastaWrapper {
 private:
  std::string filename;
  faidx_t* fasta_ptr = nullptr;
 public:
  //FastaWrapper() {}
  FastaWrapper(std::string filename_p);
  ~FastaWrapper();
  std::string getRegion(std::string region_str);
};
