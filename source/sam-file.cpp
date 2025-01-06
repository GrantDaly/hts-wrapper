#include "bam-header.h"
#include "bam-record.h"
#include "htslib/hts.h"
#include "htslib/sam.h"

#include "sam-file.h"
// #include "bam-header.h"

SamFile::SamFile(std::filesystem::path filename_path, std::string mode){

  fp = sam_open(filename_path.c_str(), mode.c_str());
  index =  sam_index_load(fp,filename_path.c_str());
}

SamFile::~SamFile() {sam_close(fp);}

BamHeader SamFile::read_header() {
  return BamHeader(fp);
}

int SamFile::write_header(const BamHeader & header) const {
  int error_code = sam_hdr_write(fp, header.hdr);
  if(error_code != 0)
    {std::cerr << "failed to write sam header" << std::endl;
      std::exit(1);
	}
  return error_code;
}

int SamFile::write_record(const BamHeader & header,
			  const BamRecord & rec) const {
  return sam_write1(fp, header.hdr, rec.bam_ptr);
  
}
