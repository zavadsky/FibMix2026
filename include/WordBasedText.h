#ifndef WORDBASEDTEXT_H
#define WORDBASEDTEXT_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <math.h>
#include "UniversalCode.h"

using namespace std;


class WordBasedText
{
    typedef void (WordBasedText::*BestBlockSearchFunc)(
    uint8_t[], uint32_t, uint64_t, uint64_t, uint64_t,
    double, double);

    public:
        int glob=0;
        WordBasedText(string,char);
        virtual ~WordBasedText();
        int word_frequences();
        map<string,int> word_freq; // <word,frequency> map
        void text_rewind(){buffer.clear(std::stringstream::goodbit); buffer.seekg(0);};
        bool eof(){return buffer.eof();};
        string get_word();
        string get_char();
        int getMaxSymb(){return diff_words;};
        map<string,int> word_symbol; // map <word; number in the list, ordered by frequency>
        vector<uint64_t> Frequencies; // vector of descending frequencies of all words
        map<int,int> freq_freq; // <frequency, number of words of this frequency> map
        int Nwords; //total number of words in the text
        void output_stream(string);
        vector<int> numbers;
        void prepare_adaptive();
        int diff_words=0;
        multimap<int,string,greater<int>> freq_word; // <frequency,word> multimap
        /*void precalcBestBlocksR();
        void precalcBestBlocksT();
        void precalcBestBlocks(BestBlockSearchFunc searchFunc);*/
        void precalcBestBlocks();
        void precalcBestBlocks_bmix();
        static const int digitArSize=400;
        uint8_t bestDigitsSz[digitArSize];
        string get_token();
        void setBestBlocks(const std::vector<uint8_t>&);
        void BCS(uint8_t dSz[24], uint32_t, uint64_t, int64_t,int64_t, double, double);
        void BCSR(uint8_t dSz[24], uint32_t, uint64_t, uint64_t,uint64_t, double, double);
        void printEliasStats() const;

    protected:
        vector<int> DiffFreq; // vector of distinct frequencies
        int NFreq;   // number of distinct frequencies
        char alpha_num; // # - punctuation; 0 - non-alphanumeric; 4 - character-based; other - alphanumeric;
        bool if_char;

    private:
        vector<uint8_t> lower_bounds;
        vector<uint8_t> upper_bounds;
        uint32_t* PSum;
        stringstream buffer;
        uint32_t bestBlocksNum;
        double bestFullBitsSz;
        double entropy;
        uint64_t getSum_bmix(uint32_t, uint32_t);
        void BCS_bmix(uint8_t dSz[24], uint32_t, uint64_t,uint64_t,uint64_t,uint64_t);
        uint64_t getSum(uint32_t, uint32_t);
        uint8_t getLowerBound(uint32_t) const;
        uint8_t getUpperBound(uint32_t) const;
        void refineBoundsAfterSearch(int);
        static uint32_t bitLength(uint64_t x);

        static uint64_t eliasGammaLen(uint64_t x);
        static uint64_t eliasDeltaLen(uint64_t x);
        static uint64_t eliasOmegaLen(uint64_t x);

};



#endif // WORDBASEDTEXT_H
