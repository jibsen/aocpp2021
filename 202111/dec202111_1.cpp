//
// Advent of Code 2021, day 11, part one
//

#include <array>
#include <iostream>
#include <iterator>
#include <queue>
#include <string>
#include <utility>
#include <vector>

using Map = std::vector<std::vector<int>>;

auto read_map()
{
	Map map;

	map.reserve(10);

	for (std::string line; std::getline(std::cin, line); ) {
		std::vector<int> row;

		row.reserve(10);

		std::ranges::transform(line, std::back_inserter(row),
			[](auto ch) {
				return ch - '0';
			}
		);

		map.push_back(std::move(row));
	}

	return map;
}

void flash(Map &map, int start_x, int start_y)
{
	constexpr std::array<std::array<int, 2>, 8> directions = {{
		{ 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 },
		{ 0, -1 }, { -1, -1 }, { -1, 0 }, { -1, 1 }
	}};

	std::queue<std::pair<int, int>> queue;

	queue.emplace(start_x, start_y);

	while (!queue.empty()) {
		auto [x, y] = queue.front();
		queue.pop();

		for (auto [dx, dy] : directions) {
			if (x + dx < 0 || x + dx >= map[y].size()
			 || y + dy < 0 || y + dy >= map.size()) {
				continue;
			}

			++map[y + dy][x + dx];

			if (map[y + dy][x + dx] == 10) {
				queue.emplace(x + dx, y + dy);
			}
		}
	}
}

std::size_t count_and_reset_flashes(Map &map)
{
	std::size_t num_flashes = 0;

	for (auto &row : map) {
		for (auto &cell : row) {
			if (cell > 9) {
				cell = 0;
				++num_flashes;
			}
		}
	}

	return num_flashes;
}

int main()
{
	auto map = read_map();

	std::size_t num_flashes = 0;

	for (int step = 0; step < 100; ++step) {

		for (int y = 0; y < map.size(); ++y) {
			for (int x = 0; x < map[y].size(); ++x) {
				++map[y][x];

				if (map[y][x] == 10) {
					flash(map, x, y);
				}
			}
		}

		num_flashes += count_and_reset_flashes(map);
	}

	std::cout << num_flashes << '\n';
}
