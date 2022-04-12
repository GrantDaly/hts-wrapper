#include <htslib/sam.h>
#include <htslib/faidx.h>
#include <string>
int main() {
std::string refName = "../tests/mpileup-inputs/susScr11.mito.fa";
faidx_t* fasta_idx = fai_load3_format(refName.c_str(),
					  nullptr, nullptr, 0, FAI_FASTA);
return 0;

}
