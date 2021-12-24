//
// Advent of Code 2021, day 24, part two
//

#include <algorithm>
#include <format>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

long long simulate_monad(const std::vector<int> &input)
{
	long long z = 0;

	z = z * 26 + input[0] + 1;
	z = z * 26 + input[1] + 11;
	z = z * 26 + input[2] + 1;
	z = z * 26 + input[3] + 11;
	z = (input[4] != (z % 26) - 8) ? (z / 26) * 26 + input[4] + 2 : z / 26;
	z = (input[5] != (z % 26) - 5) ? (z / 26) * 26 + input[5] + 9 : z / 26;
	z = z * 26 + input[6] + 7;
	z = (input[7] != (z % 26) - 13) ? (z / 26) * 26 + input[7] + 11 : z / 26;
	z = z * 26 + input[8] + 6;
	z = (input[9] != (z % 26) - 1) ? (z / 26) * 26 + input[9] + 15 : z / 26;
	z = z * 26 + input[10] + 7;
	z = (input[11] != (z % 26) - 5) ? (z / 26) * 26 + input[11] + 1 : z / 26;
	z = (input[12] != (z % 26) - 4) ? (z / 26) * 26 + input[12] + 8 : z / 26;
	z = (input[13] != (z % 26) - 8) ? (z / 26) * 26 + input[13] + 6 : z / 26;

	return z;
}

bool inc(std::vector<int> &v)
{
	if (v.back() < 9) {
		++v.back();
		return true;
	}

	for (auto i = std::ssize(v) - 1; i >= 0; --i) {
		if (v[i] < 9) {
			++v[i];
			return true;
		}
		else {
			v[i] = 1;
		}
	}

	return false;
}

std::vector<int> find_part_4(long long start_z)
{
	std::vector<int> in = { 1, 1, 1, 1 };

	for (;;) {
		long long z = start_z;

		z = z * 26 + in[0] + 7;

		if (in[1] != (z % 26) - 5) {
			if (!inc(in)) {
				return {};
			}
			continue;
		}

		z = z / 26;

		if (in[2] != (z % 26) - 4) {
			if (!inc(in)) {
				return {};
			}
			continue;
		}

		z = z / 26;

		if (in[3] != (z % 26) - 8) {
			if (!inc(in)) {
				return {};
			}
			continue;
		}

		z = z / 26;

		if (z != 0) {
			if (!inc(in)) {
				return {};
			}
			continue;
		}

		return in;
	}
}

std::vector<int> find_part_3(long long start_z)
{
	std::vector<int> in = { 1, 1 };

	for (;;) {
		long long z = start_z;

		z = z * 26 + in[0] + 6;

		if (in[1] != (z % 26) - 1) {
			if (!inc(in)) {
				return {};
			}
			continue;
		}

		z = z / 26;

		auto next_part = find_part_4(z);

		if (next_part.empty()) {
			if (!inc(in)) {
				return {};
			}
			continue;
		}

		in.insert(in.end(), next_part.begin(), next_part.end());

		return in;
	}
}

std::vector<int> find_part_2(long long start_z)
{
	std::vector<int> in = { 1, 1 };

	for (;;) {
		long long z = start_z;

		z = z * 26 + in[0] + 7;

		if (in[1] != (z % 26) - 13) {
			if (!inc(in)) {
				return {};
			}
			continue;
		}

		z = z / 26;

		auto next_part = find_part_3(z);

		if (next_part.empty()) {
			if (!inc(in)) {
				return {};
			}
			continue;
		}

		in.insert(in.end(), next_part.begin(), next_part.end());

		return in;
	}
}

std::vector<int> find_solution()
{
	std::vector<int> in = { 1, 1, 1, 1, 1, 1 };

	for (;;) {
		long long z = 0;

		z = z * 26 + in[0] + 1;
		z = z * 26 + in[1] + 11;
		z = z * 26 + in[2] + 1;
		z = z * 26 + in[3] + 11;

		if (in[4] != (z % 26) - 8) {
			if (!inc(in)) {
				return {};
			}
			continue;
		}

		z = z / 26;

		if (in[5] != (z % 26) - 5) {
			inc(in);
			continue;
		}

		z = z / 26;

		auto next_part = find_part_2(z);

		if (next_part.empty()) {
			if (!inc(in)) {
				return {};
			}
			continue;
		}

		in.insert(in.end(), next_part.begin(), next_part.end());

		return in;
	}
}

int main()
{
	auto solution = find_solution();

	if (!solution.empty()) {
		std::ranges::copy(solution, std::ostream_iterator<int>(std::cout));
		std::cout << '\n';

		std::cout << std::format("MONAD output: {}\n", simulate_monad(solution));
	}
}
