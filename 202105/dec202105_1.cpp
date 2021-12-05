//
// Advent of Code 2021, day 5, part one
//

#include <algorithm>
#include <format>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

struct Line {
	int x1, y1;
	int x2, y2;
};

auto read_lines()
{
	std::vector<Line> lines;

	std::string arrow;
	char sep;

	Line line;

	while (std::cin >> line.x1 >> sep >> line.y1 >> arrow >> line.x2 >> sep >> line.y2) {
		lines.push_back(line);
	}

	return lines;
}

int main()
{
	auto lines = read_lines();

	std::vector<std::vector<int>> map;

	map.reserve(1000);

	for (int i = 0; i < 1000; ++i) {
		map.emplace_back(1000, 0);
	}

	std::cout << std::format("{} lines read\n", lines.size());

	auto horizontal_or_vertical = [](const auto &line) {
		return line.x1 == line.x2 || line.y1 == line.y2;
	};

	for (auto &line : lines | std::views::filter(horizontal_or_vertical)) {
		auto [x1, x2] = std::minmax(line.x1, line.x2);
		auto [y1, y2] = std::minmax(line.y1, line.y2);

		for (int x = x1; x <= x2; ++x) {
			for (int y = y1; y <= y2; ++y) {
				map[y][x]++;
			}
		}
	}

	std::cout << std::accumulate(map.begin(), map.end(), 0,
		[](auto sum, auto &row) {
			return sum + std::ranges::count_if(row, [](auto n) { return n > 1; });
		}
	) << '\n';
}
