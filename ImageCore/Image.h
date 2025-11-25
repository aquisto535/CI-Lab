#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace ImageCore {

	struct Image {
		int width{};
		int height{};
		std::vector<uint8_t> pixels;

		Image() = default;
		Image(int w, int h, uint8_t value = 0);

		[[nodiscard]] std::size_t size() const noexcept { return pixels.size(); }

		uint8_t& at(int x, int y);
		const uint8_t& at(int x, int y) const;

		[[nodiscard]] bool empty() const noexcept { return pixels.empty(); }
	};

	Image CreateDemoGradient(int width, int height);

}  // namespace ImageCore
