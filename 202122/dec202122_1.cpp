//
// Advent of Code 2021, day 22, part one
//

#include <algorithm>
#include <array>
#include <format>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

struct ArrayHash {
	template<typename T, std::size_t size>
	std::size_t operator()(const std::array<T, size> &array) const noexcept
	{
		std::size_t hash = 17;
		for (const auto &elem : array) {
			hash = hash * 37 + std::hash<T>()(elem);
		}
		return hash;
	}
};

struct Cuboid {
	int x1 = 0, x2 = 0;
	int y1 = 0, y2 = 0;
	int z1 = 0, z2 = 0;
};

auto read_reboot_steps()
{
	std::vector<std::pair<bool, Cuboid>> steps;

	for (std::string line; std::getline(std::cin, line); ) {
		bool status = line[1] == 'n';

		std::string::size_type pos = 0;

		pos = line.find("x=", pos);
		int x1 = std::stoi(line.substr(pos + 2));

		pos = line.find("..", pos);
		int x2 = std::stoi(line.substr(pos + 2));

		pos = line.find("y=", pos);
		int y1 = std::stoi(line.substr(pos + 2));

		pos = line.find("..", pos);
		int y2 = std::stoi(line.substr(pos + 2));

		pos = line.find("z=", pos);
		int z1 = std::stoi(line.substr(pos + 2));

		pos = line.find("..", pos);
		int z2 = std::stoi(line.substr(pos + 2));

		steps.emplace_back(status, Cuboid{x1, x2, y1, y2, z1, z2});
	}

	return steps;
}

constexpr bool has_overlap(const Cuboid &lhs, const Cuboid &rhs)
{
	return lhs.x1 <= rhs.x2 && lhs.x2 >= rhs.x1
	    && lhs.y1 <= rhs.y2 && lhs.y2 >= rhs.y1
	    && lhs.z1 <= rhs.z2 && lhs.z2 >= rhs.z1;
}

constexpr Cuboid intersect_overlapping_cuboids(const Cuboid &lhs, const Cuboid &rhs)
{
	return {std::max(lhs.x1, rhs.x1), std::min(lhs.x2, rhs.x2),
	        std::max(lhs.y1, rhs.y1), std::min(lhs.y2, rhs.y2),
	        std::max(lhs.z1, rhs.z1), std::min(lhs.z2, rhs.z2)};
}

int main()
{
	auto steps = read_reboot_steps();

	std::cout << std::format("{} steps read\n", steps.size());

	std::unordered_set<std::array<int, 3>, ArrayHash> on_cubes;

	for (const auto &[status, cuboid] : steps) {
		if (has_overlap(cuboid, {-50, 50, -50, 50, -50, 50})) {
			auto overlap = intersect_overlapping_cuboids(cuboid, {-50, 50, -50, 50, -50, 50});

			for (int x = overlap.x1; x <= overlap.x2; ++x) {
				for (int y = overlap.y1; y <= overlap.y2; ++y) {
					for (int z = overlap.z1; z <= overlap.z2; ++z) {
						if (status) {
							on_cubes.insert({x, y, z});
						}
						else {
							on_cubes.erase({x, y, z});
						}
					}
				}
			}
		}
	}

	std::cout << on_cubes.size() << '\n';
}
