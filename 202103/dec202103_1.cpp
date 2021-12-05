//
// Advent of Code 2021, day 3, part one
//

#include <array>
#include <iostream>
#include <string>

int main()
{
	constexpr int bits = 12;

	std::array<std::size_t, bits> counts = {};

	std::size_t num_lines = 0;

	for (std::string line; std::cin >> line; ++num_lines) {
		for (int i = 0; i < bits; ++i) {
			counts[i] += static_cast<std::size_t>(line[i] == '1');
		}
	}

	std::uint16_t gamma = 0;

	for (int i = 0; i < bits; ++i) {
		if (counts[i] > num_lines - counts[i]) {
			gamma |= 1U << ((bits - 1) - i);
		}
	}

	std::cout << gamma * (gamma ^ ((1U << bits) - 1)) << '\n';
}
