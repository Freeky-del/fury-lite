#include "../headers/hacks.h"

void hacks::VisualsThread() noexcept {
	while (true) {
		globals::LocalPlayerPawn = memory::read<uintptr_t>(globals::client + offsets::client_dll::dwLocalPlayerPawn);
		globals::EntityList = memory::read<uintptr_t>(globals::client + offsets::client_dll::dwEntityList);
		globals::old_origin = memory::read<Vec3>(globals::LocalPlayerPawn + offsets::client_dll::CBasePlayerPawn::m_vOldOrigin);

		if (globals::enableEsp) {
			globals::g_espEntityUpdate.clear();
			for (int i = 0; i < 64; ++i) {
				uintptr_t listEntry = memory::read<uintptr_t>(globals::EntityList + 0x8 * ((i & 0x7FFF) >> 9) + 0x10);

				if (listEntry) {
					uintptr_t currentController = memory::read<uintptr_t>(listEntry + 0x70 * (i & 0x1FF));

					if (currentController) {
						char name[128] = {};
						memory::read_array<char>(currentController + offsets::client_dll::CBasePlayerController::m_iszPlayerName, name, sizeof(name));

						int pawnHandle = memory::read<int>(currentController + offsets::client_dll::CCSPlayerController::m_hPlayerPawn);

						if (pawnHandle != 0) {
							uintptr_t listEntry2 = memory::read<uintptr_t>(globals::EntityList + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 0x10);

							if (listEntry2) {
								uintptr_t currentPawn = memory::read<uintptr_t>(listEntry2 + 0x70 * (pawnHandle & 0x1FF));

								if (currentPawn == globals::LocalPlayerPawn) continue;

								if (currentPawn) {
									uintptr_t sceneNode = memory::read<uintptr_t>(currentPawn + offsets::client_dll::C_BaseEntity::m_pGameSceneNode);
									uint8_t team = memory::read<uint8_t>(currentPawn + offsets::client_dll::C_BaseEntity::m_iTeamNum);
									int32_t health = memory::read<int32_t>(currentPawn + offsets::client_dll::C_BaseEntity::m_iHealth);
									int32_t armor = memory::read<int32_t>(currentPawn + offsets::client_dll::C_CSPlayerPawn::m_ArmorValue);
									Vec3 old_origin = memory::read<Vec3>(currentPawn + offsets::client_dll::CBasePlayerPawn::m_vOldOrigin);

									if (health <= 0 || health >= 1337) continue;

									Vec3 viewOffset = memory::read<Vec3>(currentPawn + offsets::client_dll::C_BaseModelEntity::m_vecViewOffset);
									Vec3 vecViewOrigin = memory::read<Vec3>(sceneNode + offsets::client_dll::CGameSceneNode::m_vecOrigin);

									uintptr_t boneMatrix = memory::read<uintptr_t>(sceneNode + offsets::client_dll::CSkeletonInstance::m_modelState + 0x80);

									if (boneMatrix) {
										Vec3 head = memory::read<Vec3>(boneMatrix + bones::head * 32);

										globals::g_espEntityUpdate.emplace_back(esp_entity{
											vecViewOrigin, head, std::string(name), team, health, armor, boneMatrix, old_origin
											}
										);
									}
								}
							}
						}
					}
				}
			}

			std::lock_guard<std::mutex> lock(globals::esp_mutex);
			std::swap(globals::g_espEntityRender, globals::g_espEntityUpdate);
		}
	}
}