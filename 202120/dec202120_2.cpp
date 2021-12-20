//
// Advent of Code 2021, day 20, part two
//

#include <algorithm>
#include <array>
#include <deque>
#include <format>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>

using Image = std::deque<std::deque<int>>;

auto read_image()
{
	Image image;

	for (std::string line; std::cin >> line; ) {
		std::deque<int> row;

		std::ranges::transform(line, std::back_inserter(row),
		                       [](auto ch) { return static_cast<int>(ch == '#'); });

		image.push_back(std::move(row));
	}

	return image;
}

void expand_image(Image &img, int pixel)
{
	for (auto &row : img) {
		row.push_front(pixel);
		row.push_back(pixel);
	}

	img.emplace_front(img[0].size(), pixel);
	img.emplace_back(img[0].size(), pixel);
}

std::size_t count_lit_pixels(const Image &img)
{
	std::size_t num = 0;

	for (const auto &row : img) {
		num += std::accumulate(row.begin(), row.end(), 0ULL);
	}

	return num;
}

int main()
{
	std::string iea;

	std::getline(std::cin, iea);

	auto image = read_image();

	std::cout << std::format("iea has {} entries\n", iea.size());
	std::cout << std::format("image is {}x{} pixels\n", image[0].size(), image.size());

	expand_image(image, 0);

	for (int step = 0; step < 50; ++step) {
		expand_image(image, image[0][0]);

		Image next_image = image;

		for (int y = 0; y < image.size(); ++y) {
			for (int x = 0; x < image[y].size(); ++x) {
				constexpr std::array<std::array<int, 2>, 9> deltas = {{
					{-1, -1}, {0, -1}, {1, -1},
					{-1, 0}, {0, 0}, {1, 0},
					{-1, 1}, {0, 1}, {1, 1}
				}};

				unsigned int index = 0;

				for (auto [dx, dy] : deltas) {
					if (x + dx < 0 || x + dx >= image[y].size()
					 || y + dy < 0 || y + dy >= image.size()) {
						index = image[0][0] ? 511 : 0;
						break;
					}

					index = (index << 1) + static_cast<unsigned int>(image[y + dy][x + dx]);
				}

				next_image[y][x] = static_cast<int>(iea[index] == '#');
			}
		}

		image.swap(next_image);
	}

	std::cout << count_lit_pixels(image) << '\n';
}
