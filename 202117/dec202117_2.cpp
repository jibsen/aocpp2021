//
// Advent of Code 2021, day 17, part two
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

	while (x <= target_x.second && y >= target_y.first
	    && (x_vel > 0 || x >= target_x.first)) {

		if (x >= target_x.first && x <= target_x.second
		 && y >= target_y.first && y <= target_y.second) {
			return true;
		}

		x += x_vel;
		y += y_vel;

		x_vel -= static_cast<int>(x_vel > 0);
		--y_vel;
	}

	return false;
}

int main()
{
	std::size_t num_good = 0;

	// The upper limit of 200 for y velocity works for the given target
	// areas, but not in general
	for (int x_vel = 1; x_vel <= target_x.second; ++x_vel) {
		for (int y_vel = target_y.first; y_vel <= 200; ++y_vel) {
			num_good += static_cast<std::size_t>(simulate_launch(x_vel, y_vel));
		}
	}

	std::cout << num_good << '\n';
}
