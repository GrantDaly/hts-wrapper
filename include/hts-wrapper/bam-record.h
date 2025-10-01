#pragma once
#include <iostream>
#include <vector>

#include "htslib/hts.h"
#include <htslib/sam.h>

#include "bam-header.h"

#include "cigar.h"
class BamRecord {
 private:  

  friend class SamFile;
 public:
  bam1_t * bam_ptr = nullptr;
  // todo want to have shared pointer to bam header
  // cigar vector should be vestigial for now. getCigar needs to calculate it as needed
  // from htslib representation
  std::vector<CigarOperation> cigar;


  // issue that default constructor does not generate the cigar vector.
  // maybe I should always parse this or at least check if empty, generate if so before returning
  BamRecord();
  // BamRecord(std::string qname,
  // 		uint16_t  	flag,
  // 	    std::string contig,
  // 		hts_pos_t  	pos,
  // 		uint8_t  	mapq,
  // 		Cigar  	cigar,
  // 	    std::string mate_contig,
  // 		hts_pos_t  	mpos,
  // 		hts_pos_t  	isize,
  // 	    std::string   	seq,
  // 	    std::string qual,
  // 	    std::string auxillary 
  // 	    );
  BamRecord(std::string qname,
		     uint16_t flag,
		     std::string contig,
		     hts_pos_t pos,
		     uint8_t mapq,
		     std::vector<CigarOperation> cigar_vector,
		     std::string contig_mate,
		     hts_pos_t pos_mate,
		     hts_pos_t insert_size,
 		     std::vector<char> seq,
		     std::vector<char> qual,
	    size_t l_aux, BamHeader & hdr);

  
  ~BamRecord();

  BamRecord(const BamRecord & in);

  int setQName(std::string qname);
  std::string getQName();

  int getTidNumber() const;
  int getMateTidNumber() const;
  hts_pos_t getPos() const;

  hts_pos_t getInsertSize() const;
  hts_pos_t getBamEndPos() const;
  hts_pos_t getQueryLength() const;
  hts_pos_t getReferenceLength() const;
  
  hts_pos_t getMatePos() const;
  uint16_t getFlag() const;

  std::vector<CigarOperation> getCigar() const;
  
  bool isReadPaired() const;
  bool isProperPair() const;
  bool isUnmapped() const;
  bool isMateUnmapped() const;
  bool isReadRevStrand() const;
  bool isMateRevStrand() const;
  bool isFirstInPair() const;
  bool isSecondInPair() const;
  bool isNotPrimaryAlign() const;
  bool failsVendorQC() const;
  bool isDuplicate() const;
  bool isSupplementalAlign() const;

  std::vector<char> getSequence() const;
  std::string readOrientationString() const;
  std::string getFlagStringHTS() const;
};
