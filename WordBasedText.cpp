#include "include/WordBasedText.h"

#define shortestDigitLen 2
#define longestDigitLen 3
#define longestDigitLen2 10
#define totalDigits 240
//#define bound0 4
#define bound1 14

WordBasedText::WordBasedText(string fname,char t)
{
    lower_bounds = {2,2,2};
    upper_bounds = {22,32,16};
    alpha_num=t;
    if_char = (t==4);

    if(fname.empty())
        return;

    std::ifstream file(fname);
    buffer << file.rdbuf();
    cout<<"Data read."<<endl;
    word_frequences();
}

uint8_t WordBasedText::getLowerBound(uint32_t dN) const {return dN < lower_bounds.size() ? lower_bounds[dN] : 2;};
uint8_t WordBasedText::getUpperBound(uint32_t dN) const {return dN < upper_bounds.size() ? upper_bounds[dN] : 2;};

bool isalnum0(char c,char prev) {
    return isalnum(c) || c=='-' || (int(c)==39 && isalnum(prev));
}

// Reading a word from alpha-numeric or punctuation stream
string WordBasedText::get_word() {
string sa="",sn="";
char a,c=0,prev;
    if(buffer.eof())
        return "";
    if(alpha_num==2) {
        buffer>>sa;
        return sa;
    }
    if(alpha_num=='#') {
        buffer>>noskipws>>c;
        if(c) {
        sn=c;
        while(c!='#' && !buffer.eof()) {
            buffer>>noskipws>>c;
            sn+=c;
        }
        }
        return sn;
    }
    do {
        prev=c;
        buffer>>noskipws>>c;
        sn+=c==10?'#':c;
    } while(!isalnum0(c,prev) && ! buffer.eof());
    if(buffer.eof())
        return sn;
    buffer.unget();
    sn=sn.substr(0,sn.length()-1);
    do {
        prev=c;
        buffer>>noskipws>>c;
        sa+=c;
    } while(isalnum0(c,prev) && ! buffer.eof());
    if(buffer.eof())
        return sa;
    buffer.unget();
    if(alpha_num) {
        sa=sa.substr(0,sa.length()-1);
        return sa;
    }
    if(sn!="")
        return sn;
    do {
        prev=c;
        buffer>>noskipws>>c;
        sn+=c==10?'#':c;
    } while(!isalnum0(c,prev) && ! buffer.eof());
    if(buffer.eof())
        return sn;
    buffer.unget();
    sn=sn.substr(0,sn.length()-1);
    return sn;
}

string WordBasedText::get_char() {
char c;
    buffer.get(c);
    return string(1, c);
}

string WordBasedText::get_token() {
    return if_char?get_char():get_word();
}

//Create different maps and dictionaries
int WordBasedText::word_frequences() {
double pi,sum1=0,sum2=0;
string word;
int i=0,k=0;
    Nwords=0;
    // Create the map <word,frequency> - word_freq
	while ( ! buffer.eof() ) {
        word = get_token();
		if(word_freq.find(word)!=word_freq.end()) {
			word_freq[word]++;
        } else {
			word_freq.insert(make_pair(word,1));
		}
		Nwords++;
		if(Nwords%1000000==0)
            cout<<Nwords/1000000<<" ";
    }
	cout<<endl<<"Input file processed. <word,frequency> map buit. Words in the text: "<<Nwords<<endl;
	// Create the multimap <frequency,word> consisting all different words - freq_word
	for(auto it=word_freq.begin();it!=word_freq.end();it++,diff_words++) {
		freq_word.insert(make_pair(it->second,it->first));
		// Calculate Shannon entropy
		pi=(double)it->second;
		entropy-=pi*(long double)log2((long double)pi/Nwords);
	}
    cout<<"<frequency,word> map built "<<sum1<<" "<<sum2<<endl;
	// Create 1) map <word,symbol> which maps words of text to integers according to descending order of their frequencies;
	// 2) vector of Frequencies of all unique words; 3) vector of different Frequencies DiffFreq; 4) map freq_freq<frequency, number of words having this frequency>
int j=-1;
int frq=0;
NFreq=0;
multimap<int,string> :: iterator it1;
	for(it1=freq_word.begin(),i=0;it1!=freq_word.end();i++,it1++) {
        if(it1->first!=frq) {
            if(frq)
                freq_freq.insert(make_pair(frq,i-j));
            frq=it1->first;
            j=i;
            DiffFreq.push_back(frq);
            NFreq++;
        }
		word_symbol.insert(make_pair(it1->second,i)); //insert first element from freq_word map
		Frequencies.push_back(it1->first);
	}
    freq_freq.insert(make_pair(frq,i-j));
	cout<<"<word,symbol> map built. Different words in text: "<<diff_words<<". Entropy H0="<<(int)entropy/8<<" bytes."<<endl;
	return Nwords;
}

bool digits_found;

void WordBasedText::BCS(uint8_t dSz[digitArSize], uint32_t dN, uint64_t fullN,
	int64_t incN1,int64_t incN2, double cLen, double fullSz)
{
	if(dN>totalDigits)
        return;


        if (fullSz > bestFullBitsSz)
            return;

	if (dN > bound1)        // All digits starting from 12th = 2
	{
		if (fullN > diff_words)       // Line 1 of Alg. 1
		{
			//cout<<(int)dSz[0]<<" "<<(int)dSz[1]<<" "<<fullSz<<endl;
			dSz[dN] = 2;
			digits_found = true;

			if (fullSz < bestFullBitsSz)    // Line 2 of Alg. 1
			{
				bestFullBitsSz = fullSz;
				bestBlocksNum = dN;
				for (uint32_t i = 0; i < digitArSize; i++)
				{
					bestDigitsSz[i] = dSz[i];
				}
			}
			return;                         // Line 3 of Alg. 1
		}

		dSz[dN] = 2;
		double newSz = fullSz + (cLen + 1) * getSum(fullN, fullN + incN1 - 1);
		BCS(dSz, dN + 1, fullN + incN1, 2*incN1 - incN2, incN1, cLen + 1, newSz);
	}
	else {
        int low = getLowerBound(dN);
        int high = getUpperBound(dN);
        for (int i = low; i <= high; i++) {     // Line 5 of Alg. 1
            dSz[dN] = i;                            // Line 6 of Alg. 1
            // Line 7 of Alg. 1
            double newSz = fullSz + (cLen + log2(i)) * getSum(fullN, fullN + incN1 - 1);
            // Line 8 of Alg. 1
            BCS(dSz, dN + 1, fullN + incN1, dN>0?dSz[dN-1]*incN1 - incN2:1, incN1, cLen + log2(i), newSz);
		}
	}
}

void WordBasedText::BCSR(uint8_t dSz[digitArSize], uint32_t dN, uint64_t fullN,
	uint64_t r1,uint64_t r2, double cLen, double fullSz)
{
	if(dN>totalDigits)
        return;
	if (dN > bound1)        // All digits starting from 12th = 2
	{
		if (fullN > diff_words)       // Line 1 of Alg. 1
		{
			dSz[dN] = 2;
			digits_found = true;

			if (fullSz < bestFullBitsSz)    // Line 2 of Alg. 1
			{
				bestFullBitsSz = fullSz;
				bestBlocksNum = dN;
				for (uint32_t i = 0; i < digitArSize; i++)
				{
					bestDigitsSz[i] = dSz[i];
				}
			}
			return;                         // Line 3 of Alg. 1
		}

		dSz[dN] = 2;
		double newSz = fullSz + (cLen + 1) * getSum(fullN, fullN + r1 - r2 - 1);
		BCSR(dSz, dN + 1, fullN + r1 - r2, r1+r2, r1, cLen + 1, newSz);
	}
	else {
        int low = getLowerBound(dN);
        int high = getUpperBound(dN);

        for (int i = low; i <= high; i++) {     // Line 5 of Alg. 1
            dSz[dN] = i;                            // Line 6 of Alg. 1
            // Line 7 of Alg. 1
            double newSz = fullSz + (cLen + log2(i)) * getSum(fullN, fullN + r1 - r2 - 1);
            // Line 8 of Alg. 1
            BCSR(dSz, dN + 1, fullN + r1 - r2, r1*(i-1)+r2, r1, cLen + log2(i), newSz);
		}
	}
}

void WordBasedText::refineBoundsAfterSearch(int low)
{
    size_t m = bestBlocksNum + 1;//max(lower_bounds.size(), upper_bounds.size());

    if (lower_bounds.size() < m)
        lower_bounds.resize(m, 2);

    if (upper_bounds.size() < m)
        upper_bounds.resize(m, 2);

    for (size_t i = 0; i < m; i++)
    {
        uint8_t d = bestDigitsSz[i];

        if (d == upper_bounds[i]) {
            lower_bounds[i] = upper_bounds[i];
            upper_bounds[i] += 4;
        } else if (d == lower_bounds[i]) {
            upper_bounds[i] = lower_bounds[i];
            if(lower_bounds[i] > low)
                lower_bounds[i]--;
        }
    }

    if (m > 0 && bestDigitsSz[m - 1] != low)
    {
        lower_bounds.push_back(low);
        upper_bounds.push_back(6);   // new digit becomes searchable
    }
}

// Precalculation for searching the optimal Mixed d-ary Fibonacci code
void WordBasedText::precalcBestBlocks(/*BestBlockSearchFunc searchFunc*/)
{
const int l1=2,l2=2,u1=20,u2=20;

bestFullBitsSz = UINT64_MAX;

cout<<endl<<"========================================"<<endl;
    PSum = new uint32_t[diff_words * 5 + 1];

    for (int i = 0; i <= diff_words * 2; i++)
        PSum[i] = 0;

    PSum[0] = Frequencies[0];

    for (uint32_t i = 1; i <= diff_words; i++)
    {
        if (i < diff_words)
            PSum[i] = PSum[i - 1] + Frequencies[i];
        else
            PSum[i] = PSum[i - 1];
    }

    bestFullBitsSz = UINT64_MAX;

    uint8_t digitLen[digitArSize];

    for (int i = 0; i < digitArSize; i++) {
        digitLen[i] = 2;
        bestDigitsSz[i] = 2;
    }

    BCS(digitLen, 0, 0, 0, -1, 0, 0);

    for (int i = 0; i < 20; i++)
{
    int low =
        (i < (int)lower_bounds.size()) ?
        lower_bounds[i] : 2;

    int high =
        (i < (int)upper_bounds.size()) ?
        upper_bounds[i] : 2;

}

    lower_bounds = {bestDigitsSz[0],bestDigitsSz[1],bestDigitsSz[2]};
    upper_bounds = {bestDigitsSz[0],bestDigitsSz[1],bestDigitsSz[2]};


    const int K = 30;

for (int iter = 0; iter < K; iter++)
{
    bestFullBitsSz = UINT64_MAX;

    for (int i = 0; i < digitArSize; i++)
    {
        digitLen[i] = 2;
        bestDigitsSz[i] = 2;
    }

    digits_found = false;

     BCS(digitLen, 0, 0, 0, -1, 0, 0);


    if (!digits_found)
    {
        cout << "!!!!!!!!!!!!! Code not found !!!!!!!!!" << endl;
        break;
    }

for (int i = 0; i < 20; i++)
{
    int low =
        (i < (int)lower_bounds.size()) ?
        lower_bounds[i] : 2;

    int high =
        (i < (int)upper_bounds.size()) ?
        upper_bounds[i] : 2;
}

cout<<".";

    refineBoundsAfterSearch(2);
}

    cout << endl << "Optimal Mixed d-ary Fibonacci code: " << endl;

    for (uint32_t i = 0; i < digitArSize; i++)
        cout << (int)bestDigitsSz[i] << " | ";

    delete[] PSum;
}


// Difference between PSum values in line 8 of Alg. 1
uint64_t WordBasedText::getSum(uint32_t l, uint32_t r)
{
	if (l > diff_words)
		return 0;

	uint64_t res = 0;
	res += r > diff_words ? PSum[diff_words] : PSum[r];
	res -= l == 0 ? 0 : PSum[l - 1];

	return res;
}

// Difference between PSum for BCMix codes
uint64_t WordBasedText::getSum_bmix(uint32_t l, uint32_t r)
{
	if (l > static_cast<uint32_t>(diff_words))
		return 0;

	uint64_t res = 0;

	res += r > static_cast<uint32_t>(diff_words) ? PSum[diff_words] : PSum[r];
	res -= l == 0 ? 0 : PSum[l - 1];

	return res;
}

// Searching the optimal code for BCMix codes
void WordBasedText::BCS_bmix(uint8_t* dSz, uint32_t dN, uint64_t fullN,
	uint64_t incN, uint64_t cLen, uint64_t fullSz)
{
    if(dN>=totalDigits)
        return;
	if (fullN > diff_words) {
			dSz[dN] = 2;
			digits_found = true;
			if (fullSz < bestFullBitsSz)
			{
				bestFullBitsSz = fullSz;
				bestBlocksNum = dN-1;
				for (uint32_t i = 0; i < digitArSize; i++)
				{
					bestDigitsSz[i] = dSz[i];
				}
			}
			return;
    }
int lowestDigit = getLowerBound(dN);
int highestDigit = getUpperBound(dN);
    for (int i = lowestDigit; i <= highestDigit; i++) {
            dSz[dN] = i;
            uint64_t newSz = fullSz + (cLen + i) * getSum_bmix(fullN, fullN + incN - 1);
            BCS_bmix(dSz, dN + 1, fullN + incN, incN * ((1 << i)-1), cLen + i, newSz);
		}
}


void WordBasedText::precalcBestBlocks_bmix()
{

    lower_bounds = {1, 1, 1, 1, 1, 1, 1, 1};
    upper_bounds = {4, 4, 4, 4, 2, 2, 2, 2};

    PSum = new uint32_t[diff_words * 5 + 1];

    for (int i = 0; i <= diff_words * 5; i++)
        PSum[i] = 0;

    PSum[0] = Frequencies[0];

    for (uint32_t i = 1; i <= diff_words; i++)
    {
        if (i < diff_words)
            PSum[i] = PSum[i - 1] + Frequencies[i];
        else
            PSum[i] = PSum[i - 1];
    }

    const int K = 20;

    uint8_t digitLen[digitArSize];

    for (int iter = 0; iter < K; iter++)
    {
        bestFullBitsSz = UINT64_MAX;
        digits_found = false;

        for (int i = 0; i < digitArSize; i++)
        {
            digitLen[i] = 2;
            bestDigitsSz[i] = 2;
        }

        BCS_bmix(digitLen, 0, 0, 1, 0, 0);

        for (int i = 0; i < 20; i++)
        {
            int low = (i < (int)lower_bounds.size()) ?  lower_bounds[i] : 2;
            int high = (i < (int)upper_bounds.size()) ? upper_bounds[i] : 2;
        }

        if (!digits_found)
        {
            cout << "!!!!!!!!!!!!! BCMix code not found !!!!!!!!!" << endl;
            break;
        }

        cout << ".";

        refineBoundsAfterSearch(1);
    }

    cout << endl << "Optimal BCMix code: ";

    for (int i = 0; i < 32; i++)
        cout << (int)bestDigitsSz[i] << "|";

    cout << endl << "Optimal BCMix code size: "<<bestBlocksNum;


    cout << endl;

    delete[] PSum;
}


WordBasedText::~WordBasedText()
{
    //dtor
}


void WordBasedText::setBestBlocks(const std::vector<uint8_t>& digits)
{
    for (int i = 0; i < digitArSize; i++)
        bestDigitsSz[i] = 2;

    for (size_t i = 0; i < digits.size() && i < digitArSize; i++)
        bestDigitsSz[i] = digits[i];

    for (uint32_t i = 0; i < digitArSize; i++)
        cout<<(int)bestDigitsSz[i]<<" | ";
}

uint32_t WordBasedText::bitLength(uint64_t x)
{
    uint32_t len = 0;

    do
    {
        len++;
        x >>= 1;
    }
    while (x);

    return len;
}

uint64_t WordBasedText::eliasGammaLen(uint64_t x)
{
    uint32_t l = bitLength(x);
    return 2 * l - 1;
}

uint64_t WordBasedText::eliasDeltaLen(uint64_t x)
{
    uint32_t l = bitLength(x);
    return eliasGammaLen(l) + l - 1;
}

uint64_t WordBasedText::eliasOmegaLen(uint64_t x)
{
    uint64_t len = 1; // final terminating 0

    while (x > 1)
    {
        uint32_t l = bitLength(x);
        len += l;
        x = l - 1;
    }

    return len;
}

void WordBasedText::printEliasStats() const
{
    uint64_t gammaBits = 0, deltaBits = 0, omegaBits = 0;

    if (!numbers.empty())
    {
        for (int x : numbers)
        {
            uint64_t v = static_cast<uint32_t>(x) + 1;
            gammaBits += eliasGammaLen(v);
            deltaBits += eliasDeltaLen(v);
            omegaBits += eliasOmegaLen(v);
        }
    }
    else
    {
        for (uint32_t i = 0; i < Frequencies.size(); i++)
        {
            uint64_t v = i + 1;

            gammaBits += Frequencies[i] * eliasGammaLen(v);
            deltaBits += Frequencies[i] * eliasDeltaLen(v);
            omegaBits += Frequencies[i] * eliasOmegaLen(v);
        }
    }

    cout << endl;
    cout << "Elias gamma: " << (gammaBits + 7) / 8 << " bytes (" << (double)(gammaBits + 7) / (8*1024*1024) << " MB)" << endl;
    cout << "Elias delta: " << (deltaBits + 7) / 8 << " bytes (" << (double)(deltaBits + 7) / (8*1024*1024) << " MB)" << endl;
    cout << "Elias omega: " << (omegaBits + 7) / 8 << " bytes (" << (double)(omegaBits + 7) / (8*1024*1024) << " MB)" << endl;
}
