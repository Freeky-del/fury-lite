#include "../fonts/fonts.h"
#include "../fonts/fontawesome.cpp"
#include "../fonts/fontawesome.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_internal.h"

#include "../headers/menu.h"
#include "../headers/cmath.h"
#include "../headers/globals.h"
#include "../headers/colors.h"
#include "../headers/draw.h"

#include <DirectxTex.h>
#pragma comment(lib, "DirectxTex.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static int debounceFrames = 0;
inline ImFont* icons_font = nullptr;
inline ImFont* ice_cold_font = nullptr;

typedef struct Animator {
	float alpha;
	bool tick;
	float speed;
};

void AnimateContent(Animator& animator) {
	const int limit = 255;

	if (animator.tick || animator.alpha == limit) {
		animator.tick = true;
		if (!(animator.alpha <= 0)) animator.alpha -= animator.speed;
		else if (animator.alpha <= 0) animator.tick ^= 1;
	}
}

void CustomCheckBox(const char* format, bool* value) {
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.5f, 1.5f));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.5f);
	
	ImGui::PushStyleColor(ImGuiCol_Border, ImColor(145, 0, 0, 255).Value);
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(145, 0, 0, 25).Value);
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImColor(145, 0, 0, 50).Value);
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImColor(145, 0, 0, 50).Value);

	ImGui::Checkbox(format, value);
	
	ImGui::PopStyleColor(4);
	
	ImGui::PopStyleVar(3);
}

void CustomSliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d") {
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.0f, 5.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.5f);
	ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 5.0f);

	ImGui::PushStyleColor(ImGuiCol_Border, ImColor(145, 0, 0, 255).Value);
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImColor(200, 50, 50, 255).Value);
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImColor(255, 80, 80, 255).Value);
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(145, 0, 0, 25).Value);
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImColor(145, 0, 0, 50).Value);
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImColor(145, 0, 0, 50).Value);
	
	ImGui::Text("%s", label);
	
	ImGui::PushID(label);

	ImGui::SliderInt("##slider", v, v_min, v_max, format);
	
	ImGui::PopID();

	ImGui::PopStyleColor(6);
	
	ImGui::PopStyleVar(4);
	
	ImGui::Dummy(ImVec2(5.0f, 5.0f));
}

void CustomSliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f") {
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.0f, 5.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.5f);
	ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 5.0f);

	ImGui::PushStyleColor(ImGuiCol_Border, ImColor(145, 0, 0, 255).Value);
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImColor(200, 50, 50, 255).Value);
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImColor(255, 80, 80, 255).Value);
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(145, 0, 0, 25).Value);
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImColor(145, 0, 0, 50).Value);
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImColor(145, 0, 0, 50).Value);

	ImGui::Text("%s", label);

	ImGui::PushID(label);

	ImGui::SliderFloat("##slider", v, v_min, v_max, format);

	ImGui::PopID();

	ImGui::PopStyleColor(6);

	ImGui::PopStyleVar(4);

	ImGui::Dummy(ImVec2(5.0f, 5.0f));
}

ImVec4 ToImVec4(const Vec4& col) {
	return ImVec4(col.r, col.g, col.b, col.a);
}

Vec4 FromImVec4(const ImVec4& col) {
	return Vec4{ col.x, col.y, col.z, col.w };
}

void ColorPickerPopup(const char* label, Vec4& color) {
	ImGui::Text("%s", label);
	ImGui::SameLine();

	float radius = 10.0f;
	ImVec2 p = ImGui::GetCursorScreenPos();
	ImVec2 center = ImVec2(p.x + radius, p.y + radius);

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddCircleFilled(center, radius, ImGui::ColorConvertFloat4ToU32(ToImVec4(color)), 32);

	ImGui::SetCursorScreenPos(ImVec2(p.x, p.y));
	ImGui::InvisibleButton((std::string("##") + label).c_str(), ImVec2(radius * 2, radius * 2));

	if (ImGui::IsItemClicked()) {
		ImGui::OpenPopup((std::string(label) + "_popup").c_str());
	}

	if (ImGui::BeginPopup((std::string(label) + "_popup").c_str())) {
		ImVec4 tmp = ToImVec4(color);
		if (ImGui::ColorPicker4(("##" + std::string(label) + "_picker").c_str(), (float*)&tmp, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview)) {
			color = FromImVec4(tmp);
		}
		ImGui::EndPopup();
	}
}

void CenterButtons(std::vector<std::string> names, std::vector<int> indexs, int& selected_index) {
	std::vector<ImVec2> sizes = {};
	float total_area = 0.0f;

	const auto& style = ImGui::GetStyle();

	for (std::string& name : names) {
		const ImVec2 label_size = ImGui::CalcTextSize(name.c_str(), 0, true);
		ImVec2 size = ImGui::CalcItemSize(ImVec2(), label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

		size.x += 45.0f;
		size.y += 15.0f;

		sizes.push_back(size);
		total_area += size.x;
	}

	ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (total_area / 2));
	for (uint32_t i = 0; i < names.size(); i++) {
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

		if (selected_index == indexs[i]) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImColor(145, 0, 0, 255).Value);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(145, 0, 0, 255).Value);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(145, 0, 0, 255).Value);
			if (ImGui::Button(names[i].c_str(), sizes[i]))  selected_index = indexs[i];
			ImGui::PopStyleColor(3);
		}
		else {
			if (ImGui::Button(names[i].c_str(), sizes[i]))  selected_index = indexs[i];
		}

		ImGui::PopStyleVar();
		if (i != (names.size() - 1)) ImGui::SameLine();
	}
}

LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM w_param, LPARAM l_param) {
	if (ImGui_ImplWin32_WndProcHandler(window, message, w_param, l_param)) {
		return 0L;
	}


	switch (message) {
	case WM_NCHITTEST:
	{
		const LONG borderWidth = GetSystemMetrics(SM_CXSIZEFRAME);
		const LONG titleBarHeight = GetSystemMetrics(SM_CYCAPTION);
		POINT cursorPos = { GET_X_LPARAM(w_param), GET_Y_LPARAM(l_param) };
		RECT windowRect;
		GetWindowRect(window, &windowRect);

		if (cursorPos.y >= windowRect.top && cursorPos.y < windowRect.top + titleBarHeight)
			return HTCAPTION;

		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(window, message, w_param, l_param);
}

void menu::UpdateFade()
{
	using namespace std::chrono;
	auto now = steady_clock::now();
	float delta = duration<float>(now - lastTime).count();
	lastTime = now;

	if (fadingIn) {
		alpha += delta * 3.0f; // velocità fade-in
		if (alpha >= 1.0f) {
			alpha = 1.0f;
			fadingIn = false;
		}
	}
	else if (fadingOut) {
		alpha -= delta * 3.0f; // velocità fade-out
		if (alpha <= 0.0f) {
			alpha = 0.0f;
			fadingOut = false;
			show = false;  // Nascondi la finestra alla fine del fade-out
		}
	}
}

void menu::ShowMenu()
{
	if (!show) {
		show = true;
		fadingIn = true;
		fadingOut = false;
		alpha = 0.0f;
		lastTime = std::chrono::steady_clock::now();
	}
}

void menu::HideMenu()
{
	if (show) {
		fadingOut = true;
		fadingIn = false;
		lastTime = std::chrono::steady_clock::now();
	}
}

void menu::UpdateClickability()
{
	LONG_PTR style = GetWindowLongPtr(window, GWL_EXSTYLE);

	if (alpha <= 0.0f) {
		// Imposta WS_EX_TRANSPARENT ? overlay diventa "click-through"
		SetWindowLongPtr(window, GWL_EXSTYLE, style | WS_EX_TRANSPARENT);
	}
	else {
		// Rimuove WS_EX_TRANSPARENT ? overlay riceve eventi mouse
		SetWindowLongPtr(window, GWL_EXSTYLE, style & ~WS_EX_TRANSPARENT);
	}
}

void menu::CreateHWindow(LPCWSTR windowName, LPCWSTR className) noexcept {
	windowClass.cbSize = sizeof(WNDCLASSEXW);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = window_procedure;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = className;
	windowClass.hIconSm = 0;

	RegisterClassExW(&windowClass);

	window = CreateWindowExW(
		WS_EX_TOPMOST | WS_EX_LAYERED,
		className,
		windowName,
		WS_POPUP,
		0,
		0,
		width,
		height,
		nullptr,
		nullptr,
		windowClass.hInstance,
		nullptr
	);

	SetLayeredWindowAttributes(window, RGB(0, 0, 0), 255, LWA_COLORKEY);

	sd.BufferDesc.RefreshRate.Numerator = 60U;
	sd.BufferDesc.RefreshRate.Denominator = 1U;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1U;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2U;
	sd.OutputWindow = window;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void menu::DestroyHWindow() noexcept {
	DestroyWindow(window);
	UnregisterClassW(windowClass.lpszClassName, windowClass.hInstance);
}

bool menu::CreateDevice() noexcept {
	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		levels,
		2,
		D3D11_SDK_VERSION,
		&sd,
		&swap_chain,
		&device,
		&level,
		&device_context
	);

	ID3D11Texture2D* back_buffer = nullptr;
	swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));

	if (back_buffer) {
		device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
		back_buffer->Release();
		return true;
	}
	else return false;
}

void menu::DestroyDevice() noexcept {
	if (render_target_view) { render_target_view->Release(); render_target_view = nullptr; }
	if (swap_chain) { swap_chain->Release(); swap_chain = nullptr; }
	if (device_context) { device_context->Release(); device_context = nullptr; }
	if (device) { device->Release(); device = nullptr; }
}

void menu::CreateImGui() noexcept {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ::ImGui::GetIO();
	ImGuiStyle& style = ::ImGui::GetStyle();
	auto& colors = style.Colors;

	io.LogFilename = NULL;
	io.IniFilename = NULL;

	io.Fonts->AddFontFromMemoryTTF(font_rubik, sizeof(font_rubik), 18.5f);

	static const ImWchar icon_ranges[]{ 0xF000, 0xFF8F, 0 };
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = 3;
	icons_config.OversampleV = 3;

	 io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 16.0f, &icons_config, icon_ranges);

	style.ScrollbarRounding = 0;
	style.WindowRounding = 4.0f;

	style.ScrollbarRounding = 0;
	style.WindowRounding = 4.0f;

	colors[ImGuiCol_Button] = ImColor(18, 18, 18, 100);
	colors[ImGuiCol_ButtonActive] = ImColor(21, 21, 21, 100);
	colors[ImGuiCol_ButtonHovered] = ImColor(21, 21, 21, 100);

	colors[ImGuiCol_CheckMark] = ImColor(145, 0, 0, 255);

	colors[ImGuiCol_FrameBg] = ImColor(24, 24, 24);
	colors[ImGuiCol_FrameBgActive] = ImColor(26, 26, 26);
	colors[ImGuiCol_FrameBgHovered] = ImColor(26, 26, 26);

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, device_context);
}

void menu::DestroyImGui() noexcept {
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void menu::BeginRender() noexcept {
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void menu::EndRender() noexcept {
	ImGui::Render();

	float color[4]{ 0,0,0,1 };
	device_context->OMSetRenderTargets(1U, &render_target_view, nullptr);
	device_context->ClearRenderTargetView(render_target_view, color);

	ImGuiIO& io = ImGui::GetIO();

	D3D11_VIEWPORT vp{};
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = io.DisplaySize.x;
	vp.Height = io.DisplaySize.y;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	device_context->RSSetViewports(1, &vp);
	device_context->RSSetState(nullptr);
	device_context->OMSetDepthStencilState(nullptr, 0);

	FLOAT blendFactor[4]{ 0.f, 0.f, 0.f, 0.f };
	device_context->OMSetBlendState(nullptr, blendFactor, 0xffffffff);

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swap_chain->Present(1U, 0U); // vsync ON
}

void menu::Render() noexcept {
	ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
	draw_list->AddText(ImVec2(0, 0), ImU32(ImColor(145, 0, 0, 255)), "Fury Lite - v1.1.0");

	ImVec2 bottom_left_screen(0, menu::height - 300.0f);
	ImColor outline = ImColor(1, 0, 0, 255);

	static Animator animator{ 255, false, 4.5f };
	AnimateContent(animator);

	if (globals::enableEsp) {
		draw::draw_esp();

		if (globals::esp::box_esp) draw_list->AddText(bottom_left_screen, colors::green, "Box Esp");
		if (globals::esp::corner_esp) draw_list->AddText(bottom_left_screen, colors::green, "Corner Esp");
		if (globals::esp::skeleton_esp) draw_list->AddText(ImVec2(bottom_left_screen.x, bottom_left_screen.y + 20.0f), colors::green, "Skeleton Esp");
		if (globals::esp::joints) draw_list->AddText(ImVec2(bottom_left_screen.x, bottom_left_screen.y + 40.0f), colors::green, "Joints");
		if (globals::esp::head_circle) draw_list->AddText(ImVec2(bottom_left_screen.x, bottom_left_screen.y + 60.0f), colors::green, "Head Circle");
		if (globals::esp::name_esp) draw_list->AddText(ImVec2(bottom_left_screen.x, bottom_left_screen.y + 80.0f), colors::green, "Player Name");
		if (globals::esp::health_bar) draw_list->AddText(ImVec2(bottom_left_screen.x, bottom_left_screen.y + 100.0f), colors::green, "Player Health");
		if (globals::esp::armor_bar) draw_list->AddText(ImVec2(bottom_left_screen.x, bottom_left_screen.y + 120.0f), colors::green, "Player Armor");
		if (globals::esp::distance) draw_list->AddText(ImVec2(bottom_left_screen.x, bottom_left_screen.y + 140.0f), colors::green, "Distance");
		if (globals::esp::distance_line) draw_list->AddText(ImVec2(bottom_left_screen.x, bottom_left_screen.y + 160.0f), colors::green, "Distance Line");
		if (globals::team_checker) draw_list->AddText(ImVec2(bottom_left_screen.x, bottom_left_screen.y + 180.0f), colors::green, "Team Checker");
	}

	UpdateFade();
	UpdateClickability();

	if (alpha <= 0.0f) return;

	ImGui::SetNextWindowPos(ImVec2(20, 20));
	ImGui::SetNextWindowSize(ImVec2(700, 500));
	ImGui::SetNextWindowBgAlpha(alpha);

	ImGui::Begin("Fury Lite",
		NULL,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar
	);

	ImVec2 window_pos{ ImGui::GetWindowPos() };
	ImVec2 window_size{ ImGui::GetWindowSize() };
	ImVec2 cursor_pos{ ImGui::GetCursorPos() };

	ImGuiIO& io = ::ImGui::GetIO();
	ice_cold_font = io.Fonts->AddFontFromMemoryTTF(ice_cold, sizeof(ice_cold), 30.0f);

	ImGui::PushStyleColor(ImGuiCol_Text, ImU32(ImColor(145, 0, 0, 255)));
	ImGui::PushFont(ice_cold_font);

	ImGui::Text("Fury");

	ImGui::PopStyleColor();

	ImGui::SameLine();

	ImGui::Text("Lite");

	ImGui::PopFont();

	CenterButtons(
		{ ICON_FA_EYE" Visuals"},
		{ 0 },
		index
	);

	ImGui::PushStyleColor(ImGuiCol_Separator, ImColor(145, 0, 0, 255).Value);
	ImGui::Separator();
	ImGui::PopStyleColor();

	ImGui::BeginChild("##left_side", ImVec2(ImGui::GetContentRegionAvail().x / 2.0f, ImGui::GetContentRegionAvail().y));
	static int esp_selected = 0;
	struct option { const char* name; ImU32 color; };
	option esp_options[] = {
		{ ICON_FA_CROSSHAIRS" Box 2D", IM_COL32(255, 255, 255, 255)},
		{ ICON_FA_EYE" Corners", IM_COL32(255, 255, 255, 255) },
		{ "None", IM_COL32(255,  255, 255, 255)}
	};

	switch (index) {
	case 0:
		CustomCheckBox("Enable", &globals::enableEsp);
		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2(200.0f, 0));
		CustomCheckBox("Team Checker", &globals::team_checker);
		
		//combo
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 5.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.5f);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

			ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 3.5f);

			ImGui::PushStyleColor(ImGuiCol_Border, ImColor(145, 0, 0, 255).Value);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(145, 0, 0, 25).Value);
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImColor(145, 0, 0, 50).Value);
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImColor(145, 0, 0, 50).Value);
			
			ImGui::Text("Esp type");
			if (ImGui::BeginCombo(" ", esp_options[esp_selected].name, ImGuiComboFlags_NoArrowButton)) {
				for (int i = 0; i < IM_ARRAYSIZE(esp_options); ++i) {
					ImGui::PushStyleColor(ImGuiCol_Text, esp_options[i].color);
					ImGui::PushStyleColor(ImGuiCol_Header, ImColor(145, 0, 0, 25).Value);
					ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImColor(145, 0, 0, 50).Value);
					ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImColor(145, 0, 0, 50).Value);

					if (ImGui::Selectable(esp_options[i].name, esp_selected == i)) esp_selected = i;

					ImGui::PopStyleColor(4);
				}
				ImGui::EndCombo();
			}

			ImGui::PopStyleVar(4);
			ImGui::PopStyleColor(4);

			ImGui::Dummy(ImVec2(10.0f, 10.0f));
		}

		CustomCheckBox("Skeletons", &globals::esp::skeleton_esp);
		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2(200.0f, 95.0f));
		CustomCheckBox("Joints", &globals::esp::joints);
		CustomCheckBox("Head Circle", &globals::esp::head_circle);
		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2(200.0f, 120.0f));
		CustomCheckBox("Player Name", &globals::esp::name_esp);
		CustomCheckBox("Player Health", &globals::esp::health_bar);
		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2(200.0f, 145.0f));
		CustomCheckBox("Player Armor", &globals::esp::armor_bar);
		CustomCheckBox("Distance", &globals::esp::distance);
		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2(200.0f, 170.0f));
		CustomCheckBox("Distance Line", &globals::esp::distance_line);

		switch (esp_selected) {
		case 0:
			globals::esp::corner_esp = false;
			globals::esp::box_esp = true;
			break;
		case 1:
			globals::esp::corner_esp = true;
			globals::esp::box_esp = false;
			break;
		case 2:
			globals::esp::corner_esp = false;
			globals::esp::box_esp = false;
		}
		break;
	}

	ImGui::EndChild();
	
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Separator, ImColor(145, 0, 0, 255).Value);
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
	ImGui::PopStyleColor();
	ImGui::SameLine();

	ImGui::BeginChild("##right_side", ImVec2(ImGui::GetContentRegionAvail().x / 1.0f, ImGui::GetContentRegionAvail().y));

	switch (index) {
	case 0:
		ColorPickerPopup("Esp Color", colors::esp_color);
	
		if (globals::esp::corner_esp || globals::esp::box_esp) CustomSliderFloat("Box Thickness", &globals::esp::box_thickness, 1.0f, 3.0f);
		if (globals::esp::skeleton_esp) CustomSliderFloat("Skeleton Thickness", &globals::esp::skeleton_thickness, 1.0f, 3.0f);
	
		if (globals::esp::distance_line) {
			static int line_selected = 0;
			option distance_line_options[] = {
				{ "Top", IM_COL32(255, 255, 255, 255)},
				{ "Center", IM_COL32(255, 255, 255, 255)},
				{ "Bottom", IM_COL32(255, 255, 255, 255)}
			};

			//combo
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 5.0f));
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.5f);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

				ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 3.5f);

				ImGui::PushStyleColor(ImGuiCol_Border, ImColor(145, 0, 0, 255).Value);
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(145, 0, 0, 25).Value);
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImColor(145, 0, 0, 50).Value);
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImColor(145, 0, 0, 50).Value);

				ImGui::Text("Line Position");
				if (ImGui::BeginCombo(" ", distance_line_options[line_selected].name, ImGuiComboFlags_NoArrowButton)) {
					for (int i = 0; i < IM_ARRAYSIZE(distance_line_options); ++i) {
						ImGui::PushStyleColor(ImGuiCol_Text, distance_line_options[i].color);
						ImGui::PushStyleColor(ImGuiCol_Header, ImColor(145, 0, 0, 25).Value);
						ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImColor(145, 0, 0, 50).Value);
						ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImColor(145, 0, 0, 50).Value);

						if (ImGui::Selectable(distance_line_options[i].name, line_selected == i)) line_selected = i;

						ImGui::PopStyleColor(4);
					}
					ImGui::EndCombo();
				}

				ImGui::PopStyleVar(4);
				ImGui::PopStyleColor(4);

				ImGui::Dummy(ImVec2(10.0f, 10.0f));
			}

			switch (line_selected) {
			case 0:
				globals::esp::distance_line_position::top = true;
				globals::esp::distance_line_position::center = false;
				globals::esp::distance_line_position::bottom = false;
				break;
			case 1:
				globals::esp::distance_line_position::top = false;
				globals::esp::distance_line_position::center = true;
				globals::esp::distance_line_position::bottom = false;
				break;
			case 2:
				globals::esp::distance_line_position::top = false;
				globals::esp::distance_line_position::center = false;
				globals::esp::distance_line_position::bottom = true;
				break;
			}
		}
	}

	ImGui::EndChild();

	draw_list->AddRect({ window_pos.x - 1, window_pos.y - 1 }, { window_pos.x + window_size.x + 1, window_pos.y + window_size.y + 1 },
		ImColor(145, 0, 0, (int)animator.alpha), 4.0f, 0, 1.0f);

	ImGui::End();
}