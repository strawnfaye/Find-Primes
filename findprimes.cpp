// S. Faye Strawn
// COP4520 - Spring 2018
// Programming Assignment #1: Finding Primes

#include <iostream>
#include <fstream>
#include <cmath>
#include <climits>
#include <thread>
#include <vector>
#include <algorithm>
#include <chrono>
#include <time.h>
#include <ctime>
#include <cstdio>

#ifdef __linux__ 
#define _BSD_SOURCE
#include <sys/time.h>
#endif


#define 		MAX_PRIMES	10
#define			N_MAX		100000000
#define 		N_WORKERS	8


struct Range
{
	int a;
	int b;
};

int isPrime(int N)
{
	if (N == 2)
		return true;

	// Check if it divides evenly
	if (N == 1 || N % 2 == 0)
		return false;

	int max = sqrt(N) + 1;
	for (int i = 3; i < max; i += 2)
	{
		// Check if it divides evenly
		if (N % i == 0)
			return false;
	}
	return true;
}

class ThreadCalc
{
	std::thread* _worker;
	Range _low;
	Range _high;
	
	long long _count;
	long long _sum;
	int _maxPrimes[MAX_PRIMES];
	
	void itsPrime(int N)
	{
		int smol = 0;
		_count++;
		_sum += N;
		
		for (int i = 1; i < MAX_PRIMES; ++i)
		{
			if (_maxPrimes[smol] > _maxPrimes[i])
				smol = i;
		}
			
		if (N > _maxPrimes[smol])
			_maxPrimes[smol] = N;
	}
	
	void run()
	{
		_count = 0;
		_sum = 0;
		for (int i = 0; i < MAX_PRIMES; ++i)
			_maxPrimes[i] = 0;

		for(int i = _low.a; i <= _low.b; ++i)
		{
			if(isPrime(i))
				itsPrime(i);
		}
		
		for(int i = _high.a; i <= _high.b; ++i)
		{
			if(isPrime(i))
				itsPrime(i);
		}
	}
	
public:
	ThreadCalc(Range lRange, Range hRange) : _low(lRange), _high(hRange)
	{
		_worker = new std::thread(&ThreadCalc::run, this);

	}
	~ThreadCalc()
	{
		delete _worker;
	}
	void join()
	{
		_worker->join();
	}
	
	// These functions only to be used after join()
	long long getCount()
	{
		return _count;
	}
	long long getSum()
	{
		return _sum;
	}
	void mergeMaxes(int* oMax)
	{
		for (int i = 0; i < MAX_PRIMES; ++i)
		{
			int smol = 0;
		
			for (int j = 1; j < MAX_PRIMES; ++j)
			{
				if (oMax[smol] > oMax[j])
					smol = j;
			}
				
			if (_maxPrimes[i] > oMax[smol])
				oMax[smol] = _maxPrimes[smol];
		}
	}
};

class Timer
{
	std::clock_t t1, t0;

public:
	void Start()
	{
		t0 = std::clock();
	}
	void Stop()
	{
		t1 = std::clock();
	}
	int TimePassed()
	{
		return (t1 - t0) / CLOCKS_PER_SEC;
	}
};

int main()
{
	Range ranges[N_WORKERS * 2];
	std::vector<ThreadCalc*> threads;
	Timer time;
	int dTime;
	long long count = 0;
	long long sum = 0;
	std::vector<int> maxes(MAX_PRIMES, 0);
	
	time.Start();
	
	for (int i = 0; i < N_WORKERS * 2; ++i)
	{
		ranges[i].a = 1 + i * (N_MAX / (N_WORKERS * 2));
		ranges[i].b = (i + 1) * (N_MAX / (N_WORKERS * 2));
	}
	
	for (int i = 0; i < N_WORKERS; ++i)
		threads.push_back(new ThreadCalc(ranges[i], ranges[(N_WORKERS * 2 - 1) - i]));
	
	for (int i = 0; i < N_WORKERS; ++i)
		threads[i]->join();
	
	for (int i = 0; i < N_WORKERS; ++i)
	{
		count += threads[i]->getCount();
		sum += threads[i]->getSum();
		
		threads[i]->mergeMaxes(&(maxes[0]));
	}
	
	std::sort(maxes.begin(), maxes.end());
	
	time.Stop();
	dTime = time.TimePassed();
	
	std::ofstream file("primes.txt");

	file << "Runtime: " << dTime << " seconds" << std::endl;
	file << "Sum of primes: " << sum << std::endl;
	file << "Count of primes: " << count << std::endl;
	file << "The 10 max primes are:" << std::endl;

	for (int i = 0; i < MAX_PRIMES; ++i)
		file  << maxes[i] << std::endl;

	file.close();

	return 0;
}
