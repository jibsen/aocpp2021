//
// Advent of Code 2021, day 25, part one
//

#include <iostream>
#include <string>
#include <vector>

using Map = std::vector<std::string>;

auto read_map()
{
	Map map;

	for (std::string line; std::getline(std::cin, line); ) {
		map.push_back(std::move(line));
	}

	return map;
}

std::size_t move_east(Map &map)
{
	for (int y = 0; y < map.size(); ++y) {
		for (int x = 0; x < map[y].size(); ++x) {
			if (map[y][x] != '>') {
				continue;
			}

			int new_x = (x + 1) % map[y].size();

			if (map[y][new_x] == '.') {
				map[y][x] = 'm';
			}
		}
	}

	std::size_t num_moves = 0;

	for (int y = 0; y < map.size(); ++y) {
		for (int x = 0; x < map[y].size(); ++x) {
			if (map[y][x] != 'm') {
				continue;
			}

			int new_x = (x + 1) % map[y].size();

			map[y][new_x] = '>';
			map[y][x] = '.';

			++num_moves;
		}
	}

	return num_moves;
}

std::size_t move_south(Map &map)
{
	for (int y = 0; y < map.size(); ++y) {
		for (int x = 0; x < map[y].size(); ++x) {
			if (map[y][x] != 'v') {
				continue;
			}

			int new_y = (y + 1) % map.size();

			if (map[new_y][x] == '.') {
				map[y][x] = 'm';
			}
		}
	}

	std::size_t num_moves = 0;

	for (int y = 0; y < map.size(); ++y) {
		for (int x = 0; x < map[y].size(); ++x) {
			if (map[y][x] != 'm') {
				continue;
			}

			int new_y = (y + 1) % map.size();

			map[new_y][x] = 'v';
			map[y][x] = '.';

			++num_moves;
		}
	}

	return num_moves;
}

int main()
{
	auto map = read_map();

	std::size_t steps = 0;

	for (;;) {
		auto east_moves = move_east(map);
		auto south_moves = move_south(map);

		++steps;

		if (east_moves == 0 && south_moves == 0) {
			break;
		}
	}

	std::cout << steps << '\n';
}
