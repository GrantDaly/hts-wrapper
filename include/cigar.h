#pragma once

//#include <initializer_list>
#include <ostream>
#include <map>
#include <sys/types.h>
enum class CigarOperationType {
  Match,
    Mismatch,
  Deletion,
  Insertion,
  SoftClip,
  HardClip
};

using op_t = CigarOperationType;
class CigarOperation {
 
 private :
  CigarOperationType operation;
  int op_count = -1;

  // static const std::map<CigarOperationType, char> cigar_to_char_map = {
  //   {op_t::Match, 'M'}, {op_t::Mismatch, 'X'},{op_t::Deletion,'D'},
  //   {op_t::Insertion, 'I'}, {op_t::SoftClip, 'S'}, {op_t::HardClip, 'H'}
  // };
 public:
    /* CigarOperation(); */
  char marshallOperationToChar() const;
  op_t marshallCharToOperation(const char op_char_p) const;
  CigarOperation(op_t op, int op_count) : operation(op), op_count(op_count) {};
  CigarOperation(u_int32_t hts_p);
  friend std::ostream& operator<<(std::ostream& os,const CigarOperation& operation);
  CigarOperationType getCigar() const;
  int getCount() const;
  int incrementCount();
  int addToCount(int add_amount);
  int subjectBasesConsumed() const;
  int queryBasesConsumed() const;
  int subjectNumPadPrint() const;
  int queryNumPadPrint() const;
};
