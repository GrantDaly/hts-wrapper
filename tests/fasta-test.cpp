//#include <htslib/sam.h>
#include <iostream>
#include <string>
# include "faidx-cpp.h"
#include "faidx-cpp.h"

int main() {

std::string refName = "data/test1.fasta";
 std::cout << "Fasta Filename: " << refName << std::endl;
// faidx_t* fasta_idx = fai_load3_format(refName.c_str(),
// 					  nullptr, nullptr, FAI_CREATE, FAI_FASTA);
faidx_t* fasta_idx = fai_load_format(refName.c_str(),
					  FAI_FASTA);

 const char* chr1_name = "chr1";
 //int* tid_chr1 = (int*) malloc(sizeof(int)) ;
 //hts_pos_t* beg_chr1 = (hts_pos_t*) malloc(sizeof(hts_pos_t));
 //hts_pos_t* end_chr1 = (hts_pos_t*) malloc(sizeof(hts_pos_t));
 int tid_chr1 = -1;
 hts_pos_t beg_chr1 = 0, end_chr1=0, len=-1;


 const char* chr1_char;
 // while((chr1_char = fai_parse_region( fasta_idx, chr1_name, &tid_chr1, &beg_chr1, &end_chr1, HTS_PARSE_THOUSANDS_SEP))){
 if(! (chr1_char = fai_fetch64( fasta_idx, chr1_name, &len))){
   if (len == -1) {
     std::cout << "Failed to retreive data" << std::endl;
   }
 }
 // char * chr1_char = fai_fetch64( fasta_idx, chr1_name, end_chr1);
 //if(chr1_char != NULL){
 //std::string chr1 = chr1_char;
 //std::cout << chr1[0] << std::endl;
 // for(int i=0; i < end_chr1 - beg_chr1; i++){
 //   std::cout << chr1_char[i] << std::endl;
 // }
 std::cout << "As a char* " << chr1_char << std::endl;
 std::string stringVersion{chr1_char};
 std::cout << "As a string " <<stringVersion << std::endl;
 // iterate by finding number of sequences, querying name, and querying by name
 int num_seqs = faidx_nseq (fasta_idx);
 // name of last sequence
 std::string last_seq = "";
 if(const char* last_seq_char =  faidx_iseq (fasta_idx, num_seqs-1))
   {
   last_seq  = std::string{last_seq_char};
   }
 else
   {
   std::cerr << "Invalid Contig Number" << std::endl;
   last_seq = "Invalid";
 }
 std::cout << "number of seqs " << num_seqs << " " << last_seq << std::endl;

 std::cout << "Testing Class Version " << std::endl;
 auto newFasta = FastaWrapper{refName};
 std::string chr1_new = newFasta.getRegion("chr1");
 std::cout << "New Chr1 " << chr1_new << std::endl;

 std::string chr1_subset = newFasta.getRegion("chr1", 2, 4);
 std::cout << "Subsetted : " << chr1_subset << std::endl;

 std::cout << "Testing iteration through contigs with Class Version" << std::endl;
 for (int i=0; i < newFasta.getNumberContigs(); i++){
   std::string tempContigName = newFasta.getContigNameByIndex(i);
   std::string tempContig = newFasta.getRegion(tempContigName);
   
   std::cout << "Contig Name: " << tempContigName << std::endl;
   std::cout << "Contig: " << tempContig << std::endl;

   
 }
 return 0;

}
