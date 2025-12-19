#pragma once
#include "cmath.h"

#include <cstdint>
#include <Windows.h>
#include <mutex>

namespace globals {
	inline std::uintptr_t client = 0;
	inline std::uintptr_t engine = 0;
	inline std::uint32_t pid = 0;

	inline uintptr_t LocalPlayerPawn = NULL;
	inline uintptr_t EntityList = NULL;
	inline float ViewMatrix[16];
	inline Vec3 old_origin;

	inline bool team_checker = false;

	inline std::mutex esp_mutex;
	inline std::vector<esp_entity> g_espEntityUpdate;
	inline std::vector<esp_entity> g_espEntityRender;

	inline bool enableEsp = false;
	namespace esp {
		inline float skeleton_thickness = 1.0f;
		inline float box_thickness = 1.0f;

		inline bool box_esp = false;
		inline bool corner_esp = false;
		inline bool skeleton_esp = false;
		inline bool head_circle = false;
		inline bool health_bar = false;
		inline bool armor_bar = false;
		inline bool name_esp = false;
		inline bool distance = false;
		inline bool distance_line = false;

		namespace distance_line_position {
			inline bool top = false;
			inline bool center = false;
			inline bool bottom = false;
		}
	}
}