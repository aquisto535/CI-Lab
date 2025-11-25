#include <gtest/gtest.h>

#include "../ImageCore/ImageFilter.h"
#include "../ImageCore/ImageIO.h"
#include "../ImageCore/ImageStats.h"

using namespace ImageCore;

TEST(ImageCoreTests, GradientGenerationHasExpectedBounds) {
	auto gradient = CreateDemoGradient(64, 4);
	EXPECT_EQ(gradient.width, 64);
	EXPECT_EQ(gradient.height, 4);
	EXPECT_EQ(gradient.at(0, 0), 0);
	EXPECT_EQ(gradient.at(63, 0), 255);
}

TEST(ImageCoreTests, BoxBlurSmoothsHighFrequencyNoise) {
	Image noisy(8, 1, 0);
	for (int x = 0; x < noisy.width; ++x) {
		noisy.at(x, 0) = static_cast<uint8_t>((x % 2) ? 255 : 0);
	}
	auto blurred = ApplyBoxBlur(noisy, 1);
	EXPECT_LT(blurred.at(1, 0), 200);
	EXPECT_GT(blurred.at(1, 0), 50);
}

TEST(ImageCoreTests, HistogramHas256Bins) {
	auto gradient = CreateDemoGradient(16, 16);
	auto histogram = ComputeHistogram(gradient);
	EXPECT_EQ(histogram.bins.size(), 256);
	std::uint32_t sum = 0;
	for (auto value : histogram.bins) {
		sum += value;
	}
	EXPECT_EQ(sum, gradient.size());
}