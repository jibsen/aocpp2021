//
// Advent of Code 2021, day 1, part one
//

#include <iostream>
#include <iterator>
#include <limits>

int main()
{
	long prev = std::numeric_limits<long>::max();
	std::size_t num_increases = 0;

	for (std::istream_iterator<long> it(std::cin), end; it != end; ++it) {
		num_increases += static_cast<std::size_t>(*it > prev);
		prev = *it;
	}

	std::cout << num_increases << '\n';
}
