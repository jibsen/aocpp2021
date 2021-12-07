//
// Advent of Code 2021, day 7, part one
//

#include <cstdlib>
#include <iostream>
#include <limits>
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

	int best_pos = -1;
	long best_cost = std::numeric_limits<long>::max();

	for (auto pos : positions) {
		long cost = std::accumulate(positions.begin(), positions.end(), 0L,
			[&](long sum, int p) {
				return sum + std::abs(p - pos);
			}
		);

		if (cost < best_cost) {
			best_pos = pos;
			best_cost = cost;
		}
	}

	std::cout << best_cost << '\n';
}
