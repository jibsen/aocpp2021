//
// Advent of Code 2021, day 16, part two
//

#include <algorithm>
#include <format>
#include <iostream>
#include <numeric>
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

std::uint64_t parse_packet(BitReader &br);

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

auto parse_subpackets(BitReader &br)
{
	std::vector<std::uint64_t> sub_values;

	auto length_type_ID = br.get_bit();

	if (length_type_ID == 0) {
		std::size_t sub_length = br.get_bits(15);

		auto start_bit_pos = br.bit_position();

		while (br.bit_position() < start_bit_pos + sub_length) {
			sub_values.push_back(parse_packet(br));
		}
	}
	else {
		int num_sub = static_cast<int>(br.get_bits(11));

		sub_values.reserve(num_sub);

		for (int i = 0; i < num_sub; ++i) {
			sub_values.push_back(parse_packet(br));
		}
	}

	return sub_values;
}

enum class Type {
	sum, product, minimum, maximum, literal, greater, less, equal
};

std::uint64_t parse_packet(BitReader &br)
{
	auto version = br.get_bits(3);
	auto type_id = Type(br.get_bits(3));

	if (type_id == Type::literal) {
		return parse_literal(br);
	}

	const auto sub_values = parse_subpackets(br);

	switch (type_id) {
	case Type::sum:
		return std::accumulate(sub_values.begin(), sub_values.end(), 0ULL);
	case Type::product:
		return std::accumulate(sub_values.begin(), sub_values.end(), 1ULL, std::multiplies{});
	case Type::minimum:
		return std::ranges::min(sub_values);
	case Type::maximum:
		return std::ranges::max(sub_values);
	case Type::greater:
		return static_cast<std::uint64_t>(sub_values[0] > sub_values[1]);
	case Type::less:
		return static_cast<std::uint64_t>(sub_values[0] < sub_values[1]);
	case Type::equal:
		return static_cast<std::uint64_t>(sub_values[0] == sub_values[1]);
	}

	return 0;
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
