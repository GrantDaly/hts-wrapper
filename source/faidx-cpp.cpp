#include "faidx-cpp.h"

FastaWrapper::FastaWrapper(std::string filename_p)
{
    filename = filename_p;
    // todo: check that not empty string and file suffix = ".fa",".fasta", etc.
    fasta_ptr = fai_load_format(filename.c_str(),
					  FAI_FASTA);
}

FastaWrapper::~FastaWrapper() { fai_destroy(fasta_ptr); }

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

//std::cout << "hello world" << std::endl;
