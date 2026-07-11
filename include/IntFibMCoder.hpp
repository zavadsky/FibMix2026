#pragma once

#include "FibMCoder.hpp"
#include "IntegerSequenceText.h"
#include "UniversalCode.h"

class IntFibMCoderS : public FibMCoderS
{
public:
    IntFibMCoderS(IntegerSequenceText* t, UniversalCode* c);
    double emulate();

private:
    IntegerSequenceText* int_text;
    UniversalCode* int_code;
};
