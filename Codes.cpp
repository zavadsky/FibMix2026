#include <iterator>
#include "include/WordBasedText.h"
#include "include/UniversalCode.h"


FibUCode::FibUCode(int x):d(x),UniversalCode("Higher order Fibonacci code ") {
int un=1,un_1=0,n=0;
    for(int i=0;n<Max_code;i++) {
        for(int k=0;k<un;k++)
            lengths.push_back((i+2)*log2(d));
        n+=un;
        int t=un;
        un=un*d-un_1;
        un_1=t;
    }
    cout<<endl;
}

Fib3Code::Fib3Code():UniversalCode("Fibonacci code Fib3 ") {
int un=1,un1=0,un2=0,n=0;
     for(int i=3;n<Max_code;i++) {
        for(int k=0;k<un;k++)
            lengths.push_back(i);
        n+=un;
        int q=un;
        un+=un1+un2;
        un2=un1;
        un1=q;
    }
    cout<<endl;
}

Fib2Code::Fib2Code():UniversalCode("Fibonacci code Fib2 ") {
int un=1,un1=0,n=0;
     for(int i=2;n<Max_code;i++) {
        for(int k=0;k<un;k++) {
            lengths.push_back(i);
            n++;
        }
        int q=un;
        un+=un1;
        un1=q;
    }
    cout<<endl;
}

FibMixTCode::FibMixTCode(WordBasedText* w):UniversalCode("Mixed d-ary Fibonacci code ") {
for(int i=0;i<w->digitArSize;i++)
    digits.push_back(w->bestDigitsSz[i]);
int un=1,un_1=0,n=0,r,x,i=1,cur_dig=0,prev_digit=digits[0];
double cur_len=log2(digits[0]);
double sum_len=0,kraft=(double)1/digits[0];
    for(int j=1;n<w->diff_words;j++) {
            cur_len+=log2(digits[j]);
            kraft/=digits[j];
            sum_len+=(double)un*kraft;
            for(int k=0;k<un;k++)
                lengths.push_back(cur_len);
            n+=un;
            int q=un;
            un=un*prev_digit-un_1;
            un_1=q;
            prev_digit=digits[j];
    }
    //cout<<endl<<"Kraft="<<sum_len<<endl;
}

FibMixRCode::FibMixRCode(WordBasedText* w):UniversalCode("Mixed d-ary Fibonacci-R code ") {
for(int i=0;i<w->digitArSize;i++)
    digits.push_back(w->bestDigitsSz[i]);
int un=digits[0]-1,r1=digits[0],r2=1,n=0,x,i=1,cur_dig=0;
double cur_len=log2(digits[0]);
double sum_len=0,kraft=(double)1/digits[0];
    for(int j=1;n<w->diff_words;j++) {
            cout<<"digits["<<j+1<<"]="<<digits[j]<<" un["<<j+1<<"]="<<un;
            cur_len+=log2(digits[j]);
            cout<<" cur_len="<<cur_len<<" n="<<n<<endl;
            kraft/=digits[j];
            sum_len+=(double)un*kraft;
            for(int k=0;k<un;k++)
                lengths.push_back(cur_len);
            n+=un;
            int q=r1;
            r1 = r2 + (digits[j]-1)*r1;
            r2 = q;
            un = r1 - r2;
    }
    cout<<endl<<"Kraft="<<sum_len<<endl;
}
