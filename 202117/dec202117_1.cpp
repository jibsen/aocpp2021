//
// Advent of Code 2021, day 17, part one
//

#include <algorithm>
#include <iostream>
#include <limits>

// constexpr auto target_x = std::make_pair(20, 30);
// constexpr auto target_y = std::make_pair(-10, -5);
constexpr auto target_x = std::make_pair(56, 76);
constexpr auto target_y = std::make_pair(-162, -134);

constexpr auto simulate_launch(int x_vel, int y_vel)
{
	int x = 0;
	int y = 0;

	int max_y = std::numeric_limits<int>::min();

	while (x <= target_x.second && y >= target_y.first
	    && (x_vel > 0 || x >= target_x.first)) {

		max_y = std::max(max_y, y);

		if (x >= target_x.first && x <= target_x.second
		 && y >= target_y.first && y <= target_y.second) {
			return max_y;
		}

		x += x_vel;
		y += y_vel;

		x_vel -= static_cast<int>(x_vel > 0);
		--y_vel;
	}

	return std::numeric_limits<int>::min();
}

int main()
{
	int max_y = std::numeric_limits<int>::min();

	// The upper limit of 200 for y velocity works for the given target
	// areas, but not in general
	for (int x_vel = 1; x_vel <= target_x.second; ++x_vel) {
		for (int y_vel = target_y.first; y_vel <= 200; ++y_vel) {
			max_y = std::max(max_y, simulate_launch(x_vel, y_vel));
		}
	}

	std::cout << max_y << '\n';
}
