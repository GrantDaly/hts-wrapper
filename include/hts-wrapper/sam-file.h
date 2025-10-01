#pragma once
#include <filesystem>

#include <iostream>

#include <htslib/sam.h>

#include "htslib/hts.h"

#include "bam-header.h"
#include "bam-record.h"
//#include "faidx-cpp.h"

//class BamHeader;

class SamFile {
 private:

  
  
 public:
  samFile * fp = nullptr;
  hts_idx_t * index = nullptr; 
  SamFile(std::filesystem::path filename_path, std::string mode);
  ~SamFile();
  BamHeader read_header();
  int write_header(const BamHeader & header) const;
  int write_record(const BamHeader & header,
	    const BamRecord & rec) const;
};


// class BamHeader {
//  private:

//   sam_hdr_t * hdr = nullptr;
//   friend class SamFile;

//  public:

//   BamHeader();
//   ~BamHeader();
//   int addSequenceHeaderEntry(std::string contig, hts_pos_t length);
//   int addSequencesFromFasta(const FastaWrapper & f);
//   int getNumEntriesByField(std::string field) const;
// };
