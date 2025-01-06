#pragma once

#include "htslib/hts.h"
#include <iostream>

#include <htslib/sam.h>
class BamRecord {
 private:
  std::string filename;
  bam1_t bam_ptr = nullptr;

 public:

  BamRecord(std::string filename_p);
  ~BamRecord();
  
};
