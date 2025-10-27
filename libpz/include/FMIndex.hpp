#ifndef FMINDEX_HPP
#define FMINDEX_HPP

#include <pz_cxx_std.hpp>

const char SENTINEL_CHAR = 0;
const int ALPHABET_SIZE = 150;

class FMIndex {
public:
  FMIndex(std::string &text, int text_length, int rank_interval,
          int sample_interval);
  int count(const std::string &pattern, int m);
  std::vector<int> locate(const std::string &pattern);

private:
  std::string L;
  std::vector<std::vector<int>> Occ;
  std::vector<int> C;
  std::vector<int> SA_sample;
  int rank_interval;
  int sample_interval;
  int text_length;

  void build(const std::string &text, int text_length, int rank_interval,
             int sample_interval);
  int rank(unsigned char ch, int pos);
  int LF(int i, unsigned char ch) { return C[ch] + rank(ch, i); }
};

#endif
