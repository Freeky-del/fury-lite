#pragma once
#include "cmath.h"

#include "../imgui/imgui.h"

#include <iostream>
#include <cstdint>
#include <cstddef>
#include <wtypes.h>

inline DWORD getARGB(Vec4 color) {
	return ((DWORD)(color.a * 255) << 24) |
		((DWORD)(color.b * 255) << 16) |
		((DWORD)(color.g * 255) << 8) |
		((DWORD)(color.r * 255));
}

namespace colors {
	inline ImColor red = ImColor(255, 0, 0, 255);
	inline ImColor green = ImColor(0, 255, 0, 255);
	inline ImColor white = ImColor(255, 255, 255, 255);
	inline ImColor orange = ImColor(255, 140, 0, 255);

	inline Vec4 esp_color = { 1.0f, 1.0f, 1.0f, 1.0f };
}