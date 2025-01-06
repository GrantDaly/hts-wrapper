#include <iostream>
#include <sys/types.h>

#include "cigar.h"
#include "htslib/sam.h"

// using CigarOperation;

char CigarOperation::marshallOperationToChar() const{
  switch(operation)
  {
  case op_t::Match :
    return 'M';
  case op_t::Mismatch:
    return 'X';
  case op_t::Deletion:
    return 'D';
  case op_t::Insertion:
    return 'I';
  case op_t::SoftClip:
    return 'S';
  case op_t::HardClip:
    return 'H';
  default:
    std::cerr << "cigar.cpp: Invalid Cigar Operation" << std::endl;
    std::exit(1);
      // return 'e';
  }
 }

op_t CigarOperation::marshallCharToOperation(const char op_char_p) const{
  switch(op_char_p)
  {
  case 'M':
    return op_t::Match;
  case 'X':
    return op_t::Mismatch;
  case 'D':
    return op_t::Deletion;
  case 'I':
    return op_t::Insertion;
  case 'S':
    return op_t::SoftClip;
  case 'H':
    return op_t::HardClip;
  default:
    std::cerr << "cigar.cpp: Invalid Cigar Operation" << std::endl;
    std::exit(1);
      // return 'e';
  }
}

CigarOperation::CigarOperation(u_int32_t hts_p) {
  // char to internal representation
  char op_char = bam_cigar_opchr(hts_p);
  operation = marshallCharToOperation(op_char);
  // count
  op_count = (int) bam_cigar_oplen(hts_p);

}

std::ostream& operator<<(std::ostream& os, const CigarOperation& cigar)
{
  // os << "test" << ":" << cigar.op_count;
  os << cigar.op_count << cigar.marshallOperationToChar();
  return os;
}


CigarOperationType CigarOperation::getCigar() const { return operation; }

int CigarOperation::getCount() const { return op_count;}

int CigarOperation::incrementCount() { if(op_count >= 0) {
    op_count++;}
  return op_count;
}

int CigarOperation::addToCount(int add_amount) { if(op_count >= 0) {
    op_count = op_count + add_amount;}
  return op_count; }

int CigarOperation::subjectBasesConsumed() const {

  switch(operation){
  case op_t::Match :
    return op_count;
  case op_t::Mismatch:
    return op_count;
  case op_t::Deletion:
    return op_count;
  case op_t::Insertion:
    return 0;
  case op_t::SoftClip:
    return op_count;
  case op_t::HardClip:
    return op_count;
  default:
    std::cerr << "cigar.cpp: Invalid Cigar Operation" << std::endl;
    std::exit(1);
  }
}


int CigarOperation::queryBasesConsumed() const {
      switch(operation){
  case op_t::Match :
    return op_count;
  case op_t::Mismatch:
    return op_count;
  case op_t::Deletion:
    return 0;
  case op_t::Insertion:
    return op_count;
  case op_t::SoftClip:
    return op_count;
  case op_t::HardClip:
    return op_count;
  default:
    std::cerr << "cigar.cpp: Invalid Cigar Operation" << std::endl;
    std::exit(1);
  }
}


int CigarOperation::subjectNumPadPrint() const {

  switch(operation){
  case op_t::Match :
    return 0;
  case op_t::Mismatch:
    return 0;
  case op_t::Deletion:
    return 0;
  case op_t::Insertion:
    return op_count;
  case op_t::SoftClip:
    return op_count;
  case op_t::HardClip:
    return op_count;
  default:
    std::cerr << "cigar.cpp: Invalid Cigar Operation" << std::endl;
    std::exit(1);
  }
}

int CigarOperation::queryNumPadPrint() const {

  switch(operation){
  case op_t::Match :
    return 0;
  case op_t::Mismatch:
    return 0;
  case op_t::Deletion:
    return op_count;
  case op_t::Insertion:
    return 0;
  case op_t::SoftClip:
    return 0;
  case op_t::HardClip:
    return 0;
  default:
    std::cerr << "cigar.cpp: Invalid Cigar Operation" << std::endl;
    std::exit(1);
  }
}
