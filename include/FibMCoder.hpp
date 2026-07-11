#pragma once

#include <vector>
#include "UniversalCode.h"

class WordBasedText;

using namespace std;


class FibMCoderS {
    public:
        FibMCoderS(WordBasedText*,UniversalCode*);
        virtual ~FibMCoderS();
        double emulate();
        WordBasedText* text;
    protected:

    private:
        UniversalCode *code;
        map<string, int> word_order_desc; //<word; its number in the sequence of words ordered by descending frequency>

};
