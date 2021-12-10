//
// Advent of Code 2021, day 9, part two
//

#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <queue>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

int fill_basin(std::vector<std::string> &map, int start_x, int start_y)
{
	const std::array<std::array<int, 2>, 4> directions = {{
		{ 0, 1 }, { 0, -1 }, { -1, 0 }, { 1, 0 }
	}};

	std::queue<std::pair<int, int>> queue;

	queue.emplace(start_x, start_y);

	int size = 0;

	while (!queue.empty()) {
		auto [x, y] = queue.front();
		queue.pop();

		if (map[y][x] == '9') {
			continue;
		}

		map[y][x] = '9';
		++size;

		for (auto [dx, dy] : directions) {
			if (x + dx < 0 || x + dx >= map[y].size()
			 || y + dy < 0 || y + dy >= map.size()) {
				continue;
			}

			queue.emplace(x + dx, y + dy);
		}
	}

	return size;
}

int main()
{
	std::vector<std::string> map(std::istream_iterator<std::string>{std::cin},
	                             std::istream_iterator<std::string>{});

	std::vector<int> sizes;

	for (int y = 0; y < map.size(); ++y) {
		for (int x = 0; x < map[y].size(); ++x) {
			if (map[y][x] == '9') {
				continue;
			}

			auto size = fill_basin(map, x, y);

			sizes.push_back(size);
		}
	}

	std::ranges::nth_element(sizes, sizes.begin() + 2, std::greater{});

	std::cout << std::accumulate(sizes.begin(), sizes.begin() + 3, 1, std::multiplies{}) << '\n';
}
