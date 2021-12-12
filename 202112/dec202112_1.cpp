//
// Advent of Code 2021, day 12, part one
//

#include <iostream>
#include <cctype>
#include <queue>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Graph = std::unordered_map<std::string, std::vector<std::string>>;

auto read_paths()
{
	Graph adj;

	for (std::string line; std::getline(std::cin, line); ) {
		if (auto dash = line.find('-'); dash != std::string::npos) {
			auto from = line.substr(0, dash);
			auto to = line.substr(dash + 1);

			adj[from].push_back(to);
			adj[to].push_back(from);
		}
	}

	return adj;
}

bool is_big_cave(std::string_view name)
{
	return std::isupper(static_cast<unsigned char>(name.front()));
}

std::size_t count_paths(const Graph &adj, std::string cur, std::size_t num_paths)
{
	static std::unordered_set<std::string> visited;

	if (cur == "end") {
		return num_paths + 1;
	}

	visited.insert(cur);

	for (const auto &next : adj.at(cur)) {
		if (is_big_cave(next) || !visited.contains(next)) {
			num_paths = count_paths(adj, next, num_paths);
		}
	}

	visited.erase(cur);

	return num_paths;
}

int main()
{
	auto adj = read_paths();

	std::cout << count_paths(adj, "start", 0) << '\n';
}
