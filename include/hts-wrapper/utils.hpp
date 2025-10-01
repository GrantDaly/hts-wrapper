#pragma once
#include "cigar.h"

// eventually these may become std::views or similar

// take cigar operations and merge the same, eg. =1,=1, =1,M1,=1,=1 -> =3,M1,=2

std::vector<CigarOperation>
deflate_cigar_vector(const std::vector<CigarOperation> & inflated) {
  std::vector<CigarOperation> deflated;

  for(const auto & cigar: deflated) {
    std::cout << cigar << std::endl;
  }

}
