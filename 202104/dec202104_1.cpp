//
// Advent of Code 2021, day 4, part one
//

#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <format>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

struct Board {
	std::array<std::array<int, 5>, 5> board = {};

	void mark(int n);
	bool wins() const;
	int score(int n) const;
};

void Board::mark(int n)
{
	for (auto &row : board) {
		if (auto it = std::ranges::find(row, n); it != row.end()) {
			*it = -1;
			return;
		}
	}
}

bool Board::wins() const
{
	auto is_marked = [](int n) { return n == -1; };

	for (auto &row : board) {
		if (std::ranges::all_of(row, is_marked)) {
			return true;
		}
	}

	for (int col = 0; col < board[0].size(); ++col) {
		int num_marked = 0;

		for (auto &row : board) {
			num_marked += static_cast<int>(row[col] == -1);
		}

		if (num_marked == board.size()) {
			return true;
		}
	}

	return false;
}

int Board::score(int n) const
{
	int score = 0;

	for (auto &row : board) {
		for (auto &cell : row) {
			score += cell == -1 ? 0 : cell;
		}
	}

	return n * score;
}

auto read_numbers_drawn()
{
	std::vector<int> numbers;

	std::string line;

	std::getline(std::cin, line);
	std::istringstream ss(line);

	int number = 0;

	for (char sep = ','; sep == ',' && ss >> number; ss >> sep) {
		numbers.push_back(number);
	}

	return numbers;
}

auto read_boards()
{
	std::vector<Board> boards;

	for (std::istream_iterator<int> it(std::cin), end; it != end; ) {
		Board board;

		for (auto &row : board.board) {
			for (auto &cell : row) {
				cell = *it++;
			}
		}

		boards.push_back(board);
	}

	return boards;
}

int main()
{
	auto numbers = read_numbers_drawn();
	auto boards = read_boards();

	std::cout << std::format("{} numbers read\n", numbers.size());
	std::cout << std::format("{} boards read\n", boards.size());

	for (auto number : numbers) {
		for (auto &board : boards) {
			board.mark(number);

			if (board.wins()) {
				std::cout << board.score(number) << '\n';
				return 0;
			}
		}
	}
}
