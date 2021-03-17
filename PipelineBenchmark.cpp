// PipelineBenchmark.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <random>
#include <limits>

using namespace std;
using namespace std::chrono;

extern "C" int64_t calc(int64_t * arr, size_t size);
extern "C" int64_t calc_no_jump(int64_t * arr, size_t size);
extern "C" int64_t calc_no_jump_long_fetches(int64_t * arr, size_t size);
extern "C" int64_t calc_no_jump_longer_fetches(int64_t * arr, size_t size);
extern "C" int64_t calc_no_jump_not_so_long_fetches(int64_t * arr, size_t size);
extern "C" int64_t calc_no_jump_custom_fetches(int64_t * arr, size_t size, size_t skip);

#define SHORT_OUTPUT

int main()
{
	cout << "Allocating 1GB longs" << endl;
	const size_t MemSize = 1LL*1024LL*1024LL*1024LL;
	auto size = MemSize / sizeof(int64_t);
	auto arr = new int64_t[size];

#ifndef SHORT_OUTPUT
	std::cout.imbue(std::locale(""));
#endif
	cout << "Populating the array with random values" << endl;
	std::random_device rd;     //Get a random seed from the OS entropy device, or whatever
	std::mt19937_64 eng(rd()); //Use the 64-bit Mersenne Twister 19937 generator
							   //and seed it with entropy.
	//std::ranlux48_base // faster?

	//Define the distribution, by default it goes from 0 to MAX(unsigned long long)
	//or what have you.
	std::uniform_int_distribution<int64_t> distr;
	//std::subtract_with_carry<unsigned long long> distr;

	auto start = high_resolution_clock::now();
	for (int i = 0; i < size; i++) {
		arr[i] = distr(eng) * ((distr(eng) % 2) ? -1 : 1);
			//((int64_t)rand()) << 0 |
			//((int64_t)rand()) << 15 |
			//((int64_t)rand()) << 30 |
			//((int64_t)rand()) << 45 |
			//((int64_t)rand()) << 60;
		//arr[i] = rand() + ((int64_t)rand()) << 45;
		if ((i + 1) % (size / 10) == 0) cout << "." << flush;
		//arr[i] = ((int64_t)i + 1) << (i * 4);
	}
	auto elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start);
	cout << endl << "Done in " << elapsed.count() << " ms" << endl;

	cout << "Summing all positive elements" << endl;
	for (int i = 0; i < 10; i++)
	{
		start = high_resolution_clock::now();
		int64_t sum = 0;
		for (int i = 0; i < size; i++) {
			if (arr[i] > 0)
				sum += arr[i];
		}

		elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start);
		cout << "Result: " << sum << " (in " << elapsed.count() << " ms)" << endl;
	}

	start = high_resolution_clock::now();
	auto sum = calc(arr, size);
	elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start);
	cout << "Result for calc: " << sum << " (in " << elapsed.count() << " ms)" << endl;

	start = high_resolution_clock::now();
	sum = calc(arr, size);
	elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start);
	cout << "Result for calc: " << sum << " (in " << elapsed.count() << " ms)" << endl;

	start = high_resolution_clock::now();
	sum = calc_no_jump(arr, size);
	elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start);
	cout << "Result for calc_no_jump: " << sum << " (in " << elapsed.count() << " ms)" << endl;

	start = high_resolution_clock::now();
	sum = calc_no_jump(arr, size);
	elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start);
	cout << "Result for calc_no_jump: " << sum << " (in " << elapsed.count() << " ms)" << endl;

	//start = high_resolution_clock::now();
	//sum = calc_no_jump(arr, size);
	//elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start);
	//cout << "Result for calc_no_jump: " << sum << " (in " << elapsed.count() << " ms)" << endl;

	//start = high_resolution_clock::now();
	//sum = calc_no_jump_long_fetches(arr, size);
	//elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start);
	//cout << "Result for calc_no_jump_long_fetches: " << sum << " (in " << elapsed.count() << " ms)" << endl;

	start = high_resolution_clock::now();
	sum = calc_no_jump_long_fetches(arr, size);
	elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start);
	cout << "Result for calc_no_jump_long_fetches: " << sum << " (in " << elapsed.count() << " ms)" << endl;

	//start = high_resolution_clock::now();
	//sum = calc_no_jump_longer_fetches(arr, size);
	//elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start);
	//cout << "Result for calc_no_jump_longer_fetches: " << sum << " (in " << elapsed.count() << " ms)" << endl;


	//start = high_resolution_clock::now();
	//sum = calc_no_jump_not_so_long_fetches(arr, size);
	//elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start);
	//cout << "Result for calc_no_jump_not_so_long_fetches: " << sum << " (in " << elapsed.count() << " ms)" << endl;

	//start = high_resolution_clock::now();
	//sum = calc_no_jump_not_so_long_fetches(arr, size);
	//elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start);
	//cout << "Result for calc_no_jump_not_so_long_fetches: " << sum << " (in " << elapsed.count() << " ms)" << endl;

	auto run_custom_skip = [&](auto skip) {
		start = high_resolution_clock::now();
		sum = calc_no_jump_custom_fetches(arr, size, skip);
		elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start);
#ifdef SHORT_OUTPUT
		cout << skip << " " << elapsed.count() << " " << sum << endl;
#else
		cout << "Result for calc_no_jump_custom_fetches(" << skip << "): " << sum << " (in " << elapsed.count() << " ms)" << endl;
#endif
		
	};

	for (int skip = 1; skip <= 65536; skip <<= 1) {
		run_custom_skip(skip);
		run_custom_skip(skip);
		run_custom_skip(skip);
	}

	delete[] arr;
	return 0;
}