#include "faidx-cpp.h"

FastaWrapper::FastaWrapper(std::string filename_p)
{
    filename = filename_p;
    // todo: check that not empty string and file suffix = ".fa",".fasta", etc.
    fasta_ptr = fai_load_format(filename.c_str(),
					  FAI_FASTA);
    num_seqs = faidx_nseq (fasta_ptr);
}

FastaWrapper::~FastaWrapper() { fai_destroy(fasta_ptr); }

int FastaWrapper::getNumberContigs() {return num_seqs;}

std::string FastaWrapper::getRegion(std::string region_str){
  const char* return_char = nullptr;
  hts_pos_t len = -1;
  if(! (return_char = fai_fetch64( fasta_ptr, region_str.c_str(), &len))){
    if (len == -1) {
      std::cout << "Failed to retreive " << region_str <<  std::endl;
      return std::string("");
    }
}
  auto out_str = std::string{return_char};
  // going ahead and freeing memory of the char*. Techincally inneficient but should prevent memory leakage
  free((void*)return_char);
  return out_str;
}

std::string FastaWrapper::getContigNameByIndex(int contig_index){
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
//std::cout << "hello world" << std::endl;
