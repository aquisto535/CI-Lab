#include "ImageStats.h"

#include <cmath>

namespace ImageCore {

	Histogram ComputeHistogram(const Image& image) {
		Histogram histogram{};
		for (auto value : image.pixels) {
			++histogram.bins[value];
		}
		return histogram;
	}

	double ComputeMean(const Image& image) {
		if (image.empty()) {
			return 0.0;
		}
		double sum = 0.0;
		for (auto value : image.pixels) {
			sum += static_cast<double>(value);
		}
		return sum / static_cast<double>(image.size());
	}

	double ComputeStdDev(const Image& image) {
		if (image.empty()) {
			return 0.0;
		}
		double mean = ComputeMean(image);
		double accum = 0.0;
		for (auto value : image.pixels) {
			double diff = static_cast<double>(value) - mean;
			accum += diff * diff;
		}
		return std::sqrt(accum / static_cast<double>(image.size()));
	}

}  // namespace ImageCore