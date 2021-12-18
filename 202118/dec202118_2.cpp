//
// Advent of Code 2021, day 18, part two
//

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

constexpr int open_par = -1;
constexpr int close_par = -2;

using Number = std::vector<int>;

auto read_number(std::string_view sv)
{
	Number res;

	for (auto ch : sv) {
		switch (ch) {
		case '[':
			res.push_back(open_par);
			break;
		case ']':
			res.push_back(close_par);
			break;
		case ',':
			break;
		default:
			res.push_back(ch - '0');
			break;
		}
	}

	return res;
}

void print_number(const Number &num)
{
	for (int n : num) {
		switch(n) {
		case open_par:
			std::cout << '[';
			break;
		case close_par:
			std::cout << ']';
			break;
		default:
			std::cout << n << ',';
		}
	}
	std::cout << '\n';
}

bool explode_number(Number &num)
{
	int nesting = 0;

	for (std::size_t i = 0; i < num.size(); ++i) {
		switch (num[i]) {
		case open_par:
			++nesting;
			break;
		case close_par:
			--nesting;
			break;
		default:
			if (nesting >= 5) {
				for (std::size_t j = i - 1; j > 0; --j) {
					if (num[j] >= 0) {
						num[j] += num[i];
						break;
					}
				}

				for (std::size_t j = i + 2; j < num.size(); ++j) {
					if (num[j] >= 0) {
						num[j] += num[i + 1];
						break;
					}
				}

				num[i - 1] = 0;

				num.erase(num.begin() + i, num.begin() + i + 3);

				return true;
			}
		}
	}

	return false;
}

bool split_number(Number &num)
{
	for (std::size_t i = 0; i < num.size(); ++i) {
		if (num[i] > 9) {
			num.insert(num.begin() + i + 1, {num[i] / 2, (num[i] + 1) / 2, close_par});
			num[i] = open_par;
			return true;
		}
	}

	return false;
}

void reduce_number(Number &num)
{
	while (explode_number(num) || split_number(num)) {
		// nothing
	}
}

Number add_numbers(const Number &lhs, const Number &rhs)
{
	Number res;

	res.reserve(lhs.size() + rhs.size() + 2);

	res.push_back(open_par);
	res.insert(res.end(), lhs.begin(), lhs.end());
	res.insert(res.end(), rhs.begin(), rhs.end());
	res.push_back(close_par);

	reduce_number(res);

	return res;
}

std::size_t magnitude_internal(const Number &num, int &pos)
{
	if (num[pos] >= 0) {
		return static_cast<std::size_t>(num[pos++]);
	}

	++pos;

	std::size_t left_mag = magnitude_internal(num, pos);
	std::size_t right_mag = magnitude_internal(num, pos);

	++pos;

	return 3 * left_mag + 2 * right_mag;
}

std::size_t magnitude(const Number &num)
{
	int pos = 0;
	return magnitude_internal(num, pos);
}

int main()
{
	std::vector<Number> numbers;

	for (std::string line; std::getline(std::cin, line); ) {
		numbers.push_back(read_number(line));
	}

	std::size_t largest_mag = 0;

	for (int i = 0; i < numbers.size(); ++i) {
		for (int j = i + 1; j < numbers.size(); ++j) {
			largest_mag = std::max(largest_mag, magnitude(add_numbers(numbers[i], numbers[j])));
			largest_mag = std::max(largest_mag, magnitude(add_numbers(numbers[j], numbers[i])));
		}
	}

	std::cout << largest_mag << '\n';
}
