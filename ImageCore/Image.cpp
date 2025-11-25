#include "Image.h"

#include <algorithm>

namespace ImageCore {

	Image::Image(int w, int h, uint8_t value) : width(w), height(h), pixels(static_cast<std::size_t>(w* h), value) {
		if (w <= 0 || h <= 0) {
			throw std::invalid_argument("Image dimensions must be positive");
		}
	}

	uint8_t& Image::at(int x, int y) {
		if (x < 0 || y < 0 || x >= width || y >= height) {
			throw std::out_of_range("Pixel coordinate out of range");
		}
		return pixels[static_cast<std::size_t>(y) * width + x];
	}

	const uint8_t& Image::at(int x, int y) const {
		if (x < 0 || y < 0 || x >= width || y >= height) {
			throw std::out_of_range("Pixel coordinate out of range");
		}
		return pixels[static_cast<std::size_t>(y) * width + x];
	}

	Image CreateDemoGradient(int width, int height) {
		Image image(width, height, 0);
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				auto value = static_cast<uint8_t>((x * 255) / std::max(1, width - 1));
				image.at(x, y) = value;
			}
		}
		return image;
	}

}  // namespace ImageCore