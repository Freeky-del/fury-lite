#include "../headers/draw.h"

#include <algorithm>

BoneConnection boneConnections[] = {
	BoneConnection(6, 5),
	BoneConnection(5, 4),
	BoneConnection(4, 0),
	BoneConnection(4, 8),
	BoneConnection(8, 9),
	BoneConnection(9, 11),
	BoneConnection(4, 13),
	BoneConnection(13, 14),
	BoneConnection(14, 16),
	BoneConnection(4, 2),
	BoneConnection(0, 22),
	BoneConnection(0, 25),
	BoneConnection(22, 23),
	BoneConnection(23, 24),
	BoneConnection(25, 26),
	BoneConnection(26, 27)
};

float draw::distance_2d(ImVec2 origin, ImVec2 dest) {
	ImVec2 d(origin.x - dest.x, origin.y - dest.y);
	return sqrt(d.x * d.x + d.y * d.y);
}

float draw::distance_3d(Vec3 origin, Vec3 dest) {
	Vec3 d(origin.x - dest.x, origin.y - dest.y, origin.z - dest.z);
	return sqrt(d.x * d.x + d.y * d.y + d.z * d.z);
}

bool draw::wts(Vec3 pos, float matrix[16], int width, int height, float& screen_x, float& screen_y) noexcept {
	Vec3 clip;
	clip.x = pos.x * matrix[0] + pos.y * matrix[1] + pos.z * matrix[2] + matrix[3];
	clip.y = pos.x * matrix[4] + pos.y * matrix[5] + pos.z * matrix[6] + matrix[7];
	clip.z = pos.x * matrix[12] + pos.y * matrix[13] + pos.z * matrix[14] + matrix[15];

	if (clip.z < 0.01f) return false;

	ImVec2 ndc;
	ndc.x = clip.x / clip.z;
	ndc.y = clip.y / clip.z;

	screen_x = static_cast<int>((ndc.x + 1.0f) * 0.5f * width);
	screen_y = static_cast<int>((1.0f - ndc.y) * 0.5f * height);

	return true;
}

void draw::draw_box(ImDrawList* draw_list, ImVec2 topLeft, ImVec2 bottomRight)
{
	draw_list->AddRect(topLeft, bottomRight, getARGB(colors::esp_color), 0.0f, 0, globals::esp::box_thickness);
}

void draw::draw_corner_box(ImDrawList* draw_list, ImVec2 topLeft, ImVec2 bottomRight, ImVec2 head, float boxWidth)
{
	float boxHeight = bottomRight.y - topLeft.y;
	float corner_size = std::clamp(boxHeight * 0.05f, 5.0f, 25.0f);
	
	draw_list->AddLine(topLeft, ImVec2(topLeft.x, topLeft.y + corner_size), getARGB(colors::esp_color), globals::esp::box_thickness);
	draw_list->AddLine(topLeft, ImVec2(topLeft.x + corner_size, topLeft.y), getARGB(colors::esp_color), globals::esp::box_thickness);

	ImVec2 topRight(head.x + boxWidth / 2.0f, head.y);
	draw_list->AddLine(topRight, ImVec2(topRight.x - corner_size, topRight.y), getARGB(colors::esp_color), globals::esp::box_thickness);
	draw_list->AddLine(topRight, ImVec2(topRight.x, topRight.y + corner_size), getARGB(colors::esp_color), globals::esp::box_thickness);

	draw_list->AddLine(bottomRight, ImVec2(bottomRight.x, bottomRight.y - corner_size), getARGB(colors::esp_color), globals::esp::box_thickness);
	draw_list->AddLine(bottomRight, ImVec2(bottomRight.x - corner_size, bottomRight.y), getARGB(colors::esp_color), globals::esp::box_thickness);

	ImVec2 bottomLeft(head.x - boxWidth / 2.0f, bottomRight.y);
	draw_list->AddLine(bottomLeft, ImVec2(bottomLeft.x + corner_size, bottomLeft.y), getARGB(colors::esp_color), globals::esp::box_thickness);
	draw_list->AddLine(bottomLeft, ImVec2(bottomLeft.x, bottomLeft.y - corner_size), getARGB(colors::esp_color), globals::esp::box_thickness);
}

void draw::draw_skeleton(ImDrawList* draw_list, const esp_entity& entity)
{
	for (const auto& connection : boneConnections) {
		Vec3 b1 = memory::read<Vec3>(entity.boneMatrix + connection.bone1 * 32);
		Vec3 b2 = memory::read<Vec3>(entity.boneMatrix + connection.bone2 * 32);

		ImVec2 p1, p2;
		if (wts(b1, globals::ViewMatrix, menu::width, menu::height, p1.x, p1.y) &&
			wts(b2, globals::ViewMatrix, menu::width, menu::height, p2.x, p2.y)) {
			draw_list->AddLine(p1, p2, getARGB(colors::esp_color), globals::esp::skeleton_thickness);
			draw_list->AddCircleFilled(p2, globals::esp::skeleton_thickness  + 1.0f,  getARGB(colors::esp_color));
		}
	}
}

void draw::draw_head_circle(ImDrawList* draw_list, ImVec2 head, float BoxHeigth)
{
	float radius = (BoxHeigth / 8.0f) - 3;
	draw_list->AddCircle(head, radius,  getARGB(colors::esp_color), 0, 1.0f);
	draw_list->AddCircleFilled(head, globals::esp::skeleton_thickness + 1.0f,  getARGB(colors::esp_color));

}

void draw::draw_name(ImDrawList* draw_list, const std::string& name, ImVec2 topLeft)
{
	ImVec2 name_pos(topLeft.x, topLeft.y - 20.0f);
	draw_list->AddText(name_pos, colors::white, name.c_str());
}

void draw::draw_health_bar(ImDrawList* draw_list, int32_t health, ImVec2 topLeft, ImVec2 bottomRight, float boxWidth)
{
	float percent = std::clamp(health / 100.0f, 0.0f, 1.0f);
	float barWidth = std::clamp(boxWidth * 0.08f, 3.0f, 6.0f);
	float spacing = std::clamp(barWidth * 0.03f, 2.0f, 4.0f);

	float bar_x = topLeft.x - barWidth - spacing;
	float bar_y_top = topLeft.y;
	float bar_y_bottom = bottomRight.y;

	float health_height = (bar_y_bottom - bar_y_top) * percent;

	ImU32 health_color = (health > 80) ? colors::green : (health > 50 ? colors::orange : colors::red);
	draw_list->AddRectFilled(
		ImVec2(bar_x, bar_y_bottom - health_height),
		ImVec2(bar_x + barWidth, bar_y_bottom),
		health_color
	);
}

void draw::draw_armor_bar(ImDrawList* draw_list, int32_t armor, ImVec2 topLeft, ImVec2 bottomRight, float boxHeight)
{
	float percent = std::clamp(armor / 100.0f, 0.0f, 1.0f);
	float barHeight = std::clamp(boxHeight * 0.05f, 4.0f, 8.0f);
	float spacing = std::clamp(boxHeight * 0.03f, 3.0f, 6.0f);

	float bar_y = bottomRight.y + spacing;
	float bar_x_left = topLeft.x;
	float bar_x_right = bottomRight.x;

	float armor_width = (bar_x_right - bar_x_left) * percent;

	draw_list->AddRectFilled(
		ImVec2(bar_x_left, bar_y),
		ImVec2(bar_x_left + armor_width, bar_y + barHeight),
		colors::orange
	);
}

void draw::draw_distance(ImDrawList* draw_list, Vec3 origin, const esp_entity& entity, ImVec2 top_right)
{
	float dst = distance_3d(origin, entity.old_origin);
	dst = dst / 100.0f;
	int dst_int = static_cast<int>(std::trunc(dst));

	draw_list->AddText(top_right, colors::white, std::string(std::to_string(dst_int) + "m").c_str());
}

void draw::draw_distance_line(ImDrawList* draw_list, const esp_entity& entity)
{
	ImVec2 origin;
	Vec3 stomach = memory::read<Vec3>(entity.boneMatrix + bones::spine_1 * 32);

	if (wts(stomach, globals::ViewMatrix, menu::width, menu::height, origin.x, origin.y)) {
		if(globals::esp::distance_line_position::top) draw_list->AddLine(ImVec2(menu::width / 2.0f,  0), origin, getARGB(colors::esp_color), globals::esp::box_thickness);
		if(globals::esp::distance_line_position::center) draw_list->AddLine(ImVec2(menu::width / 2.0f, menu::height / 2.0f), origin, getARGB(colors::esp_color), globals::esp::box_thickness);
		if(globals::esp::distance_line_position::bottom) draw_list->AddLine(ImVec2(menu::width / 2.0f, menu::height), origin, getARGB(colors::esp_color), globals::esp::box_thickness);
	}
}

void draw::draw_entity_esp(ImDrawList* draw_list, const esp_entity& entity)
{
	ImVec2 feet, head;

	if (!wts(entity.feet, globals::ViewMatrix, menu::width, menu::height, feet.x, feet.y) ||
		!wts(entity.head, globals::ViewMatrix, menu::width, menu::height, head.x, head.y)) 
		return;

	ImVec2 box;
	box.y = static_cast<float>(feet.y - head.y);
	box.x = box.y / 2.2f;

	ImVec2 top_left(head.x - box.x / 2.0f, head.y), bottom_right(head.x + box.x / 2.0f, feet.y);
	ImVec2 top_right(head.x + box.x / 2.0f, head.y);

	if (globals::esp::box_esp) draw_box(draw_list, top_left, bottom_right);
	if (globals::esp::corner_esp) draw_corner_box(draw_list, top_left, bottom_right, head, box.x);
	if (globals::esp::skeleton_esp) draw_skeleton(draw_list, entity);
	if (globals::esp::head_circle) draw_head_circle(draw_list, head, box.y);
	if (globals::esp::name_esp) draw_name(draw_list, entity.name, top_left);
	if (globals::esp::health_bar) draw_health_bar(draw_list, entity.health, top_left, bottom_right, box.x);
	if (globals::esp::armor_bar) draw_armor_bar(draw_list, entity.armor, top_left, bottom_right, box.y);
	if (globals::esp::distance) draw_distance(draw_list, globals::old_origin, entity, top_right);
	if (globals::esp::distance_line) draw_distance_line(draw_list, entity);
}

void draw::draw_esp() noexcept
{
	if (!globals::enableEsp || !globals::LocalPlayerPawn) return;

	ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
	uint8_t local_team = memory::read<uint8_t>(globals::LocalPlayerPawn + offsets::client_dll::C_BaseEntity::m_iTeamNum);
	memory::read_array<float>(globals::client + offsets::client_dll::dwViewMatrix, globals::ViewMatrix, 16);

	std::lock_guard<std::mutex> lock(globals::esp_mutex);

	for (auto& entity : globals::g_espEntityRender) {
		if (globals::team_checker && entity.team == local_team) continue;

		draw_entity_esp(draw_list, entity);
	}
}
