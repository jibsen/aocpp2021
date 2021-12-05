//
// Advent of Code 2021, day 3, part two
//

// Rather than counting the number of bits at each position, we can partition
// the numbers based on if a bit is set, and then continue the partitioning
// on the larger or smaller subrange.

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

constexpr int bits = 12;

unsigned long get_rating(std::vector<unsigned long> &numbers, bool find_most_common)
{
	auto lo = numbers.begin();
	auto hi = numbers.end();

	for (unsigned long mask = 1UL << (bits - 1); mask; mask >>= 1) {
		auto mid = std::partition(lo, hi, [&](auto n) { return n & mask; });

		if (std::distance(lo, mid) >= std::distance(mid, hi)) {
			if (find_most_common) {
				hi = mid;
			}
			else {
				lo = mid;
			}
		}
		else {
			if (find_most_common) {
				lo = mid;
			}
			else {
				hi = mid;
			}
		}

		if (hi - lo == 1) {
			break;
		}
	}

	return *lo;
}

int main()
{
	std::vector<unsigned long> numbers;

	for (std::string line; std::cin >> line; ) {
		numbers.push_back(std::stoul(line, nullptr, 2));
	}

	std::cout << get_rating(numbers, true) * get_rating(numbers, false) << '\n';
}
