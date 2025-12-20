#include "../headers/hacks.h"

void hacks::VisualsThread() noexcept {
	globals::g_espEntityUpdate.reserve(64);

	while (true) {
		globals::LocalPlayerPawn = memory::read<uintptr_t>(globals::client + offsets::client_dll::dwLocalPlayerPawn);
		globals::EntityList = memory::read<uintptr_t>(globals::client + offsets::client_dll::dwEntityList);
		globals::old_origin = memory::read<Vec3>(globals::LocalPlayerPawn + offsets::client_dll::CBasePlayerPawn::m_vOldOrigin);
		globals::local_team = memory::read<uint8_t>(globals::LocalPlayerPawn + offsets::client_dll::C_BaseEntity::m_iTeamNum);

		if (globals::enableEsp) {
			globals::g_espEntityUpdate.clear();
			for (int i = 0; i < 64; ++i) {
				uintptr_t listEntry = memory::read<uintptr_t>(globals::EntityList + 0x8 * ((i & 0x7FFF) >> 9) + 0x10);

				if (listEntry) {
					uintptr_t currentController = memory::read<uintptr_t>(listEntry + 0x70 * (i & 0x1FF));

					if (currentController) {

						int pawnHandle = memory::read<int>(currentController + offsets::client_dll::CCSPlayerController::m_hPlayerPawn);

						if (pawnHandle != 0) {
							uintptr_t listEntry2 = memory::read<uintptr_t>(globals::EntityList + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 0x10);

							if (listEntry2) {
								uintptr_t currentPawn = memory::read<uintptr_t>(listEntry2 + 0x70 * (pawnHandle & 0x1FF));

								if (currentPawn == globals::LocalPlayerPawn) continue;

								if (currentPawn) {
									int32_t health = memory::read<int32_t>(currentPawn + offsets::client_dll::C_BaseEntity::m_iHealth);
									if (health <= 0 || health >= 1337) continue;

									esp_entity ent{};

									ent.health = health;
									ent.team = memory::read<uint8_t>(currentPawn + offsets::client_dll::C_BaseEntity::m_iTeamNum);
									ent.armor = memory::read<int32_t>(currentPawn + offsets::client_dll::C_CSPlayerPawn::m_ArmorValue);
									ent.old_origin = memory::read<Vec3>(currentPawn + offsets::client_dll::CBasePlayerPawn::m_vOldOrigin);

									uintptr_t sceneNode = memory::read<uintptr_t>(currentPawn + offsets::client_dll::C_BaseEntity::m_pGameSceneNode);

									if (sceneNode) {
										ent.origin = memory::read<Vec3>(sceneNode + offsets::client_dll::CGameSceneNode::m_vecOrigin);
										uintptr_t boneMatrix = memory::read<uintptr_t>(sceneNode + offsets::client_dll::CSkeletonInstance::m_modelState + 0x80);

										if (boneMatrix) {
											for (int b = 0; b < 28; ++b) {
												ent.bones[b] = memory::read<Vec3>(boneMatrix + b * 32);

												ent.boneValid[b] = false;
												ent.boneScreen[b] = ImVec2(0.0f, 0.0f);
											}

											memory::read_array<char>(currentController + offsets::client_dll::CBasePlayerController::m_iszPlayerName, ent.name, sizeof(ent.name));
											globals::g_espEntityUpdate.emplace_back(ent);
										}
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
