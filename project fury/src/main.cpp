#include "../headers/memory.h"
#include "../headers/menu.h"
#include "../headers/hacks.h"
#include "../headers/offsets.h"

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE previousInstance, PWSTR arguments, int commandShow) {
	while (!globals::pid || !globals::client || !globals::engine) {
		globals::pid = memory::get_process_id("cs2.exe");
		
		if (!globals::pid) {
			MessageBoxA(0, "process id not found, make sure cs2 is open", "OK", 0);
			continue;
		}

		globals::client = memory::get_module_base_address("client.dll");
		globals::engine = memory::get_module_base_address("engine2.dll");
		
		if (!globals::client || !globals::engine) {
			MessageBoxA(0, "client.dll and engine2.dll not found, click \"OK\" to retry", "OK", 0);
		}
	}

	menu::width = memory::read<int>(globals::engine + offsets::engine2_dll::dwWindowWidth);
	menu::height = memory::read<int>(globals::engine + offsets::engine2_dll::dwWindowHeight);

	std::thread(hacks::VisualsThread).detach();

	menu::CreateHWindow(L"Fury Lite", L"Class");
	menu::CreateDevice();
	menu::CreateImGui();

	while (menu::exit == false) {
		if (GetAsyncKeyState(VK_END) & 0x8000) {
			menu::exit = true;
			continue;
		}

		static bool insertPressedLastFrame = false;

		bool insertDownNow = (GetAsyncKeyState(VK_INSERT) & 0x8000) != 0;

		if (insertDownNow && !insertPressedLastFrame) {
			if (menu::show) {
				menu::HideMenu();
			}
			else {
				menu::ShowMenu();
			}
		}

		insertPressedLastFrame = insertDownNow;

		menu::BeginRender();
		menu::Render();
		menu::EndRender();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	menu::DestroyImGui();
	menu::DestroyDevice();
	menu::DestroyHWindow();

	return EXIT_SUCCESS;
}