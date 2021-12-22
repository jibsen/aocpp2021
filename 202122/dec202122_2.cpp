//
// Advent of Code 2021, day 22, part two
//

#include <algorithm>
#include <array>
#include <format>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

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

constexpr std::size_t volume(const Cuboid &cb)
{
	return static_cast<std::size_t>(cb.x2 - cb.x1 + 1)
	     * static_cast<std::size_t>(cb.y2 - cb.y1 + 1)
	     * static_cast<std::size_t>(cb.z2 - cb.z1 + 1);
}

std::vector<Cuboid> subdivide_cuboid(const Cuboid &cb, const Cuboid &overlap)
{
	// Assuming overlap is a subset of cb, these 3 subranges of cb's
	// x, y, and z ranges correspond to the (up to) 27 non-overlapping
	// subcuboids. This is not ideal, but quite simple.

	std::array<std::pair<int, int>, 3> x_ranges = {{
		{cb.x1, overlap.x1 - 1}, {overlap.x1, overlap.x2}, {overlap.x2 + 1, cb.x2}
	}};

	std::array<std::pair<int, int>, 3> y_ranges = {{
		{cb.y1, overlap.y1 - 1}, {overlap.y1, overlap.y2}, {overlap.y2 + 1, cb.y2}
	}};

	std::array<std::pair<int, int>, 3> z_ranges = {{
		{cb.z1, overlap.z1 - 1}, {overlap.z1, overlap.z2}, {overlap.z2 + 1, cb.z2}
	}};

	std::vector<Cuboid> res;

	// Find all non-empty subdivisions excluding overlap
	for (auto [x1, x2] : x_ranges) {
		for (auto [y1, y2] : y_ranges) {
			for (auto [z1, z2] : z_ranges) {
				if (x1 > x2 || y1 > y2 || z1 > z2) {
					continue;
				}

				if (x1 == overlap.x1 && x2 == overlap.x2
				 && y1 == overlap.y1 && y2 == overlap.y2
				 && z1 == overlap.z1 && z2 == overlap.z2) {
					continue;
				}

				res.push_back({x1, x2, y1, y2, z1, z2});
			}
		}
	}

	return res;
}

std::vector<Cuboid> remove_overlaps(std::vector<Cuboid> cuboids)
{
	bool overlap_found = false;

	std::vector<Cuboid> non_overlapping_cuboids;

	// The idea is to repeatedly find two overlapping cuboids and
	// subdivide them so they no longer overlap. Cuboids that do not
	// overlap with any others are moved to non_overlapping_cuboids.

	for (;;) {
		std::ranges::sort(cuboids,
			[](const Cuboid &lhs, const Cuboid &rhs) {
				return std::make_tuple(lhs.x1, lhs.y1, lhs.z1)
				     < std::make_tuple(rhs.x1, rhs.y1, rhs.z1);
			}
		);

		bool overlap_found = false;
		int overlap_i = -1;
		int overlap_j = -1;

		for (int i = 0; i < cuboids.size(); ++i) {
			for (int j = i + 1; j < cuboids.size(); ++j) {
				if (has_overlap(cuboids[i], cuboids[j])) {
					overlap_found = true;
					overlap_i = i;
					overlap_j = j;
					break;
				}
			}

			if (overlap_found) {
				break;
			}
		}

		if (!overlap_found) {
			non_overlapping_cuboids.insert(non_overlapping_cuboids.end(), cuboids.begin(), cuboids.end());
			break;
		}

		// Cuboids up to overlap_i did not overlap with any other
		if (overlap_i > 0) {
			non_overlapping_cuboids.insert(non_overlapping_cuboids.end(), cuboids.begin(), cuboids.begin() + overlap_i);
		}

		auto overlap = intersect_overlapping_cuboids(cuboids[overlap_i], cuboids[overlap_j]);

		auto i_subs = subdivide_cuboid(cuboids[overlap_i], overlap);
		auto j_subs = subdivide_cuboid(cuboids[overlap_j], overlap);

		std::vector<Cuboid> subcuboids;

		subcuboids.reserve(i_subs.size() + 1 + j_subs.size() + cuboids.size() - overlap_i);

		// Insert the subcuboids that add up to i and j
		subcuboids.insert(subcuboids.end(), i_subs.begin(), i_subs.end());
		subcuboids.push_back(overlap);
		subcuboids.insert(subcuboids.end(), j_subs.begin(), j_subs.end());

		// Keep any remaining cuboids we did not process
		for (int k = overlap_i + 1; k < cuboids.size(); ++k) {
			if (k != overlap_j) {
				subcuboids.push_back(cuboids[k]);
			}
		}

		cuboids.swap(subcuboids);
	}

	return non_overlapping_cuboids;
}

int main()
{
	auto steps = read_reboot_steps();

	std::cout << std::format("{} steps read\n", steps.size());

	std::vector<Cuboid> subcuboids;

	subcuboids.reserve(steps.size());

	std::ranges::transform(steps, std::back_inserter(subcuboids),
	                       [](const auto &s) { return s.second; });

	subcuboids = remove_overlaps(subcuboids);

	std::cout << std::format("{} subcuboids after removing overlaps\n", subcuboids.size());

	std::vector<bool> states(subcuboids.size(), false);

	for (const auto &[status, cuboid] : steps) {
		for (int i = 0; i < subcuboids.size(); ++i) {
			if (has_overlap(cuboid, subcuboids[i])) {
				states[i] = status;
			}
		}
	}

	std::size_t on_cubes = 0;

	for (int i = 0; i < subcuboids.size(); ++i) {
		if (states[i]) {
			on_cubes += volume(subcuboids[i]);
		}
	}

	std::cout << on_cubes << '\n';
}
