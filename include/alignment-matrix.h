#include <cstddef>
#include <stdexcept>
#include <print>

class AlignmentMatrix {

 public:
  AlignmentMatrix(std::vector<char> subject, std::vector<char> query);
  int & operator()(size_t subject_pos, size_t query_pos);
  int operator()(size_t subject_pos, size_t query_pos) const;
  void printMatrix() const;
  ;
 private:
  std::size_t subject_length;
  std::size_t query_length;
  std::vector<char> subject_seq, query_seq;
  std::vector<int> data;

};

// copying strings for now so I don't have to worry about ownership
AlignmentMatrix::AlignmentMatrix(std::vector<char> subject_cpy,
                                 std::vector<char> query_cpy)
{
  subject_length = subject_cpy.size();
  query_length = query_cpy.size();
  subject_seq = subject_cpy;
  query_seq = query_cpy;
  data.resize((subject_length + 1) * (query_length + 1));
}

int & AlignmentMatrix::operator()(size_t subject_pos, size_t query_pos)
{
  if((0 > subject_pos) || (subject_pos >= (subject_length + 1))){
    throw std::out_of_range("AlignmenMatrix access subject out of bounds");
  }
  if((0 > query_pos) || (query_pos >= (query_length + 1))){
    throw std::out_of_range("AlignmenMatrix access query out of bounds");
  }
  return data[subject_pos*(query_length + 1) + query_pos];
};

int AlignmentMatrix::operator()(size_t subject_pos, size_t query_pos) const
{
  if(0 > subject_pos >= (subject_length + 1)){
    throw std::out_of_range("AlignmenMatrix access subject out of bounds");
  }
  else if(0 > query_pos >= (query_length + 1)){
    throw std::out_of_range("AlignmenMatrix access query out of bounds");
  }
  return data[subject_pos*(query_length + 1) + query_pos];
}

void AlignmentMatrix::printMatrix() const {

   // for(int s_dim{0}; s_dim<subject_length+1; s_dim++) {
   //   // std::cout << "Subject " << s_dim << "|";
   //      for(int q_dim{0}; q_dim<query_length+1; q_dim++) {
   // 	  std::cout << "[" << s_dim << "," << q_dim << "]";
   // 	  // std::cout << " " << this->operator()(s_dim, q_dim) << "|";
   // 	}
   //  }
   
     // std::cout << "Subject " << s_dim << "|";
  //       for(int q_dim{0}; q_dim<query_length+1; q_dim++) {
  // 	  for(int s_dim{0}; s_dim<subject_length+1; s_dim++) {
  // 	    std::cout << "[" << s_dim << "," << q_dim << "]";
  // 	}
  //   }
  //  std::cout << std::endl;
  //  for(int q_dim{0}; q_dim<(query_length+1); q_dim++) {
  // 	 std::cout << "<q>";
  // 	 for(int s_dim{0}; s_dim<(subject_length+1); s_dim++) {
  // 	    // std::cout  << this->operator()(s_dim, q_dim) << ",";
  //   	    std::cout  << this->operator()(s_dim, q_dim) << ",";
  // 	   // std::cout << data[s_dim*(query_length + 1) + q_dim] << "|";
  // 	}
  // 	  std::cout << "<\\q>\n";
  //   }
  //  std::cout << std::endl;


   
  // for(int s_dim{0}; s_dim<subject_length+1; s_dim++) {
  //   std::cout << "<s>";
  // 	for(int q_dim{0}; q_dim<query_length+1; q_dim++) {
  // 	    // std::cout  << this->operator()(s_dim, 0) << ",";
  // 	    std::cout  << this->operator()(s_dim, q_dim) << ",";
  // 	  // std::cout << data[s_dim*(query_length + 1) + q_dim] << "|";
  // 	}
  // 	  std::cout << "<\\s>\n";
  //   }
  //  std::cout << std::endl;
   // first print subject across
  std::cout << "        ";
  for(auto subject_i{0}; subject_i<subject_length; subject_i++){
    std::string temp_subject_string{subject_seq[subject_i  ]};
    /* std::printf("|%c|", subject_seq[subject_i]); */
    std::printf("%3s|", temp_subject_string.c_str());
  }
      std::cout << std::endl;
      // first row special because it doesn't align with query string chars
    std::cout << "    ";
    for(auto subject_i{0}; subject_i<subject_length + 1; subject_i++){
    std::printf("%3d|", this->operator()(subject_i, 0));
  }
    std::cout << std::endl;
      // output query char, then go through subject size row for that entry
  for(auto query_i{0}; query_i<query_length; query_i++){
    std::string temp_query_string{query_seq[query_i]};
    /* std::printf("|%c|", query_seq[query_i]); */
    std::printf("%3s|", temp_query_string.c_str());
  for(auto subject_i{0}; subject_i<subject_length + 1; subject_i++){
    std::printf("%3d|", this->operator()(subject_i, query_i+1));
      }
  std::cout << std::endl;
  }
  
}
