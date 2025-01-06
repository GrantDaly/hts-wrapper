#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>

#include "cigar.h"
#include "alignment-matrix.h"

// std::pair<std::vector<CigarOperation>, std::vector<CigarOperation>>
void
  alignTwoSequences(const std::vector<char> & subject,
		    const std::vector<char> & query)
{
  const int soft_clip_penalty{0};
  const int indel_penalty{0};
  const int mismatch_penalty{-1};
  const int match_reward{1};

  const auto q_size = query.size();
  const auto s_size = subject.size();

  // since local alignment track max and coordinates of max
  int max_score_obs = -1;
  int subject_pos_max = -1;
  int query_pos_max = -1;

  // left clip eligible as long as a mismatch hasn't been included yet. can always start a right clip
  bool q_left_eligible = false;
  bool q_right_eligible = false;

  if((q_size > 1000) | (s_size > 10000)){
    std::cerr << "Query or Subject size too large, don't want to blow stack" << std::endl;
    std::exit(1);
  }
  
  // int align_matrix[s_size + 1][q_size + 1];

  AlignmentMatrix align_matrix = AlignmentMatrix(subject, query);

  for(int s_dim{1}; s_dim<(s_size+1); s_dim++) {
    // advancing on subject, not query (soft clip, no penalty)
    align_matrix(s_dim,0) = 0;
  }

  // advancing on query not subject (on one end) describes soft clip
  for(int q_dim{1}; q_dim<(q_size+1); q_dim++) {
    align_matrix(0,q_dim) = 0;
  }


  //now iterate down and over starting at one
  for(int s_dim{1}; s_dim<=s_size; s_dim++) {
    for(int q_dim{1}; q_dim<=q_size; q_dim++)
      {	
	auto subject_char =  subject[s_dim-1];
        const auto query_char =  query[q_dim-1];
	
        auto  s_dim_score = align_matrix(s_dim,q_dim-1) + indel_penalty;
	int q_dim_score{-1};
	if(q_left_eligible){
	  q_dim_score = align_matrix(s_dim-1,q_dim) + soft_clip_penalty;
	}
	else {
	q_dim_score = align_matrix(s_dim-1,q_dim) + indel_penalty;
	}
	
	decltype(q_dim_score)  diag_score{0};
	if(subject_char == query_char){
	  diag_score = align_matrix(s_dim-1,q_dim-1) + match_reward;
	}
	else
	  {
	    diag_score = align_matrix(s_dim-1,q_dim-1) + mismatch_penalty;
	  }
	// normally just want max score but I need to know if diagonal won so I can make left clipping ineligible
	// if(diag_score > q_dim_score) {
	//   std::cout << "ineligible" << std::endl;
	//   q_left_eligible = false;
	// }
	  
	const auto current_score = std::max({diag_score, s_dim_score,q_dim_score});
	align_matrix(s_dim,q_dim) = current_score;

	if(current_score >= match_reward) {
	  std::cout << "ineligible" << std::endl;
	  q_left_eligible = false;
	}
	// check if score greater than current max
	if(current_score > max_score_obs) {
	  max_score_obs = current_score;
	  subject_pos_max = s_dim;
	  query_pos_max = q_dim;
	}
	  
      }
      }

  align_matrix.printMatrix();

  std::cout << "Max Score " << max_score_obs << "Subject: " << subject_pos_max
	    << "Query: " << query_pos_max << std::endl;
  // now that scores are generated, walk through and record softclip, indel (?), match, mismatch
    // int sub_remaining = s_size;
    // int query_remaining = q_size;
    // int s_dim=0;
    // int q_dim=0;
    // while(query_remaining > 0){
    //  // align_matrix[s_dim][q_dim]
    //  auto subject_char =  subject[s_dim];
    //  const auto query_char =  query[q_dim-1];
    //  auto  s_dim_score = align_matrix[s_dim][q_dim-1] + indel_penalty;
    //  auto q_dim_score = align_matrix[s_dim-1][q_dim] + indel_penalty;
    //  query_remaining++;
    // }

  
}

int main() {

  std::vector<char> subject{'h','e','l','l','o',',','w','o','r','l','d'};
  std::vector<char> query{'w','o','r'};

  alignTwoSequences(subject, query);

  std::vector<char> subject2{'X','X','X','X','Y','Z','A','B','C','D','X','Y','Z'};
  std::vector<char> query2{'a','a','a','A','B','C','a','a','a'};
  alignTwoSequences(subject2, query2);
  return 0;
}
