
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>


#include "bam-header.h"
#include "htslib/hts.h"
#include <htslib/sam.h>


#include "hts-wrapper-util.h"
#include "cigar.h"
#include "sam-file.h"
#include "faidx-cpp.h"
// #include "bam-header.h"
#include "bam-record.h"


//#include "insertion.h"
#include "aligned-insertion.hpp"
#include "snp.h"

// std::pair<BamRecord,BamRecord>
// generateNumtReadPair(std::string readName,
// 		     const std::string & seq_one_name,const  std::string & seq_two_name,
// 		     const std::string & seq_one,const  std::string & seq_two,
// 		     hts_pos_t seq_one_break, hts_pos_t seq_two_break,
// 		     hts_pos_t seq_one_start, hts_pos_t seq_two_start,
// 		     int insert_orientation, int read_size,
// 		     BamHeader & hdr) {

//   // if insert size positive F1R2, else if negative F2R1
//   uint16_t bamFlagR1{0}, bamFlagR2{0};
//   hts_pos_t pos_r1{0}, pos_r2{0};
//   std::vector<char> numt_sequence;
//   std::vector<char> numt_sequence_complement;
//   // first go from seq one start to break
//   for(auto i=seq_one_start; i<seq_one_break; i++){
//     numt_sequence.push_back(seq_one[i]);
//       }
//   //could flip this for numt's with different orientation i.e. inversions
//   for(auto i=seq_two_break; i<seq_two_start; i++){
//     numt_sequence.push_back(seq_two[i]);
//       }
//   for(const auto & c : numt_sequence){
//     std::cout << c;
//     numt_sequence_complement.push_back(getComplement(c));
//   }
//   std::cout << std::endl;

//   for(const auto & c : numt_sequence_complement){
//     std::cout << c;
//   }
//   std::cout << std::endl;
//   std::vector<char> seq_r1;
//   std::vector<char> seq_r2;
//   std::vector<char> qual_r1;
//   std::vector<char> qual_r2;
//   std::vector<CigarOperation> cigar_r1{};
//   std::vector<CigarOperation> cigar_r2{};

//   // for now only dealing with 5'->5' (same orientation numts)
//   auto num_matchesR1{0}, num_matchesR2{0}, num_softclipR1{0}, num_softclipR2{0};
//   if(insert_orientation >0){
//     bamFlagR1 = BAM_FPAIRED | BAM_FMREVERSE | BAM_FREAD1;
//     bamFlagR2 = BAM_FPAIRED | BAM_FREVERSE | BAM_FREAD2;
//     pos_r1 = seq_one_start;
//     pos_r2 = seq_two_break;

//     num_matchesR1 = seq_one_break - seq_one_start;
//     num_softclipR1 = read_size - num_matchesR1;
//     cigar_r1.push_back({CigarOperationType::Match, num_matchesR1});
//     cigar_r1.push_back({CigarOperationType::SoftClip, num_softclipR1});

//     num_matchesR2 = seq_two_start - seq_two_break;
//     num_softclipR2 = read_size - num_matchesR2;
//     cigar_r2.push_back({CigarOperationType::Match, num_matchesR2});
//     cigar_r2.push_back({CigarOperationType::SoftClip, num_softclipR2});

//     for(auto i{0}; i < read_size; i++){
//       seq_r1.push_back(numt_sequence[i]);
//       qual_r1.push_back(0);
//       seq_r2.push_back(numt_sequence[numt_sequence.size() - i - 1]);
//       qual_r2.push_back(0);
//     }
//   }
//   else if(insert_orientation <0){
//     bamFlagR1 = BAM_FPAIRED | BAM_FREVERSE | BAM_FREAD1;
//     bamFlagR2 = BAM_FPAIRED | BAM_FMREVERSE | BAM_FREAD2;
//     pos_r1 = seq_two_break;
//     pos_r2 = seq_one_start;
    
//     num_matchesR2 = seq_one_break - seq_one_start;
//     num_softclipR2 = read_size - num_matchesR2;
//     cigar_r1.push_back({CigarOperationType::Match, num_matchesR2});
//     cigar_r1.push_back({CigarOperationType::SoftClip, num_softclipR2});

//     num_matchesR1 = seq_two_start - seq_two_break;
//     num_softclipR1 = read_size - num_matchesR1;
//     cigar_r2.push_back({CigarOperationType::Match, num_matchesR1});
//     cigar_r2.push_back({CigarOperationType::SoftClip, num_softclipR1});

//     for(auto i{0}; i < read_size; i++){
//       seq_r1.push_back(numt_sequence[i]);
//       qual_r1.push_back(0);
//       seq_r2.push_back(numt_sequence[numt_sequence.size() - i - 1]);
//       qual_r2.push_back(0);
//     }

//     // cigar_r1.push_back({CigarOperationType::Match, num_matchesR1});
//     // cigar_r1.push_back({CigarOperationType::SoftClip, num_softclipR1});

//   }
//   else {
//     std::cerr << "Invalid Insert Size (can't be zero)" << std::endl;
//   }



//        auto readOne = BamRecord(readName,
// 			    bamFlagR1, seq_one_name, pos_r1,
// 				 30, cigar_r1,
// 				seq_two_name, pos_r2,0,
// 				 seq_r1, qual_r1, 0, hdr);
//        auto readTwo = BamRecord(readName,
// 			    bamFlagR2, seq_two_name, pos_r2,
// 				 30, cigar_r2,
// 				seq_one_name, pos_r1,0,
// 				 seq_r2, qual_r2, 0, hdr);

//       auto bamOneR2 = BamRecord(readName,
// 			    147, "chr1", 2000, 30, cigar_vector_rev,
// 				"chrM", 2000,0,seqrc, qual, 0, outSamHeader);
  

//       std::pair<BamRecord,BamRecord> outPair{readOne,readTwo};
//        return outPair;
// }


// with this version, use a NUMT object which holds an insertion object (this
// should only hold the ref contig, insertion coordinate, and inserted sequence.
// then in the NUMT object we track 5' and 3' breakpoint with respect to the
// mitochondria, the orientation as it goes into the chromosome. and any SNP's
// as they would appear to the mitochondria. std::pair<BamRecord, BamRecord>
std::pair<BamRecord,BamRecord>    
generateNumtReadPair(std::string readName, const AlignedInsertion &numt,
                     hts_pos_t breakpoint_offset, int  insert_size, int read_size,
		     const std::string & chrM,
		     BamHeader & hdr) {

  // if insert size positive F1R2, else if negative F2R1
  uint16_t bamFlagR1{0}, bamFlagR2{0};
  hts_pos_t pos_r1{0}, pos_r2{0};
  std::vector<char> numt_sequence;
  std::vector<char> numt_sequence_complement;
  std::vector<CigarOperation> cigar_numt_inflated{};
  // std::vector<CigarOperation> cigar_r2{};
  // first go from seq one start to break
  // for now it's difficult to describe ones where the insert doesn't span both breakpoints
  auto fivePrimeFlank = numt.flanking_sequence_5p.substr( numt.flanking_sequence_5p.length() - breakpoint_offset,  std::string::npos );
  const auto numt_string = numt.inserted_sequence;
  auto threePrimeFlank = numt.flanking_sequence_3p.substr( 0,breakpoint_offset);
  for(const auto c: fivePrimeFlank){
    numt_sequence.push_back(c);
      }
  for(auto i=0; i < numt.insertion_size;i++){
    const auto tempNumtBase = numt.inserted_sequence[i];
    numt_sequence.push_back(tempNumtBase);
    const auto tempMitoRef = chrM[numt.alt_5p_breakpoint + i];
    
    if(tempNumtBase != tempMitoRef)
      {
	std::cout << "Numt " << tempNumtBase << " Mito " << tempMitoRef << std::endl;
	cigar_numt_inflated.push_back({CigarOperationType::Mismatch,1});
      }
    else
      {
      cigar_numt_inflated.push_back({CigarOperationType::Match, 1});
      }  
  }

  for(const auto c: threePrimeFlank){
    numt_sequence.push_back(c);
      }
  // std::vector<CigarOperation> cigar_numt_deflated = deflate_cigar_vector(cigar_numt_inflated);


  // //could flip this for numt's with different orientation i.e. inversions
  // for(auto i=seq_two_break; i<seq_two_start; i++){
  //   numt_sequence.push_back(seq_two[i]);
  //     }
  for(const auto & c : numt_sequence){
    std::cout << c;
    numt_sequence_complement.push_back(getComplement(c));
  }
  std::cout << std::endl;

  for(const auto & c : numt_sequence_complement){
    std::cout << c;
  }
  std::cout << std::endl;
  std::vector<char> seq_r1;
  std::vector<char> seq_r2;
  std::vector<char> qual_r1;
  std::vector<char> qual_r2;
  std::vector<CigarOperation> cigar_r1_inflated{};
  std::vector<CigarOperation> cigar_r2_inflated{};
  

  hts_pos_t bases_rclipped_r1{0};
  hts_pos_t bases_matched_r1{0};
  hts_pos_t bases_rclipped_r2{0};
  hts_pos_t bases_matched_r2{0};
  // // for now only dealing with 5'->5' (same orientation numts)
  // auto num_matchesR1{0}, num_matchesR2{0}, num_softclipR1{0}, num_softclipR2{0};
  if(insert_size >0){
    bamFlagR1 = BAM_FPAIRED | BAM_FMREVERSE | BAM_FREAD1;
    bamFlagR2 = BAM_FPAIRED | BAM_FREVERSE | BAM_FREAD2;
    pos_r1 = numt.ref_breakpoint - breakpoint_offset;
    if((pos_r1 + read_size) > numt.ref_breakpoint){
      bases_rclipped_r1 = (pos_r1 + read_size) - numt.ref_breakpoint ;
      bases_matched_r1 = read_size - bases_rclipped_r1;
    }
    else {
      bases_rclipped_r1 = 0;
      bases_matched_r1 = read_size;
    }
    // pos2 is on mitochondria
    pos_r2 =  numt.alt_5p_breakpoint +
		      (insert_size - breakpoint_offset) - read_size;
  if(pos_r2 < numt.alt_5p_breakpoint){
    bases_rclipped_r2 = numt.alt_5p_breakpoint - pos_r2;
    bases_matched_r2 = read_size - bases_rclipped_r2;
    pos_r2 = numt.alt_5p_breakpoint;
  }
  else {
      bases_rclipped_r2 = 0;
      bases_matched_r2 = read_size;
  }
  std::cout << "read 1 pos " << pos_r1 << " Clip " << bases_rclipped_r1 <<
    std::endl << " Read 2 pos " << pos_r2 << " Clip " << bases_rclipped_r2 << std::endl;
  

      for(auto i{0}; i < read_size; i++){
      seq_r1.push_back(numt_sequence[i]);
      qual_r1.push_back(0);
      
      }

      // reverse for reverse complement R2
      for(auto i{0}; i < read_size; i++){
      seq_r2.push_back(numt_sequence_complement[insert_size - i - 1]);
      qual_r2.push_back(0);
    }
      
      // create inflted cigar vectors. do this seperately to deal with soft clipping
      // read one should only match flanking region and then be clipped
      if(bases_matched_r1 > 0){
	cigar_r1_inflated.push_back(CigarOperation{CigarOperationType::Match,
						   (int) bases_matched_r1});
      }
      // for(auto i{0}; i < read_size - bases_matched_r1; i++){
      // cigar_r1_inflated.push_back(cigar_numt_inflated[i]);
      // }
      if(bases_rclipped_r1 > 0){
	cigar_r1_inflated.push_back(CigarOperation{CigarOperationType::SoftClip,
						   (int) bases_rclipped_r1});
      }


      for(auto i{bases_matched_r2 -1}; i >= 0; i--){
	cigar_r2_inflated.push_back(cigar_numt_inflated.at(i));
      }
      if(bases_rclipped_r2 > 0){
	cigar_r2_inflated.push_back(CigarOperation{CigarOperationType::SoftClip,
						   (int) bases_rclipped_r2});
      }
        }
  std::vector<CigarOperation> cigar_r1_deflated = deflate_cigar_vector(cigar_r1_inflated);
  std::vector<CigarOperation> cigar_r2_deflated = deflate_cigar_vector(cigar_r2_inflated);

  //   num_matchesR1 = seq_one_break - seq_one_start;
  //   num_softclipR1 = read_size - num_matchesR1;
  //   cigar_r1.push_back({CigarOperationType::Match, num_matchesR1});
  //   cigar_r1.push_back({CigarOperationType::SoftClip, num_softclipR1});

  //   num_matchesR2 = seq_two_start - seq_two_break;
  //   num_softclipR2 = read_size - num_matchesR2;
  //   cigar_r2.push_back({CigarOperationType::Match, num_matchesR2});
  //   cigar_r2.push_back({CigarOperationType::SoftClip, num_softclipR2});

  //   for(auto i{0}; i < read_size; i++){
  //     seq_r1.push_back(numt_sequence[i]);
  //     qual_r1.push_back(0);
  //     seq_r2.push_back(numt_sequence[numt_sequence.size() - i - 1]);
  //     qual_r2.push_back(0);
  //   }
  // }
  // else if(insert_orientation <0){
  //   bamFlagR1 = BAM_FPAIRED | BAM_FREVERSE | BAM_FREAD1;
  //   bamFlagR2 = BAM_FPAIRED | BAM_FMREVERSE | BAM_FREAD2;
  //   pos_r1 = seq_two_break;
  //   pos_r2 = seq_one_start;
    
  //   num_matchesR2 = seq_one_break - seq_one_start;
  //   num_softclipR2 = read_size - num_matchesR2;
  //   cigar_r1.push_back({CigarOperationType::Match, num_matchesR2});
  //   cigar_r1.push_back({CigarOperationType::SoftClip, num_softclipR2});

  //   num_matchesR1 = seq_two_start - seq_two_break;
  //   num_softclipR1 = read_size - num_matchesR1;
  //   cigar_r2.push_back({CigarOperationType::Match, num_matchesR1});
  //   cigar_r2.push_back({CigarOperationType::SoftClip, num_softclipR1});

  //   for(auto i{0}; i < read_size; i++){
  //     seq_r1.push_back(numt_sequence[i]);
  //     qual_r1.push_back(0);
  //     seq_r2.push_back(numt_sequence[numt_sequence.size() - i - 1]);
  //     qual_r2.push_back(0);
  //   }

  //   // cigar_r1.push_back({CigarOperationType::Match, num_matchesR1});
  //   // cigar_r1.push_back({CigarOperationType::SoftClip, num_softclipR1});

  // }
  // else {
  //   std::cerr << "Invalid Insert Size (can't be zero)" << std::endl;
  // }


 
    for (const auto & cig: cigar_r1_inflated) {
    std::cout << cig << "|";
  }
    std::cout << "Inflated " << cigar_r1_inflated.size() << std::endl;
  for (const auto & cig: cigar_r1_deflated) {
    std::cout << cig << "|";
  }
  std::cout  << "Deflated " <<  cigar_r1_deflated.size() << std::endl;
    for (const auto & cig: cigar_r2_inflated) {
    std::cout << cig << "|";
  }
    std::cout << "Inflated " << cigar_r2_inflated.size() << std::endl;
  for (const auto & cig: cigar_r2_deflated) {
    std::cout << cig << "|";
  }
  std::cout  << "Deflated " <<  cigar_r2_deflated.size() << std::endl;

       auto readOne = BamRecord(readName,
			    bamFlagR1, numt.refContig, pos_r1,
				 0, cigar_r1_deflated,
				numt.altContig, pos_r2,insert_size,
				 seq_r1, qual_r1, 0, hdr);
       std::cout << "got to second " << std::endl;
       auto readTwo= BamRecord(readName,
			       bamFlagR2, numt.altContig, pos_r2,
				 0, cigar_r2_deflated,
				numt.refContig, pos_r1,insert_size,
				 seq_r2, qual_r2, 0, hdr);
 
       // auto readTwo = BamRecord(readName,
       // 			    bamFlagR2, seq_two_name, pos_r2,
       // 				 30, cigar_r2,
       // 				seq_one_name, pos_r1,0,
       // 				 seq_r2, qual_r2, 0, hdr);
       

       std::pair<BamRecord,BamRecord> outPair{readOne,readTwo};
       return std::move(outPair);
}

int main() {

  // open contigs
  std::string refName = "data/test.numt.fasta";
  auto inFasta = FastaWrapper{refName};
  std::string chrM = inFasta.getRegion("chrM");
  std::string chr1 = inFasta.getRegion("chr1");
  std::string numt_chr1 = inFasta.getRegion("NUMT.chr1");

  // initalize a sam/bam header
  // sam_hdr_t * outSamHeader = sam_hdr_init();
  auto outSamHeader = BamHeader();
  // looks like this is really to reserve order to output header
  // sam_hdr_add_line (outSamHeader, "HD","SQ");
  outSamHeader.addSequencesFromFasta(inFasta);

  auto numberLines = outSamHeader.getNumEntriesByField("SQ");
  
  std::cout << "Number of lines : " << numberLines << std::endl;

  // samFile *sf = sam_open("numt.bam", "w");
  // outSamHeader.write_header(sf);
  // sam_close(sf);
  auto sf = SamFile("numt.bam", "wb");
  sf.write_header(outSamHeader);

  // CigarOperation first_cigar{CigarOperationType::Insertion, 10};
  // std::cout << first_cigar << std::endl;

  std::vector<CigarOperation> cigar_vector_for{
    {CigarOperationType::Match, 7}, {CigarOperationType::SoftClip,3} 
  };

  std::vector<CigarOperation> cigar_vector_rev{
    {CigarOperationType::Match, 7}, {CigarOperationType::SoftClip, 3} 
  };
  
  // std::stringstream cigar_string{""};
  // for(auto const & i: cigar_vector){
  //   cigar_string << i ;
  // }
  // std::cout << cigar_string.str() << std::endl;

  std::vector<char> seq{'A','T','C','G','A','A','A','A','A','A'};
  std::vector<char> seqrc{'T','G','A','C','G','A','G','T','T','T'};
  std::vector<char> qual{0,75,0,30,30,30,30,30,30,30};
  // std::vector<char> qual{'A','T','C','G'};
  // auto bamOneR1 = BamRecord();
  // bamOneR1.bam_set_qname("pair1R1");
  char* cigar_end_addr = nullptr;
  uint32_t * cigar_buf = nullptr;
  size_t cigar_size = 0;
  int num_cigar = -1;
  // if((num_cigar =sam_parse_cigar(cigar_string.str().c_str(), & cigar_end_addr,& cigar_buf,
  // 				 & cigar_size)) > 0){

  //   printf("Num Cigar %d\n", num_cigar);


      // auto bamOneR1 = BamRecord("pair1",
      // 			    99, "chrM", 1000, 30, cigar_vector_for,
      // 				"chr1", 1000,0,seq, qual, 0, outSamHeader);

      // auto bamOneR2 = BamRecord("pair1",
      // 			    147, "chr1", 2000, 30, cigar_vector_rev,
      // 				"chrM", 2000,0,seqrc, qual, 0, outSamHeader);

      // sf.write_record(outSamHeader, bamOneR1);
      // sf.write_record(outSamHeader, bamOneR2);

  // get the reference sequence where numt will start
  std::string numt_ref_template = chrM.substr(100,40);
  std::string numt_sequence = "caattgatatcattttgcctcaacaatttcaccgaacacc";
  std::cout << numt_ref_template << std::endl;
  std::cout << numt_sequence << std::endl;

    // define numt
  // std::vector<CigarOperation> cigar_vector_rev{
  //   {CigarOperationType::Match, 7}, {CigarOperationType::SoftClip, 3} 
  // };
  auto numt = AlignedInsertion("chr1",  300,
	    "chrM", 100, 140, Orientation::refForAltFor,
			       numt_sequence, chr1,chrM);
  std::cout << "Ref Name " << numt.refContig
	    <<  " Ref Breakpoint " <<  numt.ref_breakpoint << std::endl;

  std::cout << "Inserted Seq. " << numt.inserted_sequence << std::endl;

  //     auto read_pair = generateNumtReadPair("pair1",
  // 		     "chrM","chr1",
  // 		     chrM,chr1,
  // 		     100, 300,
  // 		     90, 310,
  // 			   1, 20, outSamHeader);
  // std::cout << "5' flank " << numt.flanking_sequence_5p << std::endl;
  // std::cout << "Inserted " << numt.inserted_sequence << std::endl;
  // std::cout << "3' flank " << numt.flanking_sequence_3p << std::endl;
  auto read_pair = generateNumtReadPair("pair1",
					numt, 20, 40,
					30, chrM,
					outSamHeader);


      sf.write_record(outSamHeader, read_pair.first);
      sf.write_record(outSamHeader, read_pair.second);
      

 return 0;

}
