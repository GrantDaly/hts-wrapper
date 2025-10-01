#include <bam-record.h>
#include <memory>

class ReadPair
{
  public:
  std::unique_ptr<BamRecord> read_one;
  std::unique_ptr<BamRecord> read_two;
  
}
