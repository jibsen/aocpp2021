//
// Advent of Code 2021, day 24
//

#include <algorithm>
#include <cctype>
#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

using State = std::array<char, 15>;

enum class Opcode { inp, add, mul, div, mod, eql };

enum class Register { w, x, y, z };

using Program = std::vector<std::tuple<Opcode, Register, std::pair<bool, int>>>;

auto split_ws(std::string_view sv)
{
	const auto is_ws = [](unsigned char ch) { return std::isspace(ch); };

	std::vector<std::string_view> res;

	auto pos = sv.begin();

	for (;;) {
		pos = std::find_if_not(pos, sv.end(), is_ws);

		if (pos == sv.end()) {
			break;
		}

		auto start = pos;

		pos = std::find_if(pos, sv.end(), is_ws);

		res.push_back(sv.substr(start - sv.begin(), pos - start));
	}

	return res;
}

Opcode get_opcode(std::string_view sv)
{
	const std::unordered_map<std::string_view, Opcode> opcode_lookup = {
		{"inp", Opcode::inp}, {"add", Opcode::add}, {"mul", Opcode::mul},
		{"div", Opcode::div}, {"mod", Opcode::mod}, {"eql", Opcode::eql}
	};

	return opcode_lookup.at(sv);
}

constexpr bool is_register(std::string_view sv)
{
	return sv[0] == 'w' || sv[0] == 'x' || sv[0] == 'y' || sv[0] == 'z';
}

int get_register(std::string_view sv)
{
	switch (sv[0]) {
	case 'w':
		return static_cast<int>(Register::w);
	case 'x':
		return static_cast<int>(Register::x);
	case 'y':
		return static_cast<int>(Register::y);
	case 'z':
		return static_cast<int>(Register::z);
	default:
		std::cerr << "invalid register\n";
		exit(1);
	}
}

auto read_program()
{
	Program program;

	for (std::string line; std::getline(std::cin, line); ) {
		auto tokens = split_ws(line);

		Opcode op = get_opcode(tokens[0]);
		Register r1 = Register(get_register(tokens[1]));

		if (op == Opcode::inp) {
			program.emplace_back(op, r1, std::make_pair(false, 0));

			continue;
		}

		int rv2 = is_register(tokens[2]) ? get_register(tokens[2])
			: std::stoi(std::string(tokens[2]));

		program.emplace_back(op, r1, std::make_pair(is_register(tokens[2]), rv2));
	}

	return program;
}

struct ALU {
	explicit ALU(Program program, std::vector<int> input)
		: program(std::move(program)), input(std::move(input)) {}

	bool step();
	void run();

	long long get_register(Register r) const { return registers[static_cast<int>(r)]; }

	void print_state() const;

private:
	std::vector<int> input;
	Program program;
	long long registers[4] = {};
	std::size_t ip = 0;
	std::size_t idx = 0;
};

void ALU::print_state() const
{
	std::cout << std::format("w={} x={} y={} z={}\n", registers[0], registers[1], registers[2], registers[3]);
}

bool ALU::step()
{
	if (ip >= program.size()) {
		return true;
	}

	auto [op, r1, rv2_pair] = program[ip++];

	auto [is_reg, rv2] = rv2_pair;

	switch (op) {
	case Opcode::inp:
		print_state();
		if (idx >= input.size()) {
			std::cerr << "out of input\n";
			exit(1);
		}
		registers[static_cast<int>(r1)] = input[idx++];
		break;
	case Opcode::add:
		registers[static_cast<int>(r1)] += is_reg ? registers[rv2] : rv2;
		break;
	case Opcode::mul:
		registers[static_cast<int>(r1)] *= is_reg ? registers[rv2] : rv2;
		break;
	case Opcode::div:
		registers[static_cast<int>(r1)] /= is_reg ? registers[rv2] : rv2;
		break;
	case Opcode::mod:
		registers[static_cast<int>(r1)] %= is_reg ? registers[rv2] : rv2;
		break;
	case Opcode::eql:
		registers[static_cast<int>(r1)] = static_cast<int>(
			registers[static_cast<int>(r1)] == (is_reg ? registers[rv2] : rv2)
		);
		break;
	}

	return false;
}

void ALU::run()
{
	while (!step()) {
		// nothing
	}
}

int main()
{
	auto program = read_program();

	std::cout << std::format("{} lines of program read\n", program.size());

	std::vector<int> input = { 9,2,9,6,9,5,9,3,4,9,7,9,9,2 };

	ALU alu(program, input);

	alu.run();

	std::cout << alu.get_register(Register::z) << '\n';
}
