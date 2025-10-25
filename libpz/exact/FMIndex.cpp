#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <FMIndex.hpp>

using namespace std;
using namespace std::chrono;
const char us = 0;
const int N = 150;

static void count_sort(vector<int>& p, vector<int>& c){
    int n = p.size();
    vector<int> cnt(n), pos(n);
    for(auto x : c){
        cnt[x]++;
    }
    pos[0] = 0;
    for(int i = 1; i < n; i++){
        pos[i] = pos[i-1] + cnt[i-1];
    }
    vector<int> p_new(n);
    for(auto x : p){
        int i = c[x];
        p_new[pos[i]] = x;
        pos[i]++;
    }
    p = p_new;
}

static vector<int> suffix_array(string& s){
    s+=us;
    int len = s.length();
    vector<int> p(len), c(len);
    vector<pair<char, int>> a(len);
    for(int i = 0; i < len; i++){
        a[i] = {s[i], i};
    }
    sort(a.begin(), a.end());
    for(int i = 0; i < len; i++){
        p[i] = a[i].second;
    }
    c[p[0]] = 0; // c[j] stores 
    for(int i = 1; i < len; i++){
        if(a[i].first == a[i-1].first){
            c[p[i]] = c[p[i-1]];
        }
        else{
            c[p[i]] = c[p[i-1]] + 1;
        }
    }
    int k = 0; //we have strings temp upto 2^k prefix letters
    while((1 << k) < len){
        for(int i = 0; i < len; i++){
            p[i] = (p[i] - (1 << k) + len)%len;
        }
        count_sort(p, c);
        vector<int> c_new(len);
        c_new[p[0]] = 0; // c[j] stores 
        for(int i = 1; i < len; i++){
            pair<int, int> prev = {c[p[i-1]], c[(p[i-1] + (1 << k))%len]};
            pair<int, int> now = {c[p[i]], c[(p[i] + (1 << k))%len]};
            if(prev == now){
                c_new[p[i]] = c_new[p[i-1]];
            }
            else{
                c_new[p[i]] = c_new[p[i-1]] + 1;
            }
        }
        c = c_new;
        k++;
    
    }
    s.pop_back();
    return p;
}

FMIndex::FMIndex(string& T, int n, int a, int b){
    L.resize(n);
    vector<int> sa = suffix_array(T);
    for(int i = 0; i < n; i++){
        L[i%n] = T[sa[i]-1];
    }
    Occ.resize(n/a + 1, vector<int>(N, 0));
    for(int i = 0; i <= N; i++){
        C[i] = 0;
    }
    this->a = a;
    this->b = b;
    this->n = n;
    build(T, n, a, b);
}

void FMIndex::build(string& T, int n, int a, int b){

    int prev, temp;
    for(int i = 0; i < n; i++){
        C[T[i]]++;
    }
    prev = C[0];
    C[0] = 0;
    for(int i = 1; i <= N; i++){
        temp = C[i];
        C[i] = prev + C[i-1];
        prev = temp;
    }
    vector<int> tmp(N+1, 0);

    for(int i = 0; i < N; i++){
        Occ[0][i] = 0;
    }

    for(int i = 1; i <= n; i++){
        for(int j = 0; j < N; j++){
            if(L[i-1] == j){
                tmp[j]++;
            }
            if(i%a == 0){
                int idx = i/a;
                Occ[idx][j] = Occ[idx-1][j] + tmp[j];
                tmp[j] = 0;
            }
        }
    }
}

int FMIndex::count(const string& pattern, int m){
    char ch = pattern[m-1];
    int s = C[ch];
    int e = C[ch+1]-1;
    int i = m-2;
    while(s <= e && i >= 0){
        ch = pattern[i];
        s = C[ch] + rank(ch, s) + 1;
        e = C[ch] + rank(ch, e+1);
        i--;
    }
    if(s <= e){
      return e-s+1;
    }
    else{
      return 0;
    }
}

int FMIndex::rank(char ch, int pos){
    int cnt = 0;
    if(pos%a >= a/2 && pos <= a*(n/a)){
        while(1){
            if(pos%a == 0){
                return Occ[pos/a][ch] - cnt - 1;
            }
            if(L[pos-1] == ch){
                cnt++;
            }
            pos++;
        }
    }
    else{
        while(1){
            if(pos%a == 0){
                return Occ[pos/a][ch] + cnt - 1;
            }
            if(L[pos-1] == ch){
                cnt++;
            }
            pos--;
        }
    }
}
