#include "bam-header.h"
//#include "sam-file.h"
#include "htslib/sam.h"



BamHeader::BamHeader() { hdr = sam_hdr_init(); }

BamHeader::BamHeader(samFile* in_sam_file){
      hdr = sam_hdr_read(in_sam_file);
  }
BamHeader::~BamHeader() {sam_hdr_destroy(hdr);}


int BamHeader::addSequenceHeaderEntry(std::string contig, hts_pos_t length){

  std::string out = "@SQ\tSN:" + contig + "\tLN:" + std::to_string(length);
  if (sam_hdr_add_lines(hdr,
			out.c_str(),1000) < 0){
    std::cerr << "Could not write Sam Header Line" << std::endl;
    std::exit(-1);
  }
  return 0;
}

int BamHeader::addSequencesFromFasta(const FastaWrapper & f){
  std::string name{""};
  hts_pos_t length = -1;
  for(int i=0; i<f.getNumberContigs();i++)
    {
      name = f.getContigNameByIndex(i);
      length = f.getLengthByContig(name);
      addSequenceHeaderEntry(name, length);
    }
  return 0;
}

int BamHeader::contigNameToTid(const std::string & contig){

  int contig_code = sam_hdr_name2tid( hdr,
		    contig.c_str());
    if(contig_code >= 0){
    return contig_code;
     }
  else if (contig_code == -1){
    // need to add to header
    std::cerr << "Have not implemented ability to add new contigs to header on the fly. Assume header knows all potential contigs" << std::endl;
    std::exit(1);
  }
  else if (contig_code == -2){
    // error
    std::cerr << "Error Retreiving contig code from bam header" << std::endl;
    std::exit(1);
  }
  else{
    // error
    std::cerr << "Unexpected Error Retreiving contig code from bam header" << std::endl;
    std::exit(1);
  }
}

std::string BamHeader::contigNameToTid(int read_tid) const {
  const char* raw_string = sam_hdr_tid2name(hdr,read_tid);
  if(raw_string != nullptr) {
    return std::string(raw_string);
  }
  else {
    std::cerr << "Error finding contig name by contig ID" << std::endl;
    std::exit(1);
  }
}

int BamHeader::getNumEntriesByField(std::string field) const{
  return sam_hdr_count_lines(hdr, field.c_str());
}

// int BamHeader::write_header(samFile *fp) const {
//   int error_code = sam_hdr_write(fp, hdr);
//   if(error_code != 0)
//     {std::cerr << "failed to write sam header" << std::endl;
//       std::exit(1);
// 	}
//   return error_code;
// }
