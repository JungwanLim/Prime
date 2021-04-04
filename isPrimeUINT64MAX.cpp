#include <cmath>
#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

const uint32_t size = 64;
const uint32_t mask = size - 1;
const uint32_t shiftValue = 6;
uint64_t bit, idx, startIndex;

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

void FillCache(vector<uint64_t>& mem, uint64_t n)
{
    static uint64_t term = 32;
    uint64_t start = mem.back() + 2;
    uint64_t i, last;
    uint64_t *prime;

    if(term == 32) 
    {
    	last = term;
	}
    else 
	{
		if(start + term > n) last = n;
		else if(start < term) last = term;
		else last = ((start + term) / term) * term; 
	}
	//if(start % 2 == 0) ++start;

    while(true)
    {
    	last >= n ? last = n : last;
	    startIndex = start >> shiftValue;
	    bit = (last >> shiftValue) - startIndex + 1ULL;
	    prime = (uint64_t*)calloc(bit, sizeof(uint64_t));
	    
		findPrime(prime, start, last, mem);//체로 걸러서
		for (i = start; i <= last; i += 2)//소수만 골라 내는 작업을 해줍니다.
		{
		    idx = (i >> shiftValue) - startIndex;;
		    bit = i & mask;
		    bit = 1ULL << bit;
		    if (!(prime[idx] & bit))
		    {
		    	mem.push_back(i);
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

// 중복된 코드들을 함수로 묶으면 더 좋겠지만 우선
// 편하게 보기 위해서 하나의 함수로 작성했습니다.
bool IsPrime(uint64_t n)
{
	// 캐시된 데이터(소수)들을 담아둘 벡터
	static vector<uint64_t> prime_numbers = {2, 3, 5};
	//static vector<uint64_t> cache_mems = {2, 3};
	static uint64_t current_last = 7;

	// 필요한 소수의 최댓값. 반복해서 계산할 필요가 없으므로
	// 딱 한 번만 계산해서 그 결과를 담아두고 활용합니다.
	uint64_t last = static_cast<uint64_t>(sqrt(static_cast<double>(n)));
	//cout << prime_numbers.size() << endl;

	// 만약 소수 판별을 하려는 n이, 캐시된 소수들 중 가장 큰 수보다 작거나 같다면,
	// 벡터 내에서의 이분 탐색으로 빠르게 결과를 도출합니다.
	if (n <= *prime_numbers.rbegin()) {
		return binary_search(prime_numbers.begin(), prime_numbers.end(), n);
	}

	// 만일 캐시된 소수가 부족하다면 그 이후의 소수를 계산해서,
	// 캐시 역할을 하는 벡터에 추가합니다.
//	cout << *prime_numbers.rbegin() << ", " << last << endl;
	if(current_last < last)
	{
		//FillCache(cache_mems, last);
		cout << "cache data 생성중입니다.....\n";
		FillCache(prime_numbers, last);
		current_last = last;
		cout << prime_numbers.size() << "개의 Data가 cache 되었습니다.\n\n";
	}
/*
	for (uint64_t i = *prime_numbers.rbegin() + 1; i <= last; ++i) {
		bool is_prime = true;
		auto end = prime_numbers.end();
		uint32_t inner_last = static_cast<uint64_t>(sqrt(static_cast<double>(i)));
		
		// 캐시에 소수를 추가할 때에도 이미 캐시된 소수들을 활용하면 더 빠르게 계산이 가능합니다.
		for (auto it = prime_numbers.begin(); is_prime && it != end && *it <= inner_last; ++it) {
			if (!(i % *it)) 
			{
				is_prime = false;
				break;
			}
		}
		if (is_prime) prime_numbers.push_back(i);
	}

	cout << cache_mems.size() << ", " << prime_numbers.size() << endl;
	for(int i = 0; i < prime_numbers.size(); ++i)
	{
		if(cache_mems[i] != prime_numbers[i])
		{
			cout << "두 수에 차이가 있습니다." << cache_mems[i] << ", " << prime_numbers[i] << endl; 
		}
	}
*/
	// 위 과정에서 n^0.5 이하의 소수를 모두 구해 벡터 안에 넣어두었으니,
	// 이제 벡터안에 캐시된 소수들을 사용해서 n이 소수인지 판별하면 됩니다.
	//cout << "cache size = " << prime_numbers.size() << endl;
	auto end = prime_numbers.end();
	for (auto it = prime_numbers.begin(); it != end && *it <= last; ++it) {
		if (!(n % *it)) return false;
	}
	return n >= 2;
}

int main()
{
	uint64_t n = 1, cnt = 1;
	
	while(n)
	{
		const char *str[] = {"소수가 아닙니다.\n", "소수입니다.\n"};
	    printf("입력한 숫자가 소수인지 판별합니다. 숫자를 입력하세요 : ");
	    scanf("%llu", &n);
	
		auto tStart = chrono::steady_clock::now();
	    bool isPrime = IsPrime(n);//체로 걸러서
		auto tEnd = chrono::steady_clock::now();
	    chrono::duration<double> elapsed_seconds = tEnd-tStart;
	    cout << "숫자 " << n << " 은(는) ";
		cout << str[isPrime] << "elapsed time = " << elapsed_seconds.count() <<"sec\n\n";
	}
    
    system("pause");
}

