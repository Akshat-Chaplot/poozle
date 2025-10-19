#ifndef FMIndex_HPP
#define FMIndex_HPP

#include <pz_cxx_std.hpp>

class FMIndex{
    private:
        std::string L;
        std::vector<int> C;
        std::vector<std::vector<int>> Occ;
        std::vector<int> compressedSA;
        int a, b, n;
    
    public:
        FMIndex(){}
        FMIndex(std::string& T, int n, int a, int b);
        void build(std::string& T, int n, int a, int b);
        int count(const std::string& p, int m);
        int rank(char ch, int pos);
};

#endif /*FMIndex_HPP*/