#include "include/IntFibMCoder.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

IntFibMCoderS::IntFibMCoderS(IntegerSequenceText* t, UniversalCode* c)
    : FibMCoderS(t, c), int_text(t), int_code(c)
{
}

double IntFibMCoderS::emulate()
{
    double ent = 0.0;

    try {
        int_text->rewind_numbers();

        while (!int_text->numbers_eof()) {
            const uint32_t x = int_text->get_number();

            if (x >= int_code->lengths.size()) {
                cout<<"x="<<x<<" lengths="<<int_code->lengths.size()<<endl;
                throw std::runtime_error("The code table is too short for the input number.");
            }

            ent += int_code->lengths[x];
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << std::endl << int_code->c_name;
	printf("\nCode size=%.2f bytes (%.3f MB).\n",ent/8,ent/(8*1024*1024));
    return ent;
}
