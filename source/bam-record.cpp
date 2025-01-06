#include "bam-record.h"
#include "cigar.h"
#include "htslib/hts.h"
#include "htslib/sam.h"

#include <iostream>
#include <sstream>

BamRecord::BamRecord() { bam_ptr = bam_init1(); }

BamRecord::BamRecord(std::string qname,
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
		     size_t l_aux,
		     BamHeader & hdr) {
  bam_ptr = bam_init1();
  // std::string -> size_t & cstring
  // uint16_t flag
  // reference by name, need a pointer or reference to get numeric contig
  // uint8_t mapq
  // cigar vector -> size_t & const uint32_t * array
  // mate tid as string -> int32_table
  // mate position hts_pos_table
  // insert size hts_pos_t
  // string for sequence -> size_t and c_string
  // figure out quality score, probably numbers internally and convert to char by adding ascii
  // length of auxillary in size_t

  // convert vector of cigar operations to string.
  std::stringstream cigar_string{""};
  for(auto const & i: cigar_vector){
    cigar_string << i ;
  }

  // read contig number
  int contig_int = hdr.contigNameToTid(contig);
  int contig_mate_int = hdr.contigNameToTid(contig_mate);
  
  char* cigar_end_addr = nullptr;
  uint32_t * cigar_buf = nullptr;
  size_t cigar_size = 0;
  int num_cigar = -1;
  if((num_cigar =sam_parse_cigar(cigar_string.str().c_str(), & cigar_end_addr,& cigar_buf,
				 & cigar_size)) > 0){};

  auto contigNumber = 
  bam_set1(bam_ptr, qname.length(), qname.c_str(), flag, contig_int, pos, mapq, num_cigar,
           cigar_buf, contig_mate_int, pos_mate, seq.size(), qual.size(),
	   seq.data(), qual.data(), 0);

  };
BamRecord::~BamRecord() { bam_destroy1(bam_ptr); }

BamRecord::BamRecord(const BamRecord & in) {
 this->bam_ptr = bam_dup1(in.bam_ptr);

  }

int BamRecord::setQName(std::string qname) {
  return bam_set_qname(bam_ptr,	qname.c_str());
}
  std::string BamRecord::getQName() {
  return std::string(bam_get_qname(bam_ptr));
}


int BamRecord::getTidNumber() const
{
  return bam_ptr->core.tid;
}
int BamRecord::getMateTidNumber() const {
return bam_ptr->core.mtid;
}
hts_pos_t BamRecord::getPos() const {
  return bam_ptr->core.pos;
}

hts_pos_t BamRecord::getInsertSize() const { return bam_ptr->core.isize; }
// htslib version returns 1 after end of alignment in 0-based coords, so I'm subtracting 1
hts_pos_t BamRecord::getBamEndPos() const { return bam_endpos(bam_ptr) -1; }

// l_qseq
hts_pos_t BamRecord::getQueryLength() const { return bam_cigar2qlen(bam_ptr->core.n_cigar, bam_get_cigar(bam_ptr)); }
hts_pos_t BamRecord::getReferenceLength() const { return bam_cigar2rlen(bam_ptr->core.n_cigar, bam_get_cigar(bam_ptr)); }
//bam_cigar2rlen()
hts_pos_t BamRecord::getMatePos() const { return bam_ptr->core.mpos; }

uint16_t BamRecord::getFlag() const {
  return bam_ptr->core.flag;
}

std::vector<CigarOperation> BamRecord::getCigar() const {
  std::vector<CigarOperation> outCigar;
  uint32_t* cigar_raw = bam_get_cigar(bam_ptr);
  for(auto i{0}; i< bam_ptr->core.n_cigar; i++){
    outCigar.push_back(CigarOperation(cigar_raw[i]));
  }
  return outCigar;
}
// #define 	BAM_FPAIRED   1
 
// #define 	BAM_FPROPER_PAIR   2
 
// #define 	BAM_FUNMAP   4
 
// #define 	BAM_FMUNMAP   8
 
// #define 	BAM_FREVERSE   16
 
// #define 	BAM_FMREVERSE   32
 
// #define 	BAM_FREAD1   64
 
// #define 	BAM_FREAD2   128
 
// #define 	BAM_FSECONDARY   256
 
// #define 	BAM_FQCFAIL   512
 
// #define 	BAM_FDUP   1024
 
// #define 	BAM_FSUPPLEMENTARY   2048
  bool BamRecord::isReadPaired() const {return (bam_ptr->core.flag & BAM_FPAIRED) != 0;}
  bool BamRecord::isProperPair() const {return (bam_ptr->core.flag & BAM_FPROPER_PAIR) != 0;}
  bool BamRecord::isUnmapped() const {return (bam_ptr->core.flag & BAM_FUNMAP) != 0;}
  bool BamRecord::isMateUnmapped() const {return (bam_ptr->core.flag & BAM_FMUNMAP) != 0;}
  bool BamRecord::isReadRevStrand() const {return (bam_ptr->core.flag & BAM_FREVERSE) != 0;}
  bool BamRecord::isMateRevStrand() const {return (bam_ptr->core.flag & BAM_FMREVERSE) != 0;}
  bool BamRecord::isFirstInPair() const {return (bam_ptr->core.flag & BAM_FREAD1) != 0;}
  bool BamRecord::isSecondInPair() const {return (bam_ptr->core.flag & BAM_FREAD2) != 0;}
  bool BamRecord::isNotPrimaryAlign() const {return (bam_ptr->core.flag & BAM_FSECONDARY) != 0;}
  bool BamRecord::failsVendorQC() const {return (bam_ptr->core.flag & BAM_FQCFAIL) != 0;}
  bool BamRecord::isDuplicate() const {return (bam_ptr->core.flag & BAM_FDUP) != 0;}
  bool BamRecord::isSupplementalAlign() const {
    return (bam_ptr->core.flag & BAM_FSUPPLEMENTARY) != 0;
  }

std::vector<char> BamRecord::getSequence() const {
  int32_t query_length = bam_ptr->core.l_qseq;
  std::vector<char> outVec(query_length);
  const auto seq_raw = bam_get_seq(bam_ptr);
  int temp_char_raw{-1};
  char out_char{0};
  for(auto i{0}; i<query_length; i++){
    temp_char_raw =  bam_seqi(seq_raw, i);
    switch(temp_char_raw){
    case 1:
      out_char = 'A';
      break;
    case 2:
    out_char = 'C';
      break;
    case 4:
      out_char = 'G';
      break;
    case 8:
      out_char = 'T';
      break;
    case 15:
      out_char = 'N';
      break;
    default:
      out_char = '?';
      break;
    }
    outVec[i] = out_char;
      }
  return std::move(outVec);
}

std::string BamRecord::readOrientationString() const
{
  std::string out_string{};
  if(this->isReadPaired() && this->isProperPair())
    {
      //todo fill in later. for now focussing on improper pairs
      ;
    }
  else{
    // todo: want to add specific is second read and is forward strand functions b/c easier to read
    if(this->isFirstInPair()){
      out_string += "Read1";
	}
    else if(!this->isFirstInPair()){
      out_string += "Read2";
	}
    
    if(this->isReadRevStrand()){
	out_string += "(-)";
      }
      else if(! this->isReadRevStrand()){
	out_string += "(+)";
      }
  }
  return out_string;
}

std::string BamRecord::getFlagStringHTS() const {
  std::string outStr{bam_flag2str(this->getFlag())};
  return outStr;
}

