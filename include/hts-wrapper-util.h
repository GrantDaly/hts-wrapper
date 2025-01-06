#pragma once

#include "cigar.h"
#include "faidx-cpp.h"
#include "bam-record.h"

char getComplement(char in){
  char outChar{'N'};
  switch (in) {
  case 'A':
    return 'T';
  break;
  case 'a':
    return 't';
  break;
  case 'T':
    return 'A';
  break;
  case 't':
    return 'a';
  break;
  case 'C':
    return 'G';
  break;
  case 'c':
    return 'g';
  case 'G':
    return 'c';
  break;
  case 'g':
    return 'c';
  break;
  default:
    return 'N';
    break;
  }
return outChar;
}

// eventually these may become std::views or similar

// take cigar operations and merge the same, eg. =1,=1, =1,M1,=1,=1 -> =3,M1,=2

std::vector<CigarOperation>
deflate_cigar_vector(const std::vector<CigarOperation> & inflated) {
  std::vector<CigarOperation> deflated;

  


  CigarOperation last_cigar = inflated.at(0);
  CigarOperation current_cigar = last_cigar;
  // if it's only one entry no possibility to deflate
  if(inflated.size() == 1) {
    deflated.push_back(last_cigar);
    return std::move(deflated);
  }
  /* for(const auto & temp_cigar: deflated) { */
    for( auto i{1}; i< inflated.size(); i++) {
      std::cout << "Top of loop Index : " << i << std::endl;
      current_cigar = inflated[i];
      if(current_cigar.getCigar() == last_cigar.getCigar()) {
      /* std::cout << "Before increment " << last_cigar << std::endl; */
	/* last_cigar.incrementCount(); */
	/* std::cout << "Last Count : " << last_cigar.getCount() << " Current Count " << current_cigar.getCount() << std::endl; */
	last_cigar.addToCount(current_cigar.getCount());
	/* std:: cout << "Adding " << std::endl; */
	/* std::cout << "Last Count : " << last_cigar.getCount() << " Current Count " << current_cigar.getCount() << std::endl; */
	// if we make it to end, write out 
	/* if(i == inflated.size() - 1) { */
	/*   // make sure it's pushed back if the last entry is a match. */
  	/* deflated.push_back(last_cigar); */
	/* } */
      /* std::cout << "After increment " << last_cigar << std::endl; */

    }
      else if (current_cigar.getCigar() != last_cigar.getCigar()){
	      /* std::cout << last_cigar << std::endl; */
	deflated.push_back(last_cigar);
	last_cigar = current_cigar;
      }
      	// if we make it to end, write out 
	if(i == inflated.size() - 1) {
	  std::cout << "last writeout " << std::endl;
	  // make sure it's pushed back if the last entry is a match.
  	deflated.push_back(last_cigar);
	}
      std::cout << "End of loop" << std::endl;
      
  }
    return std::move(deflated);

}

std::string  makeAlignmentString(const BamRecord & inBam,
			    const FastaWrapper & ref )
{
    // auto const read_contig_name = bam_header.contigNameToTid(read_tid);
    // auto const mate_contig_name = bam_header.contigNameToTid(mate_tid);
  // bam doesn't necessarily start and end at listed position w.r.t. the reference, if there is left or right clipping. corrected start is position - #subject padded bases.
  
    // auto const mate_pos = mitoBam.getMatePos();
  // will insert into this vector, which should not modify original
  std::stringstream nuclear_query_f, nuclear_ref_f, nuclear_mismatch_f;
  auto nuclear_seq = inBam.getSequence();
  auto nuclear_seq_view = std::string_view(nuclear_seq.data(), nuclear_seq.size());

  int last_query_offset{0};
  int current_query_offset{0};

  int last_subject_offset{0};
  int current_subject_offset{0};

  const auto tid_name = ref.getContigNameByIndex(inBam.getTidNumber());
  const auto start_pos = inBam.getPos();
  const auto ref_bases = inBam.getReferenceLength();
  const auto nuclear_ref_seq = ref.getRegion(tid_name, start_pos, start_pos + ref_bases);
  auto nuclear_ref_seq_view = std::string_view(nuclear_ref_seq.data(), nuclear_ref_seq.size());

  for(const auto cig: inBam.getCigar()){
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

  std::string out_string{};
  nuclear_query_f << std::endl;
  

  nuclear_mismatch_f << std::endl;

  
  nuclear_ref_f << std::endl;
  
  out_string += nuclear_query_f.str()
	     + nuclear_mismatch_f.str()
	     + nuclear_ref_f.str();
  return out_string;
}
