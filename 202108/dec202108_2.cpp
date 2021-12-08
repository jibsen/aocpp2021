//
// Advent of Code 2021, day 8, part two
//

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

auto read_notes()
{
	std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> notes;

	for (std::string line; std::getline(std::cin, line); ) {
		std::vector<std::string> patterns;
		std::vector<std::string> output;

		std::istringstream ss(line);

		for (std::string word; ss >> word && word != "|"; ) {
			std::ranges::sort(word);
			patterns.emplace_back(word);
		}

		for (std::string word; ss >> word; ) {
			std::ranges::sort(word);
			output.emplace_back(word);
		}

		notes.emplace_back(patterns, output);
	}

	return notes;
}

constexpr bool contains_all_of(std::string_view word, std::string_view chars)
{
	return std::ranges::all_of(chars,
		[&](auto ch) {
			return word.find(ch) != std::string_view::npos;
		}
	);
}

auto analyze(const std::vector<std::string> &patterns)
{
	std::unordered_map<std::string, int> map;

	std::vector<std::string_view> known(10);

	// The one of length 2 is 1
	// The one of length 3 is 7
	// The one of length 4 is 4
	// The one of length 7 is 8
	for (auto &pattern : patterns) {
		switch (pattern.size()) {
		case 2:
			known[1] = pattern;
			map[pattern] = 1;
			break;
		case 3:
			known[7] = pattern;
			map[pattern] = 7;
			break;
		case 4:
			known[4] = pattern;
			map[pattern] = 4;
			break;
		case 7:
			known[8] = pattern;
			map[pattern] = 8;
			break;
		}
	}

	std::string bd;

	// 4 \ 1 = bd
	std::ranges::set_difference(known[4], known[1], std::back_inserter(bd));

	// The one of length 5 that contains bd is 5
	// The one of length 6 that does not contain bd is 0
	for (auto &pattern : patterns) {
		if (pattern.size() == 5 && contains_all_of(pattern, bd)) {
			known[5] = pattern;
			map[pattern] = 5;
		}
		if (pattern.size() == 6 && !contains_all_of(pattern, bd)) {
			known[0] = pattern;
			map[pattern] = 0;
		}
	}

	std::string aeg;

	// 8 \ 4 = aeg
	std::ranges::set_difference(known[8], known[4], std::back_inserter(aeg));

	// The one of length 5 that contains aeg is 2
	// The one of length 6 that is not 0 that contains aeg is 6
	for (auto &pattern : patterns) {
		if (pattern.size() == 5 && contains_all_of(pattern, aeg)) {
			known[2] = pattern;
			map[pattern] = 2;
		}
		if (pattern.size() == 6 && pattern != known[0] && contains_all_of(pattern, aeg)) {
			known[6] = pattern;
			map[pattern] = 6;
		}
	}

	// The last one of length 5 is 3
	// The last one of length 6 is 9
	for (auto &pattern : patterns) {
		if (pattern.size() == 5 && map.find(pattern) == map.end()) {
			known[3] = pattern;
			map[pattern] = 3;
		}
		if (pattern.size() == 6 && map.find(pattern) == map.end()) {
			known[9] = pattern;
			map[pattern] = 9;
		}
	}

	return map;
}

int main()
{
	auto notes = read_notes();

	long sum = 0;

	for (auto &[patterns, output] : notes) {
		auto map = analyze(patterns);

		long value = 0;

		for (auto &word : output) {
			value = value * 10 + map[word];
		}

		sum += value;
	}

	std::cout << sum << '\n';
}
