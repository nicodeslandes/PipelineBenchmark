// PipelineBenchmark.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

extern "C" int64_t calc(int64_t * arr, size_t size);
extern "C" int64_t calc_no_jump(int64_t * arr, size_t size);
extern "C" int64_t calc_no_jump_long_fetches(int64_t * arr, size_t size);
extern "C" int64_t calc_no_jump_not_so_long_fetches(int64_t * arr, size_t size);

int main()
{
	cout << "Allocating 1GB longs" << endl;
	const size_t MemSize = 1024*1024*1024;
	auto size = MemSize / sizeof(int64_t);
	auto arr = new int64_t[size];

	std::cout.imbue(std::locale(""));
	cout << "Populating the array with random values" << endl;
	srand((uint32_t)time(nullptr));
	for (int i = 0; i < size; i++) {
		arr[i] =
			((int64_t)rand()) << 0 |
			((int64_t)rand()) << 15 |
			((int64_t)rand()) << 30 |
			((int64_t)rand()) << 45 |
			((int64_t)rand()) << 60;
		//arr[i] = rand() + ((int64_t)rand()) << 45;
		if ((i + 1) % (size / 10) == 0) cout << "." << flush;
		//arr[i] = ((int64_t)i + 1) << (i * 4);
	}

	cout << endl << "Summing all positive elements" << endl;
	auto start = high_resolution_clock::now();
	int64_t sum = 0;
	for (int i = 0; i < size; i++) {
		if (arr[i] > 0)
			sum += arr[i];
	}
	auto elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start);
	cout << "Result: " << sum << " (in " << elapsed.count() << " ms)" << endl;

	start = high_resolution_clock::now();
	sum = calc(arr, size);
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

	start = high_resolution_clock::now();
	sum = calc_no_jump(arr, size);
	elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start);
	cout << "Result for calc_no_jump: " << sum << " (in " << elapsed.count() << " ms)" << endl;

	start = high_resolution_clock::now();
	sum = calc_no_jump_long_fetches(arr, size);
	elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start);
	cout << "Result for calc_no_jump_long_fetches: " << sum << " (in " << elapsed.count() << " ms)" << endl;

	start = high_resolution_clock::now();
	sum = calc_no_jump_long_fetches(arr, size);
	elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start);
	cout << "Result for calc_no_jump_long_fetches: " << sum << " (in " << elapsed.count() << " ms)" << endl;

	start = high_resolution_clock::now();
	sum = calc_no_jump_long_fetches(arr, size);
	elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start);
	cout << "Result for calc_no_jump_long_fetches: " << sum << " (in " << elapsed.count() << " ms)" << endl;

	start = high_resolution_clock::now();
	sum = calc_no_jump_not_so_long_fetches(arr, size);
	elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start);
	cout << "Result for calc_no_jump_not_so_long_fetches: " << sum << " (in " << elapsed.count() << " ms)" << endl;

	start = high_resolution_clock::now();
	sum = calc_no_jump_not_so_long_fetches(arr, size);
	elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start);
	cout << "Result for calc_no_jump_not_so_long_fetches: " << sum << " (in " << elapsed.count() << " ms)" << endl;

	start = high_resolution_clock::now();
	sum = calc_no_jump_not_so_long_fetches(arr, size);
	elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start);
	cout << "Result for calc_no_jump_not_so_long_fetches: " << sum << " (in " << elapsed.count() << " ms)" << endl;

	delete[] arr;
	return 0;
}