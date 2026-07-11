#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <map>
#include <set>
#include "include/FibMCoder.hpp"
#include "include/UniversalCode.h"

FibMCoderS::FibMCoderS(WordBasedText* w,UniversalCode* c):text(w),code(c) {
multimap<int, string, std::greater<int> > freq_word;
int i=0;
    for(auto it=w->freq_word.begin();it!=w->freq_word.end();it++,i++)
        word_order_desc.insert(make_pair(it->second,i));
}

double FibMCoderS::emulate() {
string word;
double ent=0;
	try {
        text->text_rewind();
        int size=0;
		while(! text->eof()) {
			word = text->get_token();
            if(word!="") {
                ent+=code->lengths[word_order_desc[word]];
            }
            size++;
            if(size%1000000==0)
                cout<<size/1000000<<" ";
		}
	}   catch (const char *msg) {
		std::cerr << msg << std::endl;
		return EXIT_FAILURE;
	}
	cout<<endl<<code->c_name;
	printf("\nCode size=%.2f bytes (%.3f MB).\n",ent/8,ent/(8*1024*1024));
	return ent;
}

FibMCoderS::~FibMCoderS()
{
    //dtor
}
