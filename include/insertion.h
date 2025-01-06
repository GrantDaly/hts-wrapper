#pragma once

#include <iostream>

#include "hts-wrapper-util.h"


// enum class Orientation {refForAltFor, refForAltRev};


// class Insertion {
//  public:
//   std::string refContig;
//   // basically storing this so I know it's in the correct orientation, so no matter what it's in 5'->3' as the reference genome is.
//   std::string inserted_sequence;
//   hts_pos_t ref_breakpoint{0}, 
//     alt_5p_breakpoint{0}, alt_3p_breakpoint{0};
//   hts_pos_t insertion_size{0};

  
//   Insertion(std::string ref_name_p, 
// 		     hts_pos_t ref_breakpoint_p,
// 		     std::string alt_name_p, 
// 		     hts_pos_t alt_5p_breakpoint_p, hts_pos_t alt_3p_breakpoint_p,
// 		     Orientation orientation_p,
// 	    const std::string & alt_string_r);
//     };

// Insertion::Insertion(std::string ref_name_p, 
// 		     hts_pos_t ref_breakpoint_p,
// 		     std::string alt_name_p, 
// 		     hts_pos_t alt_5p_breakpoint_p, hts_pos_t alt_3p_breakpoint_p,
// 		     Orientation orientation_p,
// 		     const std::string & alt_string_r){
//   refContig = ref_name_p;
//   altContig = alt_name_p;
//   ref_breakpoint= ref_breakpoint_p;
//   alt_5p_breakpoint = alt_5p_breakpoint_p;
//   alt_3p_breakpoint = alt_3p_breakpoint_p;
//   orientation = orientation_p;
  
  
//   switch(orientation){
//   case Orientation::refForAltFor :
//     {
//       if(alt_5p_breakpoint >= alt_3p_breakpoint){
// 	std::cerr << " breakpoints in wrong order " << std::endl;
// 	std::exit(1);
//       }
//       else {
// 	insertion_size = alt_3p_breakpoint - alt_5p_breakpoint + 1;
// 	inserted_sequence = alt_string_r.substr(alt_5p_breakpoint, insertion_size);
//     /* for (auto i=alt_5p_breakpoint; i<=alt_3p_breakpoint; i++){ */
//     /*   inserted_sequence.push_back(alt_string_r[i]); */
//     /* } */
//     /* std::cout << std::endl; */
// 	std::cout << "Inserted " << inserted_sequence << std::endl;
//       }
//     break;
//   }
//   default:
//     {
//     std::cerr << "Default case for insertion not yet defined" << std::endl;
//     }
//   }
//   }
