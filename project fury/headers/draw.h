#pragma once

#include "../imgui/imgui.h"

#include "colors.h"
#include "bone.h"
#include "memory.h"
#include "menu.h"
#include "offsets.h"
#include "cmath.h"
#include <string>

namespace draw {
	float distance_2d(ImVec2 origin, ImVec2 dest);
	float distance_3d(Vec3 origin, Vec3 dest);
	bool wts(Vec3 pos, float matrix[16], int width, int height, float& screen_x, float& screen_y) noexcept;
	void draw_box(ImDrawList* draw_list, ImVec2 topLeft, ImVec2 bottomRight);
	void draw_corner_box(ImDrawList* draw_list, ImVec2 topLeft, ImVec2 bottomRight, ImVec2 head, float boxWidth);
	void draw_skeleton(ImDrawList* draw_list, const esp_entity& entity);
	void draw_head_circle(ImDrawList* draw_list, ImVec2 head, float BoxHeigth);
	void draw_name(ImDrawList* draw_list, const std::string& name, ImVec2 topLeft);
	void draw_health_bar(ImDrawList* draw_list, int32_t health, ImVec2 topLeft, ImVec2 bottomRight, float barWidth, float spacing);
	void draw_armor_bar(ImDrawList* draw_list, int32_t armor, ImVec2 topLeft, ImVec2 bottomRight, float barHeight, float spacing);
	void draw_distance(ImDrawList* draw_list, Vec3 origin, const esp_entity& entity, ImVec2 top_right);
	void draw_distance_line(ImDrawList* draw_list, const esp_entity& entity);
	void draw_entity_esp(ImDrawList* draw_list, const esp_entity& entity);
	void draw_esp() noexcept;
}