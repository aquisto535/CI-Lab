#include "pch.h"
#include "ImageIO.h"
#include <filesystem>
#include <algorithm>  // std::clamp도 함께 쓰고 있으니 같이 포함 권장
#include <fstream>
#include <sstream>
#include <string>

namespace ImageCore {

	namespace {

		void ExpectMagic(const std::string& magic) {
			if (magic != "P2") {
				throw std::runtime_error("Only ASCII PGM (P2) files are supported for this lab");
			}
		}

	}  // namespace

	Image LoadPGM(const std::filesystem::path& path) {
		std::ifstream input(path);
		if (!input) {
			throw std::runtime_error("Failed to open file: " + path.string());
		}

		std::string magic;
		input >> magic;
		ExpectMagic(magic);

		int width = 0;
		int height = 0;
		int maxVal = 0;
		input >> width >> height >> maxVal;
		if (width <= 0 || height <= 0 || maxVal <= 0) {
			throw std::runtime_error("Invalid PGM header");
		}

		Image image(width, height, 0);
		for (int i = 0; i < width * height; ++i) {
			int value = 0;
			input >> value;
			image.pixels[static_cast<std::size_t>(i)] = static_cast<uint8_t>(std::clamp(value, 0, 255));
		}
		return image;
	}

	void SavePGM(const Image & image, const std::filesystem::path & path) {
		std::ofstream output(path);
		if (!output) {
			throw std::runtime_error("Failed to create file: " + path.string());
		}
		output << "P2\n" << image.width << " " << image.height << "\n255\n";
		for (int y = 0; y < image.height; ++y) {
			for (int x = 0; x < image.width; ++x) {
				output << static_cast<int>(image.at(x, y)) << ' ';
			}
			output << '\n';
		}
	}

}  // namespace ImageCore