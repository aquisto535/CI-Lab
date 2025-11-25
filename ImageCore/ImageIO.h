#pragma once

#include "Image.h"

#include <filesystem>

namespace ImageCore {

	Image LoadPGM(const std::filesystem::path& path);
	void SavePGM(const Image& image, const std::filesystem::path& path);

}  // namespace ImageCore
