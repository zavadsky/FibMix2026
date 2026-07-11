#include <string.h>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include "include/UniversalCode.h"
#include "include/WordBasedText.h"
#include "include/FibMCoder.hpp"
#include "include/IntegerSequenceText.h"
#include "include/IntFibMCoder.hpp"
#include "include/IntCodeStats.h"

using namespace std;

int main(int argc, char** argv) {
    if(argc<2)
        cout<<"Incorrect number of command line arguments.";
    else {
        string ifname(argv[1]);
        bool int32_mode = (argc >= 3 && string(argv[2]) == "int32");

        if (int32_mode) {
            IntegerSequenceText *wa = new IntegerSequenceText(ifname);
            wa->printEliasStatistics();
            cout<<endl<<"================ Encoding uint32_t sequence. =================="<<endl;
            FibUCode* u=new FibUCode(3);
            IntFibMCoderS F(wa,u);
            F.emulate();
            // Mixed d-ary Fibonacci codes
            wa->precalcBestBlocks();
            FibMixTCode* u3=new FibMixTCode(wa);
            IntFibMCoderS F3(wa,u3);
            F3.emulate();
            // BCMix code
            wa->precalcBestBlocks_bmix();
            BCMix* bmix=new BCMix(wa);
            IntFibMCoderS Fbc(wa,bmix);
            Fbc.emulate();
            cout<<endl;
        } else {
            WordBasedText *wa;
            wa = new WordBasedText(ifname,1);   // Pre-process the text
            wa->printEliasStats();
            cout<<endl<<"================ Encoding text. =================="<<endl;
            // Fibonacci Higher Order code
            FibUCode* u=new FibUCode(3);
            FibMCoderS F(wa,u);
            F.emulate();
            // Mixed d-ary Fibonacci codes
            //precalcBestBlocks();
            cout<<endl<<"===================================================="<<endl;
            wa->setBestBlocks({2,7,2,3,3,2,3,2,3,2,2,2,3});
            FibMixTCode* u3=new FibMixTCode(wa);
            FibMCoderS F3(wa,u3);
            F3.emulate();
            // BCMix code
            cout<<endl<<"===================================================="<<endl;
            wa->precalcBestBlocks_bmix();
            BCMix* bmix=new BCMix(wa);
            FibMCoderS Fbc(wa,bmix);
            Fbc.emulate();
            cout<<endl;
        }
    }
	system("pause");
}

