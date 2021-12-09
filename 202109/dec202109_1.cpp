//
// Advent of Code 2021, day 9, part one
//

#include <array>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

bool low_point(const std::vector<std::string> &map, int x, int y)
{
	const std::array<std::array<int, 2>, 4> directions = {{
		{ 0, 1 }, { 0, -1 }, { -1, 0 }, { 1, 0 }
	}};

	for (auto [dx, dy] : directions) {
		if (x + dx < 0 || x + dx >= map[y].size()
		 || y + dy < 0 || y + dy >= map.size()) {
			continue;
		}

		if (map[y + dy][x + dx] <= map[y][x]) {
			return false;
		}
	}

	return true;
}

int main()
{
	std::vector<std::string> map(std::istream_iterator<std::string>{std::cin},
	                             std::istream_iterator<std::string>{});

	int sum = 0;

	for (int y = 0; y < map.size(); ++y) {
		for (int x = 0; x < map[y].size(); ++x) {
			if (low_point(map, x, y)) {
				sum += (map[y][x] - '0') + 1;
			}
		}
	}

	std::cout << sum << '\n';
}
