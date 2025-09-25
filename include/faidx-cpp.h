#pragma once
#include <iostream>
#include<filesystem>
#include <map>

#include <htslib/hts.h>
#include <htslib/faidx.h>
class FastaWrapper {
 private:
  std::filesystem::path filename;
  faidx_t* fasta_ptr = nullptr;

  int num_seqs=-1;
  //std::map<std::string,int> names_and_indexes;
 public:
  //FastaWrapper() {}
  //FastaWrapper(std::string filename_string);
  FastaWrapper(std::filesystem::path filename_path);
  ~FastaWrapper();
  int getNumberContigs() const;
  bool hasContig(std::string contig) const;
  std::string getRegion(std::string region_str) const;
  std::string getRegion(std::string contig, hts_pos_t beginning, hts_pos_t end) const;
  std::string getContigNameByIndex(int contig_index) const;
  int getIndexByContigName(const std::string & contig_name) const;
  hts_pos_t getLengthByContig(std::string contig_name) const;
  
};
