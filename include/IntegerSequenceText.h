#ifndef INTEGERSEQUENCETEXT_H
#define INTEGERSEQUENCETEXT_H

#include "WordBasedText.h"
#include <cstdint>
#include <string>
#include <vector>

class IntegerSequenceText : public WordBasedText
{
public:
    explicit IntegerSequenceText(const std::string& fname);

    void rewind_numbers() { current_number = 0; }
    bool numbers_eof() const { return current_number >= values.size(); }
    uint32_t get_number();
    uint32_t max_number() const { return max_value; }
    uint32_t distinct_number_count() const { return static_cast<uint32_t>(rank_to_value.size()); }
    uint32_t original_value_for_rank(uint32_t rank) const { return rank_to_value.at(rank); }

    uint64_t eliasGammaBits() const;
    uint64_t eliasDeltaBits() const;
    uint64_t eliasOmegaBits() const;
    void printEliasStatistics() const;

private:
    // Original integers from the input file. Elias statistics are computed on these raw values.
    std::vector<uint32_t> values;

    // Ranks of the original integers. Table-based FibMix/BCMix codes encode these ranks,
    // because their code tables are dense and indexed from 0 to diff_words-1.
    std::vector<uint32_t> ranks;
    std::vector<uint32_t> rank_to_value;

    std::size_t current_number = 0;
    uint32_t max_value = 0;

    void read_binary_uint32_file(const std::string& fname);
    void build_value_frequencies();

    static uint32_t bitLength(uint64_t x);
    static uint64_t gammaLength(uint64_t x);
    static uint64_t deltaLength(uint64_t x);
    static uint64_t omegaLength(uint64_t x);
};

#endif // INTEGERSEQUENCETEXT_H
