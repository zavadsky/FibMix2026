#ifndef INTCODESTATS_H
#define INTCODESTATS_H

#include "IntegerSequenceText.h"
#include "UniversalCode.h"

#include <string>

void writeIntegerCodeStatisticsCsv(const IntegerSequenceText* text,
                                   const UniversalCode* rCode,
                                   const UniversalCode* tCode,
                                   const std::string& outputFileName);

#endif // INTCODESTATS_H
