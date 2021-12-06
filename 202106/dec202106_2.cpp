//
// Advent of Code 2021, day 6, part two
//

#include <array>
#include <format>
#include <iostream>
#include <numeric>
#include <vector>

auto read_lanternfish()
{
	std::vector<int> fish;
	int age = 0;

	for (char sep = ','; sep == ',' && std::cin >> age; std::cin >> sep) {
		fish.push_back(age);
	}

	return fish;
}

constexpr std::size_t simulate_fish(int initial_age, int days)
{
	std::array<std::size_t, 10> fish_per_age = {};

	fish_per_age[initial_age] = 1;

	for (int day = 0; day < days; ++day) {
		fish_per_age[7] += fish_per_age[0];
		fish_per_age[9] += fish_per_age[0];

		for (int age = 1; age < fish_per_age.size(); ++age) {
			fish_per_age[age - 1] = fish_per_age[age];
		}

		fish_per_age[9] = 0;
	}

	return std::accumulate(fish_per_age.begin(), fish_per_age.end(), 0ULL);
}

int main()
{
	constexpr auto expansion_by_age = [] {
		std::array<std::size_t, 6> res = {};

		for (int age = 0; age < res.size(); ++age) {
			res[age] = simulate_fish(age, 256);
		}

		return res;
	}();

	auto fish = read_lanternfish();

	std::cout << std::format("{} lanternfish read\n", fish.size());

	std::cout << std::accumulate(fish.begin(), fish.end(), 0ULL, [&](std::size_t sum, int age) {
		return sum + expansion_by_age[age];
	});
}
