//PALIN - The Next Palindrome
/*
If input number has an even number of digits, split into two parts:
	123456 -> 123 456
If input number has an odd number of digits, split into three parts:
	1234567 -> 123 1234 567
Flip first part, if it is lexicographically bigger than second part then we have next bigger palindrom.
	eg. 123111 -> 123 111 -> 321 is bigger than 111 -> 123321
If less than second part, increment lexicograhically first part by one and repeat
	eg. 123456 -> 123 456 -> 321 is lower than 456 -> 123 + 1 = 124 -> 421
*/
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std::chrono;

std::string flip(const std::string& s)
{
	std::string temp(s.rbegin(), s.rend());
	return temp;
}

bool is_palindrom(const std::string& s)
{
	std::string::const_iterator begin = s.begin();
	std::string::const_iterator end = s.end() - 1;
	while (begin < end) {
		if (*begin++ != *end--) {
			return false;
		}
	}
	return true;
}

std::string increment(const std::string& s)
{
	std::string output;
	output.reserve(s.size());
	std::string::const_reverse_iterator r = s.rbegin();
	bool carry = false;

	if (*r < '9') {
		output += *r + 1;
		r++;
	}
	else {
		output += '0';
		r++;
		carry = true;
	}
	for (r; r != s.rend(); r++) {
		if (carry) {
			if (*r < '9') {
				output += *r + 1;
				carry = false;
			}
			else {
				output += '0';
			}
		}
		else {
			output += *r++;
			return std::string(s.begin(), r.base()) + flip(output);
		}
	}
	if (carry) {
		output += '1';
	}
	return flip(output);
}

struct Splits {
	std::string fst_inc;
	std::string fst_flip;
	std::string sec;
};

Splits split(const std::string& s)
{
	size_t len = s.length();
	Splits splits;

	if (len % 2) {
		splits.fst_inc = s.substr(0, len / 2 + 1);
		splits.fst_flip = s.substr(0, len / 2);
		splits.sec = s.substr(len / 2 + 1, len / 2);
	}
	else {
		splits.fst_inc = s.substr(0, len / 2);
		splits.fst_flip = s.substr(0, len / 2);
		splits.sec = s.substr(len / 2, len / 2);
	}
	return splits;
}

void min_palindrom(std::string &s)
{
	Splits splits = split(s);
	std::string half = flip(splits.fst_flip);

	if (half > splits.sec) {
		s.clear();
		s += splits.fst_inc;
		s += half;
	}
	else {
		splits = split(increment(splits.fst_inc) + half);
		s.clear();
		s += splits.fst_inc;
		s += flip(splits.fst_flip);
	}
}

int main()
{
	std::vector<std::string> input;
	std::string s;
	int nul;

	auto start = high_resolution_clock::now();
	std::cin >> nul;
	while (std::cin >> s) {
		input.push_back(s);
	}
	auto end = high_resolution_clock::now();
	//std::cout << "reading time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

	start = high_resolution_clock::now();
	std::for_each(input.begin(), input.end(), min_palindrom);
	end = high_resolution_clock::now();
	//std::cout << "palindrom time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

	for (auto s : input) {
		std::cout << s << "\n";
	}

	return 0;
}