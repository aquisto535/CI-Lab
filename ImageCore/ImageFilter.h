#pragma once

#include "Image.h"

#include <cstdint>

namespace ImageCore {

	Image ApplyBoxBlur(const Image& src, int radius);
	Image ApplyUnsharpMask(const Image& src, float amount);
	Image NormalizeContrast(const Image& src);

}  // namespace ImageCore