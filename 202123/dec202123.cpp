//
// Advent of Code 2021, day 23, part one and two
//

#include <algorithm>
#include <array>
#include <format>
#include <iostream>
#include <iterator>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using State = std::vector<std::string>;

struct StateHash {
	std::size_t operator()(const State &state) const noexcept
	{
		std::size_t hash = 17;
		for (const auto &line : state) {
			hash = hash * 37 + std::hash<std::string>()(line);
		}
		return hash;
	}
};

struct PairHash {
	template<typename T1, typename T2>
	std::size_t operator()(const std::pair<T1, T2> &p) const noexcept
	{
		std::size_t h1 = std::hash<T1>()(p.first);
		std::size_t h2 = std::hash<T2>()(p.second);
		return (17 * 37 + h1) * 37 + h2;
	}
};

auto read_state()
{
	State state;

	for (std::string line; std::getline(std::cin, line); ) {
		state.push_back(std::move(line));
	}

	return state;
}

bool is_final_state(const State &state)
{
	constexpr std::array<std::pair<char, int>, 4> rooms = {{
		{'A', 3}, {'B', 5}, {'C', 7}, {'D', 9}
	}};

	for (auto [type, column] : rooms) {
		for (int row = 2; state[row][column] != '#'; ++row) {
			if (state[row][column] != type) {
				return false;
			}
		}
	}

	return true;
}

bool is_in_final_spot(const State &state, int x, int y)
{
	if (y < 2) {
		return false;
	}

	constexpr std::array<std::pair<char, int>, 4> rooms = {{
		{'A', 3}, {'B', 5}, {'C', 7}, {'D', 9}
	}};

	for (auto [type, column] : rooms) {
		if (state[y][x] != type) {
			continue;
		}

		if (x != column) {
			return false;
		}

		for (int row = y; state[row][column] != '#'; ++row) {
			if (state[row][column] != '.' && state[row][column] != type) {
				return false;
			}
		}
	}

	return true;
}

std::pair<bool, std::size_t> search_path(const State &state, int start_x, int start_y, int end_x, int end_y)
{
	std::unordered_map<std::pair<int, int>, std::size_t, PairHash> distance;

	std::queue<std::pair<int, int>> queue;

	queue.push({start_x, start_y});
	distance[{start_x, start_y}] = 0;

	while (!queue.empty()) {
		auto [x, y] = queue.front();
		queue.pop();

		auto dist = distance[{x, y}];

		if (x == end_x && y == end_y) {
			switch (state[start_y][start_x]) {
			case 'A':
				return {true, dist};
			case 'B':
				return {true, 10 * dist};
			case 'C':
				return {true, 100 * dist};
			case 'D':
				return {true, 1000 * dist};
			default:
				std::cerr << "search_path error\n";
				return {false, 0};
			}
		}

		constexpr std::array<std::pair<int, int>, 4> directions = {{
			{ -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 }
		}};

		for (auto [dx, dy] : directions) {
			if (state[y + dy][x + dx] != '.') {
				continue;
			}

			if (auto [it, success] = distance.insert({{x + dx, y + dy}, dist + 1}); success) {
				queue.push({x + dx, y + dy});
			}
		}
	}

	return {false, 0};
}

std::vector<std::pair<std::size_t, std::pair<int, int>>> moves_from_room(const State &state, int x, int y)
{
	constexpr std::array<std::pair<int, int>, 7> hallway = {{
		{ 1, 1 }, { 2, 1 }, { 4, 1 }, { 6, 1 }, { 8, 1 }, { 10, 1 }, { 11, 1 }
	}};

	std::vector<std::pair<std::size_t, std::pair<int, int>>> moves;

	for (auto [end_x, end_y] : hallway) {
		if (state[end_y][end_x] != '.') {
			continue;
		}

		auto [possible, dist] = search_path(state, x, y, end_x, end_y);

		if (possible) {
			moves.push_back({dist, {end_x, end_y}});
		}
	}

	return moves;
}

std::pair<bool, std::pair<int, int>> move_from_hallway(const State &state, int x, int y)
{
	constexpr std::array<std::pair<char, int>, 4> rooms = {{
		{'A', 3}, {'B', 5}, {'C', 7}, {'D', 9}
	}};

	for (auto [type, column] : rooms) {
		if (state[y][x] != type) {
			continue;
		}

		for (int row = 2; state[row][column] != '#'; ++row) {
			if (state[row][column] != '.' && state[row][column] != type) {
				return {false, {0, 0}};
			}
		}

		for (int row = 2; ; ++row) {
			if (state[row][column] != '.') {
				return {true, {column, row - 1}};
			}
		}
	}

	return {false, {0, 0}};
}

std::size_t organize_amphipods(State &state, std::size_t energy)
{
	static std::unordered_map<State, std::size_t, StateHash> memory;

	if (is_final_state(state)) {
		return energy;
	}

	if (auto it = memory.find(state); it != memory.end()) {
		if (it->second == std::numeric_limits<std::size_t>::max()) {
			return it->second;
		}
		else {
			return energy + it->second;
		}
	}

	std::vector<std::pair<std::size_t, std::tuple<int, int, int, int>>> moves;

	// Each step we can either take one misplaced amphipod and move it
	// into an empty space in the hallway, or take one amphipod from
	// the hallway and move it into its final position in a room

	for (int y = 0; y < state.size(); ++y) {
		for (int x = 0; x < state[y].size(); ++x) {
			if (state[y][x] != 'A' && state[y][x] != 'B' && state[y][x] != 'C' && state[y][x] != 'D') {
				continue;
			}

			if (y == 1) {
				auto [found, move] = move_from_hallway(state, x, y);

				if (found) {
					auto [end_x, end_y] = move;

					auto [possible, dist] = search_path(state, x, y, end_x, end_y);

					if (possible) {
						moves.push_back({dist, {x, y, end_x, end_y}});
					}
				}
			}
			else {
				if (is_in_final_spot(state, x, y)) {
					continue;
				}

				auto new_moves = moves_from_room(state, x, y);

				for (auto [dist, end] : new_moves) {
					auto [end_x, end_y] = end;

					moves.push_back({dist, {x, y, end_x, end_y}});
				}
			}
		}
	}

	std::ranges::sort(moves,
		[](const auto &lhs, const auto &rhs) {
			return lhs.first < rhs.first;
		}
	);

	std::size_t least_energy = std::numeric_limits<std::size_t>::max();

	for (const auto &[cost, move] : moves) {
		auto [from_x, from_y, to_x, to_y] = move;

		state[to_y][to_x] = state[from_y][from_x];
		state[from_y][from_x] = '.';

		least_energy = std::min(least_energy, organize_amphipods(state, energy + cost));

		state[from_y][from_x] = state[to_y][to_x];
		state[to_y][to_x] = '.';
	}

	if (least_energy == std::numeric_limits<std::size_t>::max()) {
		memory[state] = least_energy;
	}
	else {
		memory[state] = least_energy - energy;
	}

	return least_energy;
}

int main()
{
	auto state = read_state();

	std::cout << organize_amphipods(state, 0) << '\n';
}
