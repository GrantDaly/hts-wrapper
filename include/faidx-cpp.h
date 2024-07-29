#pragma once

#include <iostream>

#include <htslib/faidx.h>
class FastaWrapper {
 private:
  std::string filename;
  faidx_t* fasta_ptr = nullptr;

  int num_seqs=-1;
 public:
  //FastaWrapper() {}
  FastaWrapper(std::string filename_p);
  ~FastaWrapper();
  int getNumberContigs();
  std::string getRegion(std::string region_str);
  std::string getContigNameByIndex(int contig_index);
  
};
