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
	    
		findPrime(prime, start, last, mem);//ü�� �ɷ���
		for (i = start; i <= last; i += 2)//�Ҽ��� ��� ���� �۾��� ���ݴϴ�.
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

// �ߺ��� �ڵ���� �Լ��� ������ �� �������� �켱
// ���ϰ� ���� ���ؼ� �ϳ��� �Լ��� �ۼ��߽��ϴ�.
bool IsPrime(uint64_t n)
{
	// ĳ�õ� ������(�Ҽ�)���� ��Ƶ� ����
	static vector<uint64_t> prime_numbers = {2, 3, 5};
	//static vector<uint64_t> cache_mems = {2, 3};
	static uint64_t current_last = 7;

	// �ʿ��� �Ҽ��� �ִ�. �ݺ��ؼ� ����� �ʿ䰡 �����Ƿ�
	// �� �� ���� ����ؼ� �� ����� ��Ƶΰ� Ȱ���մϴ�.
	uint64_t last = static_cast<uint64_t>(sqrt(static_cast<double>(n)));
	//cout << prime_numbers.size() << endl;

	// ���� �Ҽ� �Ǻ��� �Ϸ��� n��, ĳ�õ� �Ҽ��� �� ���� ū ������ �۰ų� ���ٸ�,
	// ���� �������� �̺� Ž������ ������ ����� �����մϴ�.
	if (n <= *prime_numbers.rbegin()) {
		return binary_search(prime_numbers.begin(), prime_numbers.end(), n);
	}

	// ���� ĳ�õ� �Ҽ��� �����ϴٸ� �� ������ �Ҽ��� ����ؼ�,
	// ĳ�� ������ �ϴ� ���Ϳ� �߰��մϴ�.
//	cout << *prime_numbers.rbegin() << ", " << last << endl;
	if(current_last < last)
	{
		//FillCache(cache_mems, last);
		cout << "cache data �������Դϴ�.....\n";
		FillCache(prime_numbers, last);
		current_last = last;
		cout << prime_numbers.size() << "���� Data�� cache �Ǿ����ϴ�.\n\n";
	}
/*
	for (uint64_t i = *prime_numbers.rbegin() + 1; i <= last; ++i) {
		bool is_prime = true;
		auto end = prime_numbers.end();
		uint32_t inner_last = static_cast<uint64_t>(sqrt(static_cast<double>(i)));
		
		// ĳ�ÿ� �Ҽ��� �߰��� ������ �̹� ĳ�õ� �Ҽ����� Ȱ���ϸ� �� ������ ����� �����մϴ�.
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
			cout << "�� ���� ���̰� �ֽ��ϴ�." << cache_mems[i] << ", " << prime_numbers[i] << endl; 
		}
	}
*/
	// �� �������� n^0.5 ������ �Ҽ��� ��� ���� ���� �ȿ� �־�ξ�����,
	// ���� ���;ȿ� ĳ�õ� �Ҽ����� ����ؼ� n�� �Ҽ����� �Ǻ��ϸ� �˴ϴ�.
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
		const char *str[] = {"�Ҽ��� �ƴմϴ�.\n", "�Ҽ��Դϴ�.\n"};
	    printf("�Է��� ���ڰ� �Ҽ����� �Ǻ��մϴ�. ���ڸ� �Է��ϼ��� : ");
	    scanf("%llu", &n);
	
		auto tStart = chrono::steady_clock::now();
	    bool isPrime = IsPrime(n);//ü�� �ɷ���
		auto tEnd = chrono::steady_clock::now();
	    chrono::duration<double> elapsed_seconds = tEnd-tStart;
	    cout << "���� " << n << " ��(��) ";
		cout << str[isPrime] << "elapsed time = " << elapsed_seconds.count() <<"sec\n\n";
	}
    
    system("pause");
}

