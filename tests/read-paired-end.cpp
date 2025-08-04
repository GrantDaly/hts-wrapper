#include <map>


#include "bam-header.h"
#include "bam-record.h"
#include "cigar.h"
#include "faidx-cpp.h"
#include "hts-wrapper-util.h"
#include "htslib/faidx.h"
#include "htslib/hts.h"
#include "htslib/sam.h"
#include "sam-file.h"
#include "sam-iterator.h"
#include <memory>
#include <ostream>
#include <sstream>
#include <string_view>
#include <vector>

void printPreviouslyAligned(const BamRecord & mitoBam,
			    const BamRecord & nuclearBam,
			    const FastaWrapper & ref,
			    std::ostream & os) {

 // print cigar 
  for(const auto cig: nuclearBam.getCigar()){
    std::cout << "[" << cig << "|" << 
      cig.subjectBasesConsumed() << "]";
  }
  os << std::endl;

  std::stringstream nuclear_query_f, nuclear_ref_f, nuclear_mismatch_f;
  auto nuclear_seq = nuclearBam.getSequence();
  auto nuclear_seq_view = std::string_view(nuclear_seq.data(), nuclear_seq.size());

  int last_query_offset{0};
  int current_query_offset{0};

  int last_subject_offset{0};
  int current_subject_offset{0};

  const auto tid_name = ref.getContigNameByIndex(nuclearBam.getTidNumber());
  const auto start_pos = nuclearBam.getPos();
  const auto ref_bases = nuclearBam.getReferenceLength();
  const auto nuclear_ref_seq = ref.getRegion(tid_name, start_pos, start_pos + ref_bases);
  auto nuclear_ref_seq_view = std::string_view(nuclear_ref_seq.data(), nuclear_ref_seq.size());

  for(const auto cig: nuclearBam.getCigar()){
    const auto query_consumed =  cig.queryBasesConsumed();
    const auto query_padded =  cig.queryNumPadPrint();
    const auto subject_consumed =  cig.subjectBasesConsumed();
    const auto subject_padded =  cig.subjectNumPadPrint();

    // query
    const auto temp_query_substring =  nuclear_seq_view.substr(current_query_offset, query_consumed);
      nuclear_query_f << temp_query_substring
	      << std::string(query_padded, ' ');
    current_query_offset = last_query_offset + query_consumed;
    last_query_offset = current_query_offset;


    
    // subject
    std::string temp_ref_substring;
    // softclip is special case that only adds spaces to pad reference but not change the coordinates within the reference.
    if(cig.getCigar() == CigarOperationType::SoftClip){
      nuclear_ref_f << std::string(subject_padded, ' ');
    }
    else {

    
      temp_ref_substring = nuclear_ref_seq_view.substr(current_subject_offset, subject_consumed);
      nuclear_ref_f << temp_ref_substring
	      << std::string(subject_padded, ' ');

    current_subject_offset = last_subject_offset + subject_consumed;
    last_subject_offset = current_subject_offset;
  }
    
      // alignment string
    const auto number_align_print = cig.getCount();
    const auto cigarType = cig.getCigar();
      
	if((cigarType == CigarOperationType::SoftClip) || (cigarType == CigarOperationType::SoftClip)) {
	    nuclear_mismatch_f << std::string(number_align_print, 'S');

      }
	// really just refers to not being an indel or skip. check if bases match manually
	else if (cigarType == CigarOperationType::Match) {

	  for(auto i{0}; i < temp_query_substring.length(); i++)
	    {
	      if(temp_query_substring[i] == temp_ref_substring[i])
		nuclear_mismatch_f << "|";
	      else
		nuclear_mismatch_f << "X";
	    }
	// nuclear_mismatch_f << std::string(number_align_print, '|');
	}
	else if(cigarType ==  CigarOperationType::Insertion)
	  {
	nuclear_mismatch_f << std::string(number_align_print, '+');
	  }
	else if(cigarType ==  CigarOperationType::Deletion)
	  {
	nuclear_mismatch_f << std::string(number_align_print, '-');
	  }


  }

  nuclear_query_f << std::endl;
  os << nuclear_query_f.str();

  nuclear_mismatch_f << std::endl;
  os << nuclear_mismatch_f.str();
  
  nuclear_ref_f << std::endl;
  os << nuclear_ref_f.str();
}

int main() {

  // std::string numt_bam = "~/Downloads/P_44.numt.bam";
  const std::string refName = "/home/grant/science/references/hg38/Homo_sapiens_assembly38.fasta";
   auto inFasta = FastaWrapper{refName};
  std::string chrM = inFasta.getRegion("chrM");
  // std::string refName = "data/test.numt.fasta";
  // auto inFasta = FastaWrapper{refName};
  auto numt_bam = SamFile("/home/grant/Downloads/P_44.numt.bam", "rb");
  // issue opening a second iterator with same file*, going to try opening a second file handle for now
  auto numt_mate_bam = SamFile("/home/grant/Downloads/P_44.numt.bam", "rb");
  // samFile *numt_bam = sam_open("alignment.bam", "r");
  // sam_hdr_t *numt_bam_header = sam_hdr_read(sf);
  // sam_hdr_t *numt_bam_header = sam_hdr_read(sf);
  auto bam_header = numt_bam.read_header();

  if (numt_bam.index == nullptr) {
    std::cerr << "No Index for Bam File " << std::endl;
    std::exit(1);
  }

  std::string main_chrom{"chrM"};
  int main_chrom_tid = bam_header.contigNameToTid(main_chrom);

  hts_itr_t *chrM_iter = sam_itr_querys(numt_bam.index, bam_header.hdr, "chrM");

  // int contigCode = bam_name2id (bamHeader, bedLine.chrom.c_str());
  // hts_itr_t* bamItr =  sam_itr_queryi(inBamIndex, contigCode, upstreamStart,
  // bedLine.end);
  if (chrM_iter == nullptr) {
    std::cerr << "Error opening iterator for ChrM" << std::endl;
  }

  std::map<std::string, std::pair<std::unique_ptr<BamRecord>,
				  std::unique_ptr<BamRecord>
			  >
	   >
      numt_read_pairs;

  auto counter{0};
  auto mito_iter_wrapper = BamIterator(&numt_bam, &bam_header, main_chrom);
  while (const auto & rec = mito_iter_wrapper.getNextRecord()) {
    auto first_bam = rec.value();
    auto const first_bam_name = first_bam.getQName();

    // needs to be primary alignment
    if(BAM_FSECONDARY == (first_bam.getFlag() & BAM_FSECONDARY))
      continue;

    auto const read_tid = first_bam.getTidNumber();
    auto const mate_tid = first_bam.getMateTidNumber();
    auto const read_contig_name = bam_header.contigNameToTid(read_tid);
    auto const mate_contig_name = bam_header.contigNameToTid(mate_tid);
    auto const read_pos = first_bam.getPos();
    auto const mate_pos = first_bam.getMatePos();
    auto const first_bam_flag = first_bam.getFlag();
    
    
    std::vector<CigarOperation> first_bam_cigar = first_bam.getCigar();
    
    if (read_tid != mate_tid) {
            auto mate_iter_wrapper = BamIterator(&numt_mate_bam, mate_tid,
					   mate_pos, mate_pos+1);
      const auto & rec_two = mate_iter_wrapper.getRecordByName(first_bam_name);
      if(rec_two) {

	auto second_bam = rec_two.value();
        auto const second_bam_flag = second_bam.getFlag();

	std::vector<CigarOperation> second_bam_cigar = second_bam.getCigar();

	auto direction_l = [](const decltype(first_bam) & f) {if(f.isReadRevStrand()){return "<-";}else if(! f.isReadRevStrand()) {return "->";}else{return "?";}};

	  std::cout << "------------" << std::endl;  	  
	  const auto first_bam_q_length = first_bam.getQueryLength();
  	  const auto second_bam_q_length = second_bam.getQueryLength();
  	  const auto first_bam_r_length = first_bam.getReferenceLength();
  	  const auto second_bam_r_length = second_bam.getReferenceLength();
	  const auto second_bam_seq = second_bam.getSequence();
	  const auto number_clipped = second_bam_cigar[second_bam_cigar.size()].getCount();
  	
	 std::cout << "Flag: " << first_bam_flag << "/"
		  << second_bam_flag <<direction_l(first_bam)
		  << direction_l(second_bam)
     		<< " First Contig: " << read_tid << ":" << read_pos << std::endl;
	
	 std::cout << read_contig_name << ":" << read_pos << ":" << first_bam.readOrientationString() << ":" << first_bam.getInsertSize() << ":" << first_bam_q_length << ":" << first_bam.getReferenceLength() << ":";

	for(const auto & cig: first_bam_cigar){
	  std::cout << cig << "|";
	}
	std::cout << std::endl;

	std::cout << mate_contig_name << ":" << mate_pos << ":" << second_bam.readOrientationString() << ":" << second_bam.getInsertSize() << ":" << second_bam_q_length << ":" << second_bam.getReferenceLength() << ":";
	
	for(const auto & cig: second_bam_cigar){
	  std::cout << cig << "|";
	}
	std::cout << std::endl;

	std::cout << "# Clipped " << number_clipped << std::endl;

	  std::string read_ref_seq = inFasta.getRegion(mate_contig_name,mate_pos, mate_pos+second_bam_r_length);

	  std::cout << std::endl;
	  
	  const auto formattedAlignMito = makeAlignmentString(first_bam, inFasta);
	  std::cout << formattedAlignMito;
	  
	  const auto formattedAlignNuclear = makeAlignmentString(second_bam, inFasta);
	  std::cout << formattedAlignNuclear;
          std::cout << "++++++++++" << std::endl;
      // }
      }
      else {
	std::cerr << "Couldn't find mate pair bam" << std::endl;
	//todo can I just continue the loop
	//std::exit(1);
      }

      counter++;
    }
  }

  return 0;
}
