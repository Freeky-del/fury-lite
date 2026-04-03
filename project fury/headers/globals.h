#pragma once
#include "cmath.h"

#include <cstdint>
#include <Windows.h>

namespace globals {
	inline std::uintptr_t client = 0;
	inline std::uintptr_t engine = 0;
	inline std::uint32_t pid = 0;
	inline bool updated = false;

	inline uintptr_t LocalPlayerPawn = NULL;
	inline uintptr_t EntityList = NULL;
	inline float ViewMatrix[16];
	inline Vec3 old_origin;
	inline uint8_t local_team;

	inline bool team_checker = false;

	inline std::vector<esp_entity> g_espEntityUpdate;
	inline std::vector<esp_entity> g_espEntityRender;
	inline std::vector<bomb_entity> g_bombEntityUpdate;

	inline bool enableEsp = false;
	namespace esp {
		inline float skeleton_thickness = 1.0f;
		inline float box_thickness = 1.0f;

		inline bool box_esp = false;
		inline bool corner_esp = false;
		inline bool skeleton_esp = false;
		inline bool joints = false;
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

	inline bool enable_bomb_info = false;
	namespace bomb {
		inline bool site = false;
		inline bool status = false;
		inline bool timer = false;
	}
}