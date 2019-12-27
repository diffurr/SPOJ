//PRIME1 - Prime Generator
#include <iostream>
#include <vector>
#include <cmath>

std::vector<int> sieve(int limit)
{
    std::vector<bool> nums(static_cast<size_t>(limit), true);
    std::vector<int> primes;
    
    for (int i = 2; i * i < limit; i++)
    {
        if (nums[i]) {
            for (int j = i * 2; j < limit; j += i) {
                nums[j] = false;
            }
        }
    }

    for (int i = 2; i < nums.size(); i++) {
        if (nums[i]) {
            primes.push_back(i);
        }
    }

    return primes;
}

std::vector<int> segmented_sieve(const std::vector<int> &sieve, int low, int high)
{
    std::vector<int> primes;
    if (low < 2) {
        low = 2;
    }
    if (low > high) {
        return primes;
    }

    int n = high - low + 1;
    std::vector<bool> mark(static_cast<size_t>(n) + 1, false);

    for (int i = 0; i < sieve.size(); i++) {
        int lolim = low / sieve[i] * sieve[i];
        if (lolim < low) {
            lolim += sieve[i];
        }
        if (lolim == sieve[i]) {
            lolim += sieve[i];
        }
        for (size_t j = lolim; j <= high; j += sieve[i]) {
            mark[j - low] = true;
        }
    }

    for (size_t i = low; i <= high; i++) {
        if (!mark[i - low]) {
            primes.push_back(i);
        }
    }

    return primes;
}

std::vector<bool> atkin(int limit)
{
    std::vector<bool> nums(static_cast<size_t>(limit), false);
    if (limit > 2)
        nums[2] = true;
    if (limit > 3)
        nums[3] = true;

    for (int x = 1; x * x < limit; x++) {
        for (int y = 1; y * y < limit; y++) {

            int n = (4 * x * x) + (y * y);
            if (n <= limit && (n % 12 == 1 || n % 12 == 5)) {
                nums[n] = nums[n] ^ true;
            }

            n = (3 * x * x) + (y * y);
            if (n <= limit && n % 12 == 7) {
                nums[n] = nums[n] ^ true;
            }

            n = (3 * x * x) - (y * y);
            if (x > y && n <= limit && n % 12 == 11) {
                nums[n] = nums[n] ^ true;
            }
        }
    }

    for (int r = 5; r * r < limit; r++) {
        if (nums[r]) {
            for (int i = r * r; i < limit; i += r * r) {
                nums[i] = false;
            }
        }
    }

    return nums;
}

int main()
{
    std::vector<int> input;
    int num;
    constexpr int limit = 1000000000;

    std::cin >> num;
    while (std::cin >> num) {
        input.push_back(num);
    }

    const std::vector<int> primes = sieve(std::sqrt(limit) + 1);

    for (size_t i = 0; i < input.size(); i += 2) {
        for (auto n : segmented_sieve(primes, input[i], input[i + 1])) {
            std::cout << n << "\n";
        }
        std::cout << "\n";
    }

    return 0;
}