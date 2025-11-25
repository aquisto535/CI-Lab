#include "ImageFilter.h"

#include <algorithm>
#include <numeric>
#include <vector>

namespace ImageCore {

	namespace {

		Image ApplyKernel(const Image& src, const std::vector<float>& kernel, int radius) {
			Image dst(src.width, src.height, 0);
			for (int y = 0; y < src.height; ++y) {
				for (int x = 0; x < src.width; ++x) {
					float accum = 0.0f;
					float weightSum = 0.0f;
					for (int ky = -radius; ky <= radius; ++ky) {
						for (int kx = -radius; kx <= radius; ++kx) {
							int sx = std::clamp(x + kx, 0, src.width - 1);
							int sy = std::clamp(y + ky, 0, src.height - 1);
							auto weight = kernel[(ky + radius) * (2 * radius + 1) + (kx + radius)];
							accum += static_cast<float>(src.at(sx, sy)) * weight;
							weightSum += weight;
						}
					}
					auto value = static_cast<int>(accum / std::max(0.001f, weightSum));
					dst.at(x, y) = static_cast<uint8_t>(std::clamp(value, 0, 255));
				}
			}
			return dst;
		}

	}  // namespace

	Image ApplyBoxBlur(const Image & src, int radius) {
		if (radius <= 0) {
			return src;
		}
		int diameter = radius * 2 + 1;
		float weight = 1.0f / static_cast<float>(diameter * diameter);
		std::vector<float> kernel(static_cast<std::size_t>(diameter * diameter), weight);
		return ApplyKernel(src, kernel, radius);
	}

	Image ApplyUnsharpMask(const Image & src, float amount) {
		amount = std::clamp(amount, 0.0f, 2.0f);
		auto blurred = ApplyBoxBlur(src, 1);
		Image dst(src.width, src.height, 0);
		for (int y = 0; y < src.height; ++y) {
			for (int x = 0; x < src.width; ++x) {
				int original = src.at(x, y);
				int blur = blurred.at(x, y);
				int sharpened = static_cast<int>(original + amount * (original - blur));
				dst.at(x, y) = static_cast<uint8_t>(std::clamp(sharpened, 0, 255));
			}
		}
		return dst;
	}

	Image NormalizeContrast(const Image & src) {
		auto minmax = std::minmax_element(src.pixels.begin(), src.pixels.end());
		if (minmax.first == minmax.second || *minmax.first == *minmax.second) {
			return src;
		}

		Image dst(src.width, src.height, 0);
		double minVal = *minmax.first;
		double maxVal = *minmax.second;
		double range = maxVal - minVal;
		for (int i = 0; i < src.width * src.height; ++i) {
			double normalized = (static_cast<double>(src.pixels[static_cast<std::size_t>(i)]) - minVal) / range;
			dst.pixels[static_cast<std::size_t>(i)] = static_cast<uint8_t>(std::clamp(static_cast<int>(normalized * 255.0), 0, 255));
		}
		return dst;
	}

}  // namespace ImageCore