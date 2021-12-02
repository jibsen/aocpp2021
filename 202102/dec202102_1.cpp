//
// Advent of Code 2021, day 2, part one
//

#include <iostream>
#include <string>

int main()
{
	std::string command;
	long delta = 0;
	long pos = 0;
	long depth = 0;

	while (std::cin >> command >> delta) {
		switch (command[0]) {
		case 'f':
			pos += delta;
			break;
		case 'd':
			depth += delta;
			break;
		case 'u':
			depth -= delta;
			break;
		default:
			std::cerr << "Unknown command: " << command << '\n';
			exit(1);
			break;
		}
	}

	std::cout << pos * depth << '\n';
}
