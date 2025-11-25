#pragma once

#include "Image.h"

#include <array>
#include <cstdint>

namespace ImageCore {

	struct Histogram {
		std::array<std::uint32_t, 256> bins{};
	};

	Histogram ComputeHistogram(const Image& image);
	double ComputeMean(const Image& image);
	double ComputeStdDev(const Image& image);

}  // namespace ImageCore
