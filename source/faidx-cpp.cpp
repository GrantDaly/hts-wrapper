#include "faidx-cpp.h"
#include "htslib/hts.h"

// FastaWrapper::FastaWrapper(std::string filename_string){
//   std::filesystem::path filename_path{filename_string};
//   FastaWrapper(filename_path);
// }
FastaWrapper::FastaWrapper(std::filesystem::path filename_path)
{
    filename = filename_path;
    // todo: if there are | separators, there also needs to be a space, or else samtools index includes the comment text in contig name
    // todo: check that not empty string and file suffix = ".fa",".fasta", etc.
    fasta_ptr = fai_load_format(filename.c_str(),
					  FAI_FASTA);
    if(fasta_ptr == nullptr){
      std::cerr << "Failed to open Fasta Index" << std::endl;
      std::exit(1);
    }
    num_seqs = faidx_nseq (fasta_ptr);
}

FastaWrapper::~FastaWrapper() { fai_destroy(fasta_ptr); }

int FastaWrapper::getNumberContigs() const {return num_seqs;}

std::string FastaWrapper::getRegion(std::string contig) const {
  if(! hasContig(contig)){
   return "";
    }
  
  const char* return_char = nullptr;
  hts_pos_t len = -1;
  if(! (return_char = fai_fetch64( fasta_ptr, contig.c_str(), &len))){
    if (len == -1) {
      std::cerr << "Failed to retreive " << contig <<  std::endl;
      return std::string("");
    }
}
  auto out_str = std::string{return_char};
  // going ahead and freeing memory of the char*. Techincally inneficient but should prevent memory leakage
  free((void*)return_char);
  return out_str;
}
bool FastaWrapper::hasContig(std::string contig) const {
  int seq_present = faidx_has_seq(fasta_ptr, contig.c_str());
  if(seq_present == 1){
    return true;
  }
  else {
    return false;
  }
}

std::string
FastaWrapper::getRegion(std::string contig, hts_pos_t beginning, hts_pos_t end) const
{
  if(! hasContig(contig)){
   return "";
    }

  const char* return_char = nullptr;
  int length = -1;
  return_char = faidx_fetch_seq (fasta_ptr, contig.c_str(),beginning,end, &length);
  if (length < 0) {
    std::cerr << "Failed to retreive " << contig << ":" << beginning << "-" << end <<  std::endl;
      return std::string("");
    }
  auto out_str = std::string{return_char};
  // going ahead and freeing memory of the char*. Techincally inneficient but should prevent memory leakage
  free((void*)return_char);
  return out_str;
  }
  
std::string FastaWrapper::getContigNameByIndex(int contig_index) const{
  // note: expects 0-indexed
  if((contig_index < 0) || (contig_index > num_seqs-1)){
    return std::string{"Invalid"};
  }
  else{
    std::string contig_string = "";
    if(const char* seq_char =  faidx_iseq (fasta_ptr, contig_index))
    {
      contig_string  = std::string{seq_char};
    }
    else
      {
	std::cerr << "Invalid Contig Number" << std::endl;
	contig_string = "Invalid";
      }
    return contig_string;
  }
  }

int FastaWrapper::getIndexByContigName(const std::string & contig_name) const {
  std::string cand_contig{};
  for(auto i=0; i<getNumberContigs();i++){
    cand_contig = (std::string) faidx_iseq (fasta_ptr, i);
    if(cand_contig == contig_name)
      return i;
  }
  // not found
  return -1;
}

hts_pos_t FastaWrapper::getLengthByContig(std::string contig_name) const {
  if( hasContig(contig_name)){
   return faidx_seq_len (fasta_ptr, contig_name.c_str());
    }
    else {
      return -1;
    }
 }

 
