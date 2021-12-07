#pragma once
#include <cstddef>

namespace pds {
	namespace Utils {
		constexpr std::size_t binPow(std::uint32_t deg) {
			return static_cast<std::size_t>(1) << deg;
		}

		std::size_t getId(std::size_t pos, std::uint32_t level, std::uint32_t degreeOfTwo) {
			return pos >> (level * degreeOfTwo);
		}

		std::size_t getMask(std::size_t pos, std::uint32_t level, std::uint32_t degreeOfTwo) {
			return (1 << (degreeOfTwo - 1)) << ((level - 1) * degreeOfTwo);
		}
	}
}