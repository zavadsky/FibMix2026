#include "include/IntCodeStats.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>

void writeIntegerCodeStatisticsCsv(const IntegerSequenceText* text,
                                   const UniversalCode* rCode,
                                   const UniversalCode* tCode,
                                   const std::string& outputFileName)
{
    if (text == nullptr || rCode == nullptr || tCode == nullptr)
        throw std::invalid_argument("Null argument passed to writeIntegerCodeStatisticsCsv().");

    std::ofstream out(outputFileName);
    if (!out)
        throw std::runtime_error("Cannot create CSV file: " + outputFileName);

    out << "rank;original_value;frequency;"
        << "FibMixRCode_length_bits;FibMixRCode_total_bits;FibMixRCode_total_bytes;"
        << "FibMixTCode_length_bits;FibMixTCode_total_bits;FibMixTCode_total_bytes\n";

    out << std::fixed << std::setprecision(10);

    double rTotalBits = 0.0;
    double tTotalBits = 0.0;
    uint64_t distinct = 0;

    for (std::size_t value = 0; value < text->Frequencies.size(); ++value) {
        const uint64_t frequency = text->Frequencies[value];
        if (frequency == 0)
            continue;

        if (value >= rCode->lengths.size())
            throw std::runtime_error("FibMixRCode table is too short for value " + std::to_string(value));
        if (value >= tCode->lengths.size())
            throw std::runtime_error("FibMixTCode table is too short for value " + std::to_string(value));

        const double rLen = rCode->lengths[value];
        const double tLen = tCode->lengths[value];
        const double rBits = rLen * static_cast<double>(frequency);
        const double tBits = tLen * static_cast<double>(frequency);

        rTotalBits += rBits;
        tTotalBits += tBits;
        ++distinct;

        out << value << ';' << text->original_value_for_rank(static_cast<uint32_t>(value)) << ';'
            << frequency << ';'
            << rLen << ';' << rBits << ';' << (rBits / 8.0) << ';'
            << tLen << ';' << tBits << ';' << (tBits / 8.0) << '\n';
    }

    out << "TOTAL;;" << text->Nwords << ';'
        << ';' << rTotalBits << ';' << (rTotalBits / 8.0) << ';'
        << ';' << tTotalBits << ';' << (tTotalBits / 8.0) << '\n';

    std::cout << "CSV statistics written to " << outputFileName
              << ". Distinct values: " << distinct << std::endl;
}
