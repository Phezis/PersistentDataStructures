#pragma once
#include <cstddef>

namespace PersistentContainer {
	namespace Utils {
		constexpr std::size_t binPow(std::uint32_t deg) {
			return static_cast<std::size_t>(1) << deg;
		}
	}
}