//
// Advent of Code 2021, day 19, part two
//

#include <algorithm>
#include <array>
#include <cstdlib>
#include <format>
#include <iostream>
#include <queue>
#include <ranges>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Vec3 = std::array<int, 3>;

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

auto read_scanner_reports()
{
	std::vector<std::vector<Vec3>> scanners;
	std::string line;

	while (std::getline(std::cin, line) && line.starts_with("---")) {
		std::vector<Vec3> report;

		while (std::getline(std::cin, line) && !line.empty()) {
			std::istringstream iss(line);

			int x = 0;
			int y = 0;
			int z = 0;

			for (char sep = ','; iss >> x >> sep >> y >> sep >> z; ) {
				report.push_back({x, y, z});
			}
		}

		std::ranges::sort(report);

		scanners.push_back(std::move(report));
	}

	return scanners;
}

constexpr int apply_sign(int v, int sign)
{
	return sign < 0 ? -v : v;
}

Vec3 transform_point(const Vec3 &vec, const Vec3 &perm, const Vec3 &translation)
{
	return {apply_sign(vec[std::abs(perm[0]) - 1], perm[0]) + translation[0],
	        apply_sign(vec[std::abs(perm[1]) - 1], perm[1]) + translation[1],
	        apply_sign(vec[std::abs(perm[2]) - 1], perm[2]) + translation[2]};
}

std::pair<Vec3, std::size_t> most_common_difference(const std::vector<Vec3> &lhs, const std::vector<Vec3> &rhs)
{
	std::unordered_map<Vec3, std::size_t, ArrayHash> counts;

	for (auto [lx, ly, lz] : lhs) {
		for (auto [rx, ry, rz] : rhs) {
			counts[{lx - rx, ly - ry, lz - rz}]++;
		}
	}

	return *std::ranges::max_element(counts,
		[](const auto &lhs, const auto &rhs) {
			return lhs.second < rhs.second;
		}
	);
}

std::tuple<bool, Vec3, Vec3> find_scanner_overlap(const std::vector<Vec3> &lhs, const std::vector<Vec3> &rhs)
{
	// In this array, 1 means take the first coordinate, 2 the second,
	// and 3 the third. If a number is negative it means change the sign.
	// These permutations of a point correspond to the 24 different
	// orientations of a scanner.
	constexpr std::array<Vec3, 24> orientation_perms = {{
		{ 1, 2, 3 }, { 1, 3, -2 }, { 1, -2, -3 }, { 1, -3, 2 },
		{ -1, 3, 2 }, { -1, -2, 3 }, { -1, -3, -2 }, { -1, 2, -3 },
		{ 2, 3, 1 }, { 2, 1, -3 }, { 2, -3, -1 }, { 2, -1, 3 },
		{ -2, 1, 3 }, { -2, -3, 1 }, { -2, -1, -3 }, { -2, 3, -1 },
		{ 3, 1, 2 }, { 3, 2, -1 }, { 3, -1, -2 }, { 3, -2, 1 },
		{ -3, 2, 1 }, { -3, -1, 2 }, { -3, -2, -1 }, { -3, 1, -2 }
	}};

	for (const auto &perm : orientation_perms) {
		// Orientate rhs points according to perm
		auto perm_view = rhs | std::views::transform(
			[&](const Vec3 &beacon) -> Vec3 {
				return transform_point(beacon, perm, {0, 0, 0});
			}
		);

		std::vector<Vec3> beacons(perm_view.begin(), perm_view.end());

		std::ranges::sort(beacons);

		// Find most common difference between the two sets, which
		// is the vec we need to add to the rhs points to translate
		// them to be relative to the lhs scanner
		auto [vec, count] = most_common_difference(lhs, beacons);

		if (count >= 12) {
			return {true, vec, perm};
		}
	}

	return {};
}

Vec3 combine_perms(const Vec3 &first, const Vec3 &second)
{
	Vec3 res = {};

	for (int i = 0; i < res.size(); ++i) {
		res[i] = apply_sign(first[std::abs(second[i]) - 1], second[i]);
	}

	return res;
}

int main()
{
	auto scanners = read_scanner_reports();

	std::cout << std::format("{} scanner reports read\n", scanners.size());

	std::unordered_map<int, Vec3> translations;
	std::unordered_map<int, Vec3> perms;

	std::vector<bool> visited(scanners.size(), false);
	std::queue<int> queue;

	queue.push(0);

	translations[0] = {0, 0, 0};
	perms[0] = {1, 2, 3};

	while (!queue.empty()) {
		auto cur = queue.front();
		queue.pop();

		visited[cur] = true;

		auto trans_cur = translations[cur];
		auto perm_cur = perms[cur];

		std::cout << std::format(" - scanner {} is at {},{},{} relative to 0\n",
		                         cur, trans_cur[0], trans_cur[1], trans_cur[2]);

		for (int next = 0; next < scanners.size(); ++next) {
			if (visited[next]) {
				continue;
			}

			auto [has_overlap, vec, perm] = find_scanner_overlap(scanners[cur], scanners[next]);

			if (has_overlap) {
				std::cout << std::format("   scanner {} and {} overlap\n", cur, next);

				auto trans_next = transform_point(vec, perm_cur, trans_cur);
				auto perm_next = combine_perms(perm, perm_cur);

				translations[next] = trans_next;
				perms[next] = perm_next;

				queue.push(next);
			}
		}
	}

	int max_distance = -1;

	for (int i = 0; i < scanners.size(); ++i) {
		auto p1 = translations[i];

		for (int j = i + 1; j < scanners.size(); ++j) {
			auto p2 = translations[j];

			auto distance = std::abs(p1[0] - p2[0])
			              + std::abs(p1[1] - p2[1])
			              + std::abs(p1[2] - p2[2]);

			max_distance = std::max(max_distance, distance);
		}
	}

	std::cout << max_distance << '\n';
}
