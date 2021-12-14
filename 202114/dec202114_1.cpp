//
// Advent of Code 2021, day 14, part one
//

#include <algorithm>
#include <format>
#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

auto read_rules()
{
	std::unordered_map<std::string, std::pair<std::string, std::string>> rules;

	for (std::string pair, arrow, insert; std::cin >> pair >> arrow >> insert; ) {
		std::string left_product = pair.substr(0, 1) + insert;
		std::string right_product = insert + pair.substr(1, 1);

		rules.try_emplace(pair, std::move(left_product), std::move(right_product));
	}

	return rules;
}

int main()
{
	std::string polymer_template;

	std::getline(std::cin, polymer_template);

	auto rules = read_rules();

	std::cout << std::format("template is '{}'\n", polymer_template);
	std::cout << std::format("{} rules read\n", rules.size());

	std::unordered_map<char, std::size_t> element_counts;

	for (auto element : polymer_template) {
		element_counts[element]++;
	}

	std::unordered_map<std::string, std::size_t> pair_counts;

	for (int i = 0; i < polymer_template.size() - 1; ++i) {
		pair_counts[polymer_template.substr(i, 2)]++;
	}

	for (int step = 0; step < 10; ++step) {
		auto next_pair_counts = pair_counts;

		for (const auto &[pair, count] : pair_counts) {
			if (count == 0) {
				continue;
			}

			if (auto it = rules.find(pair); it != rules.end()) {
				const auto &[left, right] = it->second;

				element_counts[right[0]] += count;

				next_pair_counts[pair] -= count;
				next_pair_counts[left] += count;
				next_pair_counts[right] += count;
			}
		}

		pair_counts.swap(next_pair_counts);
	}

	auto [min, max] = std::ranges::minmax_element(element_counts | std::views::values);

	std::cout << *max - *min << '\n';
}
