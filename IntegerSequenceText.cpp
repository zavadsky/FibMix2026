#include "include/IntegerSequenceText.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <stdexcept>

IntegerSequenceText::IntegerSequenceText(const std::string& fname)
    : WordBasedText("", 4)
{
    read_binary_uint32_file(fname);
    build_value_frequencies();
}

void IntegerSequenceText::read_binary_uint32_file(const std::string& fname)
{
    std::ifstream file(fname, std::ios::binary);
    if (!file)
        throw std::runtime_error("Cannot open input file: " + fname);

    file.seekg(0, std::ios::end);
    const std::streamoff bytes = file.tellg();
    file.seekg(0, std::ios::beg);

    if (bytes < 0)
        throw std::runtime_error("Cannot determine input file size: " + fname);
    if (bytes % static_cast<std::streamoff>(sizeof(uint32_t)) != 0)
        throw std::runtime_error("Input file size is not divisible by 4 bytes.");

    const std::size_t count = static_cast<std::size_t>(bytes / sizeof(uint32_t));
    values.resize(count);

    if (count != 0)
        file.read(reinterpret_cast<char*>(values.data()), bytes);

    if (!file && count != 0)
        throw std::runtime_error("Error while reading input file: " + fname);

    std::cout << "32-bit integer sequence read. Numbers in the file: "
              << values.size() << std::endl;
}

void IntegerSequenceText::build_value_frequencies()
{
    word_freq.clear();
    word_symbol.clear();
    Frequencies.clear();
    freq_freq.clear();
    freq_word.clear();
    numbers.clear();
    DiffFreq.clear();
    ranks.clear();
    rank_to_value.clear();

    Nwords = static_cast<int>(values.size());
    NFreq = 0;
    diff_words = 0;

    if (values.empty())
    {
        std::cout << "The integer sequence is empty." << std::endl;
        return;
    }

    max_value = *std::max_element(values.begin(), values.end());

    // Sparse table: raw integer value -> frequency.
    // We do not allocate max_value+1 entries, because values can be very large.
    std::map<uint32_t, uint64_t> valueFreq;
    for (uint32_t x : values)
        valueFreq[x]++;

    // Option A: assign a dense rank to every observed value.
    // The ranks are 0,1,...,diff_words-1 in increasing order of the raw value.
    std::map<uint32_t, uint32_t> valueToRank;
    uint32_t rank = 0;
    for (const auto& p : valueFreq)
    {
        valueToRank[p.first] = rank++;
        rank_to_value.push_back(p.first);
    }

    diff_words = static_cast<int>(valueFreq.size());

    ranks.reserve(values.size());
    for (uint32_t x : values)
        ranks.push_back(valueToRank[x]);

    Frequencies.reserve(valueFreq.size());

    std::map<uint64_t, int> frequency_multiplicities;

    // Frequencies[rank] is the frequency of the raw value rank_to_value[rank].
    for (const auto& p : valueFreq)
    {
        Frequencies.push_back(p.second);
        frequency_multiplicities[p.second]++;
    }

    for (const auto& p : frequency_multiplicities)
    {
        if (p.first <= static_cast<uint64_t>(std::numeric_limits<int>::max()))
        {
            freq_freq[static_cast<int>(p.first)] = p.second;
            DiffFreq.push_back(static_cast<int>(p.first));
            ++NFreq;
        }
    }

    std::cout << "<number,frequency> table built. Max number: "
              << max_value
              << "; distinct values/ranks: "
              << diff_words
              << std::endl;
}

uint32_t IntegerSequenceText::get_number()
{
    if (numbers_eof())
        throw std::out_of_range("No more numbers in IntegerSequenceText.");

    // Return the dense rank, not the raw integer value.
    // Raw values are kept in values[] and are still used for Elias statistics.
    return ranks[current_number++];
}

uint32_t IntegerSequenceText::bitLength(uint64_t x)
{
    uint32_t len = 0;

    do {
        ++len;
        x >>= 1;
    } while (x != 0);

    return len;
}

uint64_t IntegerSequenceText::gammaLength(uint64_t x)
{
    const uint32_t L = bitLength(x);
    return 2ULL * L - 1ULL;
}

uint64_t IntegerSequenceText::deltaLength(uint64_t x)
{
    const uint32_t L = bitLength(x);
    return gammaLength(L) + L - 1ULL;
}

uint64_t IntegerSequenceText::omegaLength(uint64_t x)
{
    uint64_t len = 1; // terminating zero

    while (x > 1) {
        const uint32_t L = bitLength(x);
        len += L;
        x = L - 1;
    }

    return len;
}

uint64_t IntegerSequenceText::eliasGammaBits() const
{
    cout << "values.size() = " << values.size() << endl;
    uint64_t total = 0;

    for (uint32_t x : values)
        total += gammaLength(static_cast<uint64_t>(x) + 1ULL);

    return total;
}

uint64_t IntegerSequenceText::eliasDeltaBits() const
{
    uint64_t total = 0;

    for (uint32_t x : values)
        total += deltaLength(static_cast<uint64_t>(x) + 1ULL);

    return total;
}

uint64_t IntegerSequenceText::eliasOmegaBits() const
{
    uint64_t total = 0;

    for (uint32_t x : values)
        total += omegaLength(static_cast<uint64_t>(x) + 1ULL);

    return total;
}

void IntegerSequenceText::printEliasStatistics() const
{
    const uint64_t gammaBits = eliasGammaBits();
    const uint64_t deltaBits = eliasDeltaBits();
    const uint64_t omegaBits = eliasOmegaBits();

    cout << endl;
    cout << "Elias gamma: " << (gammaBits + 7) / 8 << " bytes (" << (double)(gammaBits + 7) / (8*1024*1024) << " MB)" << endl;
    cout << "Elias delta: " << (deltaBits + 7) / 8 << " bytes (" << (double)(deltaBits + 7) / (8*1024*1024) << " MB)" << endl;
    cout << "Elias omega: " << (omegaBits + 7) / 8 << " bytes (" << (double)(omegaBits + 7) / (8*1024*1024) << " MB)" << endl;
}
