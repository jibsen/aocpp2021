//
// Advent of Code 2021, day 7, part two
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

long fuel_cost(const std::vector<int> &positions, int final_pos)
{
	return std::accumulate(positions.begin(), positions.end(), 0L,
		[&](long sum, int pos) {
			long dist = std::abs(final_pos - pos);
			return sum + (dist * (dist + 1)) / 2;
		}
	);
}

int main()
{
	auto positions = read_positions();

	long sum = std::accumulate(positions.begin(), positions.end(), 0L);

	long mean = (sum + positions.size() / 2) / positions.size();

	std::cout << std::min({fuel_cost(positions, mean - 1),
	                       fuel_cost(positions, mean),
			       fuel_cost(positions, mean + 1)});
}
