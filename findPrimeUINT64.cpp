#include <iostream>
#include <cmath>
#include <cstdint>
#include <chrono>
#include <vector>

#pragma warning(disable : 4996)
using namespace std;

const uint32_t size = 64;
const uint32_t mask = size - 1;
const uint32_t shiftValue = 6;
uint64_t bit, idx, startIndex;
vector<uint64_t> vCaches = {2, 3, 5}; 

uint64_t getK(uint64_t start, uint64_t i)
{
	uint64_t k = (uint64_t)((start) / i) * i;
	if(k < start) k += i;
	if(!(k % 2)) k += i;
	return k;
}

void findPrime(uint64_t* prime, uint64_t start, uint64_t n, vector<uint64_t>& vCaches)
{
    uint64_t i, j, k = 3;
    uint64_t last = static_cast<uint64_t>(sqrt(static_cast<double>(n)));
    int cnt = 0;
    
    auto end = vCaches.end();
    auto it = vCaches.begin() + 1;
    for(; it != end && *it <= last; ++it)
    {
    	i = k = *it;
    	if(i < start) k = getK(start, i);
        idx = (k >> shiftValue) - startIndex;
        bit = k & mask;
        bit = 1ULL << bit;
        //if (!(prime[idx] & bit) || k > i)
        {
        	j = i * i;
        	if(j < k) j = k;
            for (; j <= n; j += (i << 1))
            {
                idx = (j >> shiftValue) - startIndex;
                bit = j & mask;
                bit = 1ULL << bit;
                prime[idx] |= bit;
            }
        }
    }
}

void findPrime(vector<uint64_t>& vPrime, uint64_t n)
{
    uint64_t idx, bit, term = 32;
    uint64_t i, start = 7, last = term;
    uint64_t *prime;

    n = static_cast<uint64_t>(sqrt(static_cast<double>(n)));

    while(true)
    {
    	last >= n ? last = n : last;
	    startIndex = start >> shiftValue;
	    bit = (last >> shiftValue) - startIndex + 1ULL;
	    prime = (uint64_t*)calloc(bit, sizeof(uint64_t));
	    
		findPrime(prime, start, last, vPrime);//체로 걸러서
		for (i = start; i <= last; i += 2)//소수만 골라 내는 작업을 해줍니다.
		{
		    idx = (i >> shiftValue) - startIndex;;
		    bit = i & mask;
		    bit = 1ULL << bit;
		    if (!(prime[idx] & bit))
		    {
		    	vPrime.push_back(i);
		    }
		}

		free(prime);
		if(last == n)break;
		start = last + 1;
		term < 1048576 ? term *= term : term = 1048576;
		if(start < 1048576) last = term;
		else last += term;
	}
}

uint64_t getPrimeCounts(uint64_t* arrPrime, uint64_t start, uint64_t n, uint64_t& lastPrime)
{
    uint64_t cnt = 0;
    for (uint64_t i = start; i <= n; i += 2)//소수만 골라 내는 작업을 해줍니다.
    {
        idx = (i >> shiftValue) - startIndex;
        bit = i & mask;
        bit = 1ULL << bit;
        if (!(arrPrime[idx] & bit))
        {
            cnt++;
            //if(cnt <= 11) 
            //	printf("%llu ", i);
            //sum += i; //숫자가 커지면 오버플로우가 발생해서 제외시켰습니다.
            lastPrime = i;
        }
    }
    //printf("\n");
    return cnt;
}


int main()
{
    uint64_t* arrPrime;
    uint64_t i, start, end, cnt = 0, lastPrime = 2;
    uint64_t last, term = 1048576; //1048576; //16777216;

    printf("입력한 두 수 사이의 소수의 개수를 찾아줍니다.  숫자를 입력하세요(예 1000 2000) : ");
    scanf("%llu%llu", &start, &end);
    end <= 100000000000 ? term = 1048576 : term = 16777216;
    if(end < 2) return 0;
    else if(start < 3) 
	{
		cnt = 1;
		start = 3;
	} 
	if(!(start & 1)) start++;

	auto startTime = chrono::steady_clock::now();
    findPrime(vCaches, end);
    cout << start << ", " << end << ", " << vCaches.size() << endl;
    start >= term ? last = ((uint64_t)(start + term) / term) * term : last = term;
    auto t1 = chrono::steady_clock::now();
    while(true)
    {
    	last >= end ? last = end : last;
	    startIndex = start >> shiftValue;
	    bit = (last >> shiftValue) - startIndex + 1ULL;
	    arrPrime = (uint64_t*)calloc(bit, sizeof(uint64_t));
	    
		findPrime(arrPrime, start, last, vCaches);//체로 걸러서
		cnt += getPrimeCounts(arrPrime, start, last, lastPrime);
		
		auto t2 = chrono::steady_clock::now();
		chrono::duration<double> elapsed_seconds = t2 - t1;
		if(elapsed_seconds.count() >= 2.0)
		{
			auto endTime = chrono::steady_clock::now();
			chrono::duration<double> elapsed_seconds = endTime - startTime;
			cout << "count = " << cnt << "개, lastPrime = " << lastPrime << ", elapsed time = " << elapsed_seconds.count() <<"sec\n";
			t1 = chrono::steady_clock::now();
		}
		free(arrPrime);
		if(last == end) break;
		start = last + 1;
		last += term;
	} 
	auto endTime = chrono::steady_clock::now();
	chrono::duration<double> elapsed_seconds = endTime - startTime;
	cout << "count = " << cnt << "개, lastPrime = " << lastPrime << ", elapsed time = " << elapsed_seconds.count() <<"sec\n";

    system("pause");
}
