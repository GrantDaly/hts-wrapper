#pragma once

#include "htslib/hts.h"
#include <iostream>

#include <htslib/sam.h>
class BamIterator {
 private:
  std::string filename;
  bam1_t bam_ptr = nullptr;
  samFile * inBam = sam_open(bamName.c_str(), "r");
  bam_hdr_t * bamHeader = sam_hdr_read(inBam);
  hts_idx_t * inBamIndex = bam_index_load(bamName.c_str());

 public:

  BamRecord(std::string filename_p);
  ~BamRecord();
  
};
