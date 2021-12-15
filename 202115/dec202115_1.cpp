//
// Advent of Code 2021, day 15, part one
//

#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <limits>
#include <queue>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

struct PairHash {
	template<typename T1, typename T2>
	std::size_t operator()(const std::pair<T1, T2> &p) const noexcept
	{
		std::size_t h1 = std::hash<T1>()(p.first);
		std::size_t h2 = std::hash<T2>()(p.second);

		return (17 * 37 + h1) * 37 + h2;
	}
};

using Map = std::vector<std::vector<int>>;

auto read_map()
{
	Map map;

	for (std::string line; std::getline(std::cin, line); ) {
		std::vector<int> row;

		std::ranges::transform(line, std::back_inserter(row),
			[](auto ch) {
				return ch - '0';
			}
		);

		map.push_back(std::move(row));
	}

	return map;
}

int main()
{
	auto map = read_map();

	const int target_y = map.size() - 1;
	const int target_x = map[target_y].size() - 1;

	std::unordered_map<std::pair<int, int>, std::size_t, PairHash> risks;

	std::priority_queue<std::tuple<std::size_t, int, int>,
	                    std::vector<std::tuple<std::size_t, int, int>>,
			    std::greater<>> pqueue;

	pqueue.emplace(0, 0, 0);

	while (!pqueue.empty()) {
		auto [risk_here, x, y] = pqueue.top();
		pqueue.pop();

		if (x == target_x && y == target_y) {
			std::cout << risk_here << '\n';
			break;
		}

		constexpr std::array<std::pair<int, int>, 4> directions = {{
			{ 0, 1 }, { 1, 0 }, { -1, 0 }, { 0, -1 }
		}};

		for (auto [dx, dy] : directions) {
			if (x + dx < 0 || x + dx >= map[y].size()
			 || y + dy < 0 || y + dy >= map.size()) {
				continue;
			}

			std::size_t new_risk = risk_here + map[y + dy][x + dx];

			if (auto [it, success] = risks.insert({{x + dx, y + dy}, new_risk}); !success) {
				if (new_risk < it->second) {
					it->second = new_risk;
					pqueue.emplace(new_risk, x + dx, y + dy);
				}
			}
			else {
				pqueue.emplace(new_risk, x + dx, y + dy);
			}
		}
	}
}
