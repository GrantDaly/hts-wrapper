#pragma once

#include <iostream>
#include <algorithm>

#include "hts-wrapper-util.h"
#include "htslib/hts.h"
#include "insertion.h"
//#include "insertion.h"

enum class Orientation {refForAltFor, refForAltRev};


// point of this class is to deal with insertions and the type of artifactual SNPs they could induce against the alt (aka the sequence from which the inserted sequence originated).
class AlignedInsertion {
 public:
  std::string refContig, altContig;
  // basically storing this so I know it's in the correct orientation, so no matter what it's in 5'->3' as the reference genome is.
  std::string flanking_sequence_5p;
  std::string inserted_sequence;
  std::string flanking_sequence_3p;
  
  Orientation orientation;
  hts_pos_t ref_breakpoint{0},
    alt_5p_breakpoint{0}, alt_3p_breakpoint{0};
  hts_pos_t insertion_size{0};

  hts_pos_t ref_breakpoint_flank{300};
  hts_pos_t fivePFlankStartRef{0};
  hts_pos_t  threePFlankEndRef{0};

  // starting with just the interior artifactual snps
  std::vector<CigarOperation> snps_to_alt{};
  // std::vector<CigarOperation> snps_to_alt_5p{};
  // std::vector<CigarOperation> snps_to_alt_3p{};
  
  AlignedInsertion(std::string ref_name_p, 
		     hts_pos_t ref_breakpoint_p,
		     std::string alt_name_p, 
		     hts_pos_t alt_5p_breakpoint_p, hts_pos_t alt_3p_breakpoint_p,
		     Orientation orientation_p,
		   std::string inserted_sequence_p,
		   const std::string & ref_string_r,
		   const std::string & alt_string_r);
    };

    AlignedInsertion::AlignedInsertion(
        std::string ref_name_p, hts_pos_t ref_breakpoint_p,
        std::string alt_name_p, hts_pos_t alt_5p_breakpoint_p,
        hts_pos_t alt_3p_breakpoint_p, Orientation orientation_p,
        std::string inserted_sequence_p, const std::string &ref_string_r,
        const std::string &alt_string_r)
  {
  refContig = ref_name_p;
  altContig = alt_name_p;
  ref_breakpoint= ref_breakpoint_p;
  alt_5p_breakpoint = alt_5p_breakpoint_p;
  alt_3p_breakpoint = alt_3p_breakpoint_p;
  orientation = orientation_p;
  
  // std::string flanking_sequence_5p;
  inserted_sequence = inserted_sequence_p;
  insertion_size = inserted_sequence.length();
  // std::string flanking_sequence_3p;
  
  // switch(orientation){
  // case Orientation::refForAltFor :
  //   {
  //     if(alt_5p_breakpoint >= alt_3p_breakpoint){
  // 	std::cerr << " breakpoints in wrong order " << std::endl;
  // 	std::exit(1);
  //     }
  //     else {
  // 	insertion_size = alt_3p_breakpoint - alt_5p_breakpoint + 1;
  // 	inserted_sequence = alt_string_r.substr(alt_5p_breakpoint, insertion_size);
    /* for (auto i=alt_5p_breakpoint; i<=alt_3p_breakpoint; i++){ */
    /*   inserted_sequence.push_back(alt_string_r[i]); */
    /* } */
    /* std::cout << std::endl; */
	
  //     }
  //   break;
  // }
  // default:
  //   {
  //   std::cerr << "Default case for insertion not yet defined" << std::endl;
  //   }
  // }

  // include 5' and 3' flanking regions of the reference as well
  fivePFlankStartRef = std::max((hts_pos_t)0, ref_breakpoint - ref_breakpoint_flank);
  threePFlankEndRef = std::min((hts_pos_t)alt_string_r.length(), ref_breakpoint + ref_breakpoint_flank );
  flanking_sequence_5p = alt_string_r.substr(fivePFlankStartRef, ref_breakpoint);
  flanking_sequence_3p = alt_string_r.substr(ref_breakpoint, threePFlankEndRef);
  
  }

