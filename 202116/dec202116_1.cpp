//
// Advent of Code 2021, day 16, part one
//

#include <format>
#include <iostream>
#include <string>
#include <vector>

struct BitReader {
	explicit BitReader(std::string data) : data(std::move(data)) {}

	std::uint32_t get_bit();
	std::uint32_t get_bits(int num);

	std::size_t bit_position() const { return pos * 4 - (32 - lsb); }

private:
	std::string data;
	std::size_t pos = 0;

	std::uint32_t tag = 0;
	int lsb = 32;

	void refill(int num);
};

void BitReader::refill(int num)
{
	// Read nibbles until at least num bits available
	while (lsb > 32 - num) {
		lsb -= 4;

		if (pos != data.size()) {
			std::uint32_t nibble = static_cast<std::uint32_t>(std::stoi(data.substr(pos++, 1), nullptr, 16));
			tag |= nibble << lsb;
		}
	}
}

std::uint32_t BitReader::get_bit()
{
	if (lsb == 32) {
		refill(4);
	}

	// Get bit from tag
	std::uint32_t bit = tag >> 31;

	// Remove bit from tag
	tag <<= 1;
	lsb++;

	return bit;
}

std::uint32_t BitReader::get_bits(int num)
{
	refill(num);

	// Get bits from tag
	std::uint32_t bits = (tag >> 1) >> (31 - num);

	// Remove bits from tag
	tag <<= num;
	lsb += num;

	return bits;
}

std::size_t parse_packet(BitReader &br);

std::uint64_t parse_literal(BitReader &br)
{
	std::uint64_t value = 0;
	bool more_groups = false;

	do {
		more_groups = static_cast<bool>(br.get_bit());

		value = (value << 4) | br.get_bits(4);
	} while (more_groups);

	return value;
}

void parse_operator(BitReader &br)
{
	auto length_type_ID = br.get_bit();

	if (length_type_ID == 0) {
		std::size_t sub_length = br.get_bits(15);

		auto start_bit_pos = br.bit_position();

		while (br.bit_position() < start_bit_pos + sub_length) {
			parse_packet(br);
		}
	}
	else {
		int num_sub = static_cast<int>(br.get_bits(11));

		for (int i = 0; i < num_sub; ++i) {
			parse_packet(br);
		}
	}
}

enum class Type {
	op0, op1, op2, op3, literal, op5, op6, op7
};

std::size_t parse_packet(BitReader &br)
{
	static std::size_t version_sum = 0;

	auto version = br.get_bits(3);
	auto type_id = Type(br.get_bits(3));

	version_sum += version;

	switch (type_id) {
	case Type::literal:
		parse_literal(br);
		break;
	case Type::op0:
		[[fallthrough]];
	case Type::op1:
		[[fallthrough]];
	case Type::op2:
		[[fallthrough]];
	case Type::op3:
		[[fallthrough]];
	case Type::op5:
		[[fallthrough]];
	case Type::op6:
		[[fallthrough]];
	case Type::op7:
		parse_operator(br);
		break;
	}

	return version_sum;
}

int main()
{
	std::string transmission;

	std::getline(std::cin, transmission);

	if (transmission.ends_with('.')) {
		transmission.pop_back();
	}

	BitReader br(transmission);

	std::cout << parse_packet(br) << '\n';
}
