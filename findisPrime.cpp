#include <iostream>
#include <cmath>
#include <cstdint>
#include <chrono>
#include <algorithm>
#include <vector>

#pragma warning(disable : 4996)
using namespace std;

class CPrime
{
private:
	const uint32_t bit_size = 64;
	const uint32_t mask = bit_size - 1;
	const uint32_t shift_value = 6;
	
	uint64_t bit;
	uint64_t idx;
	uint64_t term = 1048576;
	uint64_t current_last = 7;
	uint64_t start_index;
	uint64_t last_prime = 2;
	
	vector<uint64_t> cached_data = {2, 3, 5};
	
public :
	CPrime();
	~CPrime(){	}
	
	uint64_t getK(uint64_t start, uint64_t i);
	bool getK(uint64_t start, uint64_t i, uint64_t n, uint64_t& k);
	bool is_prime(uint64_t n);
	void get_cached_data(uint64_t n);
	void find_prime(uint64_t* arr_prime, uint64_t start, uint64_t end);
	void find_prime_max(uint64_t* arr_prime, uint64_t start, uint64_t end);
	void run_find_prime_max(uint64_t __start, uint64_t start, uint64_t last, uint64_t end);
	void run_find_prime();
	void run_is_prime();
	
	//uint64_t get_prime_numbers_max(uint64_t* arrPrime, uint64_t start, uint64_t n);
	inline uint64_t get_prime_numbers(uint64_t* arr_prime, uint64_t start, uint64_t n)
	{
	    uint64_t cnt = 0;
	    for (uint64_t i = start; i <= n;)//소수만 골라 내는 작업을 해줍니다.
	    {
	        idx = (i >> shift_value) - start_index;
	        bit = i & mask;
	        bit = 1ULL << bit;
	        if (!(arr_prime[idx] & bit))
	        {
	            cnt++;
	            //if(cnt <= 11) 
	            //	printf("%llu\n", i);
	            //sum += i; // 이곳에서 출력을 해보거나 합을 구할 수 있슴 
	            last_prime = i;
	        }
	        if(i == n || i > n - 2) break;
	        else  i += 2;
	    }
	    //printf("\n");
	    return cnt;
	}
};

CPrime::CPrime()
{
	get_cached_data(1000000000000);
}

bool CPrime::getK(uint64_t start, uint64_t i, uint64_t n, uint64_t& k)
{
	k = (uint64_t)((start) / i) * i;
	if(k == start) return true;
	if(k > n - i) return false;
	else if(k < start) k += i;
	if(k % 2 == 0) 
	{
		if(k > n - i) return false;
		else k += i;
	}
	return true;
}

uint64_t CPrime::getK(uint64_t start, uint64_t i)
{
	uint64_t k = (uint64_t)((start) / i) * i;
	if(k < start) k += i;
	if(!(k % 2)) k += i;
	return k;
}

void CPrime::find_prime(uint64_t* arr_prime, uint64_t start, uint64_t n)
{
    uint64_t i, j, k = 3;
    uint64_t last = static_cast<uint64_t>(sqrt(static_cast<double>(n)));
    
    auto end = cached_data.end();
    auto it = cached_data.begin() + 1;
    for(; it != end && *it <= last; ++it)
    {
    	i = k = *it;
    	if(i < start) k = getK(start, i);
        idx = (k >> shift_value) - start_index;
        bit = k & mask;
        bit = 1ULL << bit;
        //if (!(prime[idx] & bit) || k > i)
        {
        	j = i * i;
        	if(j < k) j = k;
            for (; j <= n; j += (i << 1))
            {
                idx = (j >> shift_value) - start_index;
                bit = j & mask;
                bit = 1ULL << bit;
                arr_prime[idx] |= bit;
            }
        }
    }
}

void CPrime::find_prime_max(uint64_t* arr_prime, uint64_t start, uint64_t n)
{
    uint64_t i, j, k = 3;
    uint64_t last = static_cast<uint64_t>(sqrt(static_cast<double>(n)));
    
    auto end = cached_data.end();
    auto it = cached_data.begin() + 1;
    for(; it != end && *it <= last; ++it)
    {
    	i = k = *it;
    	if(i < start && !getK(start, i, n, k)) continue;
        idx = (k >> shift_value) - start_index;
        bit = k & mask;
        bit = 1ULL << bit;
        //if (!(prime[idx] & bit) || k > i)
        {
        	if(start <= i) k = i * i;
            for (j = k; j <= n; )
            {
                idx = (j >> shift_value) - start_index;
                bit = j & mask;
                bit = 1ULL << bit;
                arr_prime[idx] |= bit;
                
                if(j == n || j > n - i * 2) break;
                else j += (i * 2);
            }
        }
    }
}

void CPrime::get_cached_data(uint64_t n)
{
    n = static_cast<uint64_t>(sqrt(static_cast<double>(n)));
    if(n <= current_last) return;
	cout << "cache data 생성중입니다.....\n";

    static uint64_t term = 32;
    uint64_t start = cached_data.back() + 2;
    uint64_t i, last;
    uint64_t *arr_prime;

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
	    start_index = start >> shift_value;
	    bit = (last >> shift_value) - start_index + 1ULL;
	    arr_prime = (uint64_t*)calloc(bit, sizeof(uint64_t));
	    
		find_prime(arr_prime, start, last);//체로 걸러서
		for (i = start; i <= last; i += 2)//소수만 골라 내는 작업을 해줍니다.
		{
		    idx = (i >> shift_value) - start_index;;
		    bit = i & mask;
		    bit = 1ULL << bit;
		    if (!(arr_prime[idx] & bit))
		    {
		    	cached_data.push_back(i);
		    }
		}

		free(arr_prime);
		if(last == n)
		{
			cout << cached_data.size() << "개의 Data가 cache 되었습니다.\n\n";
			break;
		}
		start = last + 1;
		term < 1048576 ? term *= term : term = 1048576;
		if(start < 1048576) last = term;
		else last += term;
	}
    current_last = n;
}

bool CPrime::is_prime(uint64_t n)
{
	if (n <= *cached_data.rbegin()) {
		return binary_search(cached_data.begin(), cached_data.end(), n);
	}

	uint64_t last = static_cast<uint64_t>(sqrt(static_cast<double>(n)));
	get_cached_data(n);

	auto end = cached_data.end();
	for (auto it = cached_data.begin(); it != end && *it <= last; ++it) {
		if (!(n % *it)) return false;
	}
	return n >= 2;
}

void CPrime::run_find_prime_max(uint64_t __start, uint64_t start, uint64_t last, uint64_t end)
{
    uint64_t* arr_prime;
    uint64_t i, cnt = 0;
    uint64_t term = 16777216;
    if(__start < 3) cnt = 1;
    
	//while(true)
	{
		auto startTime = chrono::steady_clock::now();
	    auto t1 = chrono::steady_clock::now();
	    while(true)
	    {
	    	//last >= end ? last = end : last;
		    start_index = start >> shift_value;
		    bit = (last >> shift_value) - start_index + 1ULL;
		    arr_prime = (uint64_t*)calloc(bit, sizeof(uint64_t));
		    
		    find_prime_max(arr_prime, start, last);
			cnt += get_prime_numbers(arr_prime, start, last);
			
			auto t2 = chrono::steady_clock::now();
			chrono::duration<double> elapsed_seconds = t2 - t1;
			if(elapsed_seconds.count() >= 2.0)
			{
				auto endTime = chrono::steady_clock::now();
				chrono::duration<double> elapsed_seconds = endTime - startTime;
				cout << "count = " << cnt << "개, last_prime = " << last_prime << ", elapsed time = " << elapsed_seconds.count() <<"sec\n";
				t1 = chrono::steady_clock::now();
			}
			free(arr_prime);
			if(last == end) break;
			start = last + 1;
			if(last >= end - term) last = end;
			else last += term;
		} 
		auto endTime = chrono::steady_clock::now();
		chrono::duration<double> elapsed_seconds = endTime - startTime;
		cout << endl;
		cout << __start << "부터 " << end << "까지 소수의 개수는 " ;
		cout << cnt << "개, last_prime = " << last_prime << endl;
		cout << "elapsed time = " << elapsed_seconds.count() <<"sec\n\n";
	}
}

void CPrime::run_find_prime()
{
    uint64_t* arr_prime;
    uint64_t i, __start, start, end, cnt = 0, lastPrime = 2;
    uint64_t last, term = 1048576; //1048576; //16777216;
    
	cout << "(예 1 1000) - 1부터 1000사이의 소수를 찾아줍니다.\n";
	cout << "(예 100000000000 500000) 100000000000부터 100000500000 사이의 소수를 찾아줍니다.\n";
	cout << "0 0을 입력시 메뉴로 복귀합니다.\n\n";
	while(true)
	{
	    cout << "숫자를 입력하세요 : ";
	    cin >> start >> end;
	    if(start == 0 && end == 0) break;
	    else if(end < 2) continue;
	    else if(start > end) end += start;
	    __start = start;
	    
		auto startTime = chrono::steady_clock::now();
	    if(start < 3) 
		{
			cnt = 1;
			start = 3;
		}
		else cnt = 0; 
		if(!(start & 1)) start++;
	    end <= 100000000000 ? term = 1048576 : term = 16777216;
	    
	    get_cached_data(end);
	    //cout << __start << ", " << end << ", " << cached_data.size() << endl;
	    if(end - start < term) {
	    	last = end;
		}
	    else{
		    start >= term ? last = ((uint64_t)(start + term) / term) * term : last = term;
		}
		if(end > UINT64_MAX - 10000000000)  {
		    run_find_prime_max(__start, start, last, end);
		    continue;
		} 
	    auto t1 = chrono::steady_clock::now();
	    while(true)
	    {
	    	last >= end ? last = end : last;
		    start_index = start >> shift_value;
		    bit = (last >> shift_value) - start_index + 1ULL;
		    arr_prime = (uint64_t*)calloc(bit, sizeof(uint64_t));
		    
			find_prime(arr_prime, start, last);
			cnt += get_prime_numbers(arr_prime, start, last);
			
			auto t2 = chrono::steady_clock::now();
			chrono::duration<double> elapsed_seconds = t2 - t1;
			if(elapsed_seconds.count() >= 2.0)
			{
				auto endTime = chrono::steady_clock::now();
				chrono::duration<double> elapsed_seconds = endTime - startTime;
				cout << "count = " << cnt << "개, last_prime = " << last_prime << ", elapsed time = " << elapsed_seconds.count() <<"sec\n";
				t1 = chrono::steady_clock::now();
			}
			free(arr_prime);
			if(last == end) break;
			start = last + 1;
			last += term;
		} 
		auto endTime = chrono::steady_clock::now();
		chrono::duration<double> elapsed_seconds = endTime - startTime;
		cout << endl;
		cout << __start << "부터 " << end << "까지 소수의 개수는 " ;
		cout << cnt << "개, last_prime = " << last_prime << endl;
		cout << "elapsed time = " << elapsed_seconds.count() <<"sec\n\n";
	}
}

void CPrime::run_is_prime()
{
	uint64_t n = 1;
	
	while(n)
	{
		const char *str[] = {"소수가 아닙니다.\n", "소수입니다.\n"};
		cout << "입력한 숫자가 소수인지 판별합니다. 숫자를 입력하세요 : ";
		cin >> n;
	
		auto tStart = chrono::steady_clock::now();
	    bool isPrime = is_prime(n);//체로 걸러서
		auto tEnd = chrono::steady_clock::now();
	    chrono::duration<double> elapsed_seconds = tEnd-tStart;
	    cout << "숫자 " << n << " 은(는) ";
		cout << str[isPrime] << "elapsed time = " << elapsed_seconds.count() <<"sec\n\n";
	}
}

int main()
{
	int index = 1;
	CPrime prime;
	//system("cls");
	
	while(index != 3)
	{
		cout << "1. 입력한 두 수 사이의 소수의 개수를 찾아줍니다.\n\n";
		cout << "2. 입력한 수가 소수인지를 판별합니다.\n\n";
		cout << "3. 프로그램을 종료합니다.\n\n";
		
		cin >> index;
		switch(index)
		{
		case 1 : prime.run_find_prime();
			break;
		
		case 2 : prime.run_is_prime();
			break;
		
		case 3 : return 0;
		
		default : break;
		}
	}
	return 0;
}

