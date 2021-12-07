//
// Advent of Code 2021, day 7, part one
//

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <vector>

auto read_positions()
{
	std::vector<int> positions;
	int pos = 0;

	for (char sep = ','; sep == ',' && std::cin >> pos; std::cin >> sep) {
		positions.push_back(pos);
	}

	return positions;
}

int main()
{
	auto positions = read_positions();

	std::ranges::nth_element(positions, positions.begin() + positions.size() / 2);

	auto median = positions[positions.size() / 2];

	std::cout << std::accumulate(positions.begin(), positions.end(), 0L,
		[&](long sum, int pos) {
			return sum + std::abs(median - pos);
		}
	) << '\n';
}
