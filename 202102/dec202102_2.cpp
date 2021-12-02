//
// Advent of Code 2021, day 2, part two
//

#include <iostream>
#include <string>

int main()
{
	std::string command;
	long delta = 0;
	long pos = 0;
	long depth = 0;
	long aim = 0;

	while (std::cin >> command >> delta) {
		switch (command[0]) {
		case 'f':
			pos += delta;
			depth += aim * delta;
			break;
		case 'd':
			aim += delta;
			break;
		case 'u':
			aim -= delta;
			break;
		default:
			std::cerr << "Unknown command: " << command << '\n';
			exit(1);
			break;
		}
	}

	std::cout << pos * depth << '\n';
}
