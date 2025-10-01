#include "bam-header.h"
#include "htslib/hts.h"
#include "sam-file.h"
#include "bam-record.h"
#include <cstddef>
#include <optional>

class BamIterator {
 public:
  SamFile* file;
  BamHeader * header;
  hts_itr_t* iter = nullptr;
  BamIterator(SamFile *file_p, BamHeader * header_p,
			 const std::string &region_p);
  BamIterator( SamFile *file_p,
	       int tid_p, hts_pos_t beg_p, hts_pos_t end_p);
  ~BamIterator();
  std::optional<BamRecord> getNextRecord();
  std::optional<BamRecord> getRecordByName(std::string read_name_p);
};

BamIterator::BamIterator( SamFile *file_p, BamHeader * header_p,
			 const std::string &region_p)
{
  file = file_p;
  header = header_p;
  if(file->index == nullptr){
    std::cerr << "No Index for Bam File " << std::endl;
    std::exit(1);
  }
  iter = sam_itr_querys(file->index,
		 header->hdr, "chrM");
    if(iter == nullptr){
    std::cerr << "Error opening iterator for ChrM" << std::endl;
    std::exit(1);
  }
}

// todo could use header so I don't have to use int tid code

BamIterator::BamIterator( SamFile *file_p, 
			  int tid_p, hts_pos_t beg_p, hts_pos_t end_p)
{
  file = file_p;

  if(file->index == nullptr){
    std::cerr << "No Index for Bam File " << std::endl;
    std::exit(1);
  }
  iter = sam_itr_queryi(file->index,
			tid_p,beg_p, end_p);
    if(iter == nullptr){
    std::cerr << "Error opening iterator for ChrM" << std::endl;
    std::exit(1);
  }
}

BamIterator::~BamIterator() {
  sam_itr_destroy(iter);
}
			 

std::optional<BamRecord> BamIterator::getNextRecord()
{
  auto out_rec = BamRecord();
  int return_code = sam_itr_multi_next(file->fp, iter,out_rec.bam_ptr);
  if(return_code >= 0){
  return std::move(out_rec);
  }
  else if (return_code == -1){
    return std::nullopt;
  }
  else{
    std::cerr << "Error Reading Next Bam Record" << std::endl;
    std::exit(return_code);
  }
}

std::optional<BamRecord> BamIterator::getRecordByName(std::string read_name_p)
{
  auto out_rec = BamRecord();
  int return_code{-3};
  while((return_code = sam_itr_multi_next(file->fp, iter,out_rec.bam_ptr)) >= 0)
    {
      /* std::cout << " testing read code " << return_code <<  std::endl; */
      if(out_rec.getQName() == read_name_p){
	return std::move(out_rec);
      }
    }
      /* std::cout << " done  read code " << return_code <<  std::endl; */
  // todo: techincally more proper to check code for not found vs. error code. not distinguishing the two for now.
  // if made it past the here the read was not found
    return std::nullopt;
  }

