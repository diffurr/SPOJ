//ARITH - Simple Arithmetics
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#if defined(_M_X64) || defined(__amd64__)
typedef long long int digit;
#else
typedef int digit;
#endif

typedef std::vector<digit> Number;

struct BigInt {
#if defined(_M_X64) || defined(__amd64__)
	static const int dim = 9;
	static const int base = 1000000000;
#else
	static const int dim = 4;
	static const int base = 10000;
#endif
	Number number;

	BigInt(std::string);
	BigInt(const char* s) : BigInt(std::string(s)) {}
	BigInt(const BigInt& bigint) : number(bigint.number) {}
	BigInt(int);
	
	std::string str();
	void shiftl(int);
	
	BigInt& operator+=(const BigInt&);
	BigInt& operator-=(const BigInt&);
	BigInt& operator*=(const BigInt&);
	friend BigInt operator+(BigInt, const BigInt&);
	friend BigInt operator-(BigInt, const BigInt&);
	friend BigInt operator*(BigInt, const BigInt&);
};

BigInt::BigInt(int n)
{
	while (n / base) {
		number.push_back(n % base);
		n /= base;
	}
	number.push_back(n);
}

BigInt::BigInt(std::string s)
{
	std::reverse(s.begin(), s.end());
	while (s.size() % dim) s += '0';
	std::reverse(s.begin(), s.end());

	for (auto it = s.begin(); it != s.end(); it += dim) {
		digit num = std::stoul(std::string(it, it + dim));
		number.push_back(num);
	}
	//storing number as little-endian
	std::reverse(number.begin(), number.end());
}

void BigInt::shiftl(int shift)
{
	std::reverse(number.begin(), number.end());
	while (shift--) number.push_back(0);
	std::reverse(number.begin(), number.end());
}

BigInt& BigInt::operator*=(const BigInt& obj)
{
	std::vector<BigInt> products;
	int shift = 0;

	const Number& b = obj.number;
	for (auto itb = b.begin(); itb != b.end(); itb++) {
		BigInt product = *this;
		Number& a = product.number;
		int carry = 0;
		for (auto ita = a.begin(); ita != a.end(); ita++) {
			digit mul = *ita * *itb + carry;
			if (mul >= base) {
				*ita = mul % base;
				carry = (mul - mul % base) / base;
			}
			else {
				*ita = mul;
				carry = 0;
			}
		}

		if (carry) {
			a.push_back(carry);
		}

		product.shiftl(shift++);
		products.push_back(product);
	}

	BigInt sum("0");
	for (auto n : products) {
		sum += n;
	}

	number = sum.number;
	while (number.back() == 0 && number.size() > 1) number.pop_back();
	return *this;
}


BigInt& BigInt::operator+=(const BigInt& obj)
{
	Number& a = number;
	const Number& b = obj.number;
	int carry = 0;
	digit sum = 0;
	
	while (a.size() < b.size()) a.push_back(0);

	auto ita = a.begin();
	for (auto itb = b.begin(); itb != b.end(); ita++, itb++) {
		sum = *ita + *itb + carry;
		if (sum >= base) {
			carry = 1;
			*ita = sum - base;
		}
		else {
			carry = 0;
			*ita = sum;
		}
	}

	if (carry) {
		for (ita; ita != a.end(); ita++) {
			sum = *ita + carry;
			if (sum >= base) {
				carry = 1;
				*ita = sum - base;
			}
			else {
				carry = 0;
				*ita = sum;
				break;
			}
		}
	}

	if (carry) a.push_back(carry);

	return *this;
}

//don't support negative numbers
//subtracting bigger from smaller is undefined
BigInt& BigInt::operator-=(const BigInt& obj)
{
	Number& a = number;
	const Number& b = obj.number;
	int borrow = 0;
	digit diff = 0;

	while (a.size() < b.size()) a.push_back(0);

	auto ita = a.begin();
	for (auto itb = b.begin(); itb != b.end(); ita++, itb++) {
		diff = *ita - *itb - borrow;
		if (diff < 0) {
			borrow = 1;
			*ita = base + diff;
		}
		else {
			borrow = 0;
			*ita = diff;
		}
	}

	if (borrow) {
		for (ita; ita != a.end(); ita++) {
			diff = *ita - borrow;
			if (diff < 0) {
				*ita = base + diff;
			}
			else {
				*ita = diff;
				break;
			}
		}
	}

	while (a.back() == 0 && a.size() > 1) a.pop_back();
	return *this;
}

BigInt operator*(BigInt a, const BigInt& b)
{
	a *= b;
	return a;
}

BigInt operator-(BigInt a, const BigInt& b)
{
	a -= b;
	return a;
}

BigInt operator+(BigInt a, const BigInt& b)
{
	a += b;
	return a;
}

std::string BigInt::str()
{
	auto it = number.rbegin();
	std::string output(std::to_string(*it++));
	for (it; it != number.rend(); it++) {
		std::string s = std::to_string(*it);
		if (s.size() < dim) {
			std::reverse(s.begin(), s.end());
			while (s.size() % dim) s += '0';
			std::reverse(s.begin(), s.end());
		}
		output += s;
	}
	return output;
}

std::ostream& operator<<(std::ostream& os, const BigInt& obj)
{
	return os << const_cast<BigInt&>(obj).str();
}

std::string span(size_t len, char ch) {
	static std::string span;
	span.clear();
	while (len--) span += ch;
	return span;
}

void print_op(char op, const std::string& s1, const std::string& s2)
{
	BigInt a = s1;
	BigInt b = s2;
	BigInt r = 0;
	std::string res;
	int max = 0;
	
	if (op == '+' || op == '-') {
		if (op == '+') { r = a + b; }
		if (op == '-') { r = a - b; }
		res = r.str();
		max = std::max({ s1.size(), s2.size() + 1, res.size() });

		std::cout << span(max - s1.size(), ' ') << s1 << "\n" 
			<< span(max - s2.size() - 1, ' ') << op << s2 << "\n";
		if (s2.size() + 1 > res.size()) {
			std::cout << span(max - s2.size() - 1, ' ') << span(s2.size() + 1, '-') << "\n";
		}
		else {
			std::cout << span(max - res.size(), ' ') << span(res.size(), '-') << "\n";
		}
		std::cout << span(max - res.size(), ' ') << res << "\n";
	}
	else {
		r = a * b;
		res = r.str();
		max = std::max({ s1.size(), s2.size() + 1, res.size() });
		std::vector<std::string> products;
		
		for (auto it = s2.rbegin(); it != s2.rend(); it++) {
			products.push_back((a * BigInt(*it - '0')).str());
		}

		std::cout << span(max - s1.size(), ' ') << s1 << "\n"
			<< span(max - s2.size() - 1, ' ') << op << s2 << "\n";
		if (products.size() > 1) {
			if (products.front().size() >= s2.size() + 1) {
				std::cout << span(max - products.front().size(), ' ') <<
					span(products.front().size(), '-') << "\n";
			}
			else {
				std::cout << span(max - s2.size() - 1, ' ') <<
					span(s2.size() + 1, '-') << "\n";
			}
		}

		if (products.size() > 1) {
			int space = 0;
			for (auto s : products) {
				std::cout << span(max - s.size() - space++, ' ') << s << "\n";
			}
		}

		std::cout << span(max - res.size(), ' ') << span(res.size(), '-') << "\n"
			<< span(max - res.size(), ' ') << res << "\n";
	}
}

int main()
{
	size_t total = 0;
	while (std::cin) {
		std::string line;
		std::getline(std::cin, line);
		size_t pos = line.find_first_of("+-*");
		if (pos != std::string::npos) {
			char op = line[pos];
			std::string a = line.substr(0, pos);
			std::string b = line.substr(pos + 1, line.size() - pos);
			print_op(op, a, b);
			std::cout << "\n";
		}
	}

	return 0;
}
