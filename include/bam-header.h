#pragma once

#include "htslib/hts.h"
#include <iostream>

#include <htslib/sam.h>

//#include "sam-file.h"
#include "faidx-cpp.h"


class BamHeader {
 private:



 public:
  sam_hdr_t * hdr = nullptr;
  
  BamHeader();
  ~BamHeader();
  BamHeader(samFile* in_sam_file);
  int addSequenceHeaderEntry(std::string contig, hts_pos_t length);
  int addSequencesFromFasta(const FastaWrapper & f);
  int contigNameToTid(const std::string & contig);
  std::string contigNameToTid(int read_tid) const;
  int getNumEntriesByField(std::string field) const;

  // int write_header(samFile *fp) const;
};
