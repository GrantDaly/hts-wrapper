//#include <htslib/sam.h>
#include <iostream>
#include <string>
# include "hts-wrapper/faidx-cpp.h"


int main() {

std::string refName = "data/test1.fasta";
 std::cout << "Fasta Filename: " << refName << std::endl;

faidx_t* fasta_idx = fai_load_format(refName.c_str(),
					  FAI_FASTA);

 const char* chr1_name = "chr1";

 hts_pos_t len=-1;


 const char* chr1_char;
 // while((chr1_char = fai_parse_region( fasta_idx, chr1_name, &tid_chr1, &beg_chr1, &end_chr1, HTS_PARSE_THOUSANDS_SEP))){
 if(! (chr1_char = fai_fetch64( fasta_idx, chr1_name, &len))){
   if (len == -1) {
     std::cout << "Failed to retreive data" << std::endl;
   }
 }

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

 std::cout << "Testing iteration through contigs with Class Version" << std::endl;
 for (int i=0; i < newFasta.getNumberContigs(); i++){
   std::string tempContigName = newFasta.getContigNameByIndex(i);
   std::string tempContig = newFasta.getRegion(tempContigName);
   
   std::cout << "Contig Name: " << tempContigName
	     << "Contig Length: " << newFasta.getLengthByContig(tempContigName) << std::endl;
   std::cout << "Contig: " << tempContig << std::endl;

 }
 return 0;

}
