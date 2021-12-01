//
// Advent of Code 2021, day 1, part two
//

#include <iostream>
#include <iterator>
#include <vector>

int main()
{
	std::vector<long> depths(std::istream_iterator<long>{std::cin},
	                         std::istream_iterator<long>{});

	std::size_t num_increases = 0;

	for (std::vector<long>::size_type i = 3; i < depths.size(); ++i) {
		num_increases += static_cast<std::size_t>(depths[i] > depths[i - 3]);
	}

	std::cout << num_increases << '\n';
}
