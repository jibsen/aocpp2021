//
// Advent of Code 2021, day 8, part one
//

#include <iostream>
#include <sstream>
#include <string>
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
			patterns.emplace_back(word);
		}

		for (std::string word; ss >> word; ) {
			output.emplace_back(word);
		}

		notes.emplace_back(patterns, output);
	}

	return notes;
}

int main()
{
	auto notes = read_notes();

	std::size_t num_unique = 0;

	for (auto &[patterns, output] : notes) {
		for (auto &word : output) {
			num_unique += word.size() == 2
			           || word.size() == 3
				   || word.size() == 4
				   || word.size() == 7;
		}
	}

	std::cout << num_unique << '\n';
}
