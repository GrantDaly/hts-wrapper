#pragma once
#include <cstddef>
#include <istream>
#include <iostream>
#include <cstdio>
#include <memory>
#include <streambuf>

// https://en.cppreference.com/w/cpp/io/basic_streambuf
// https://stackoverflow.com/questions/14086417/how-to-write-custom-input-stream-in-c
class  Gzipped : public std::streambuf {
   private:
  std::streambuf* sbuf_;
    // char*           buffer_;
  char ch;
  // same blocksize as bgzip 65536
  // static constexpr size_t BLOCKSIZE = 0x10000;
  static constexpr size_t BLOCKSIZE = 0x4;

 public :
  Gzipped(std::streambuf* sbuf)
    : sbuf_(sbuf), ch(0){
			   // new char[BLOCKSIZE]) {
        // initialize compression context
    }
  // ~Gzipped(){delete[] this->buffer_;}
  int underflow();
};
int Gzipped::underflow() {
  // from https://en.cppreference.com/w/cpp/io/basic_streambuf/underflow
  traits_type::int_type i;
  i = sbuf_->sbumpc();
  if (!traits_type::eq_int_type(i, traits_type::eof())){
    ch = traits_type::to_char_type(i);
    setg(&ch, &ch, &ch+1);
  }
  return i;
}
// int Gzipped::underflow() {
//   // auto r = sbuf_->readsome(buffer_, 5)
//   // auto pos = sbuf_->getloc(); // position indicator at start of file
//   // std::cout << "eback: " << this->eback() << '\n';
//           // std::cout << "Before underflow(): get area size is " << egptr() - eback()
//         //           << ", put area size is " << epptr() - pbase() << '\n';
//         // int_type ch = std::streambuf::underflow();
//         // std::cout << "After underflow():  get area size is " << egptr() - eback()
//         //           << ", put area size is " << epptr() - pbase() << '\n';
 
//         // if (ch == EOF)
//         //     std::cout << "underflow() returns EOF\n";
//         // else
//         //     std::cout << "underflow() returns '" << char(ch) << "'\n";
//         // return ch;

//   return 0;
// }

class compressbuf
    : public std::streambuf {
private:
    std::streambuf* sbuf_;
    char*           buffer_;
    // context for the compression
public:
    compressbuf(std::streambuf* sbuf)
        : sbuf_(sbuf), buffer_(new char[1024]) {
        // initialize compression context
    }
    ~compressbuf() { delete[] this->buffer_; }
    int underflow() {
        if (this->gptr() == this->egptr()) {
            // decompress data into buffer_, obtaining its own input from
            // this->sbuf_; if necessary resize buffer
            // the next statement assumes "size" characters were produced (if
            // no more characters are available, size == 0.
	  this->setg(this->buffer_, this->buffer_, this->buffer_ );
        }
        return this->gptr() == this->egptr()
             ? std::char_traits<char>::eof()
             : std::char_traits<char>::to_int_type(*this->gptr());
    }
};
