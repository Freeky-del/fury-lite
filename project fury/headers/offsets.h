#pragma once
#include <cstddef>

namespace offsets {
	namespace client_dll {
		constexpr std::ptrdiff_t dwEntityList = 0x24B3268;
		constexpr std::ptrdiff_t dwLocalPlayerController = 0x22F8028;
		constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x206D9E0;
		constexpr std::ptrdiff_t dwViewMatrix = 0x2313F10;
		constexpr std::ptrdiff_t dwPlantedC4 = 0x231BAB0;
		constexpr std::ptrdiff_t dwWeaponC4 = 0x229D2B0;

		namespace CBasePlayerController {
			constexpr std::ptrdiff_t m_iszPlayerName = 0x6F8; // char[128]
		}

		namespace CBasePlayerPawn {
			constexpr std::ptrdiff_t m_vOldOrigin = 0x1588; // Vector
		}

		namespace CCSPlayerController {
			constexpr std::ptrdiff_t m_hPlayerPawn = 0x90C; // CHandle<C_CSPlayerPawn>
		}

		namespace C_CSPlayerPawn {
			constexpr std::ptrdiff_t m_ArmorValue = 0x272C; // int32
		}

		namespace C_BaseEntity{
			constexpr std::ptrdiff_t m_pGameSceneNode = 0x338; // CGameSceneNode*
			constexpr std::ptrdiff_t m_iTeamNum = 0x3F3; // uint8
			constexpr std::ptrdiff_t m_iHealth = 0x354; // int32
		}

		namespace C_BaseModelEntity {
			constexpr std::ptrdiff_t m_vecViewOffset = 0xD58; // CNetworkViewOffsetVector
		}

		namespace CGameSceneNode {
			constexpr std::ptrdiff_t m_vecOrigin = 0x88; // CNetworkOriginCellCoordQuantizedVector
		}

		namespace CSkeletonInstance {
			constexpr std::ptrdiff_t m_modelState = 0x160; // CModelState
		}
		
		namespace C_C4 {
			constexpr std::ptrdiff_t m_bStartedArming = 0x1F48; // bool
			constexpr std::ptrdiff_t m_bBombPlanted = 0x1F7B; // bool
		}

		namespace C_PlantedC4 {
			constexpr std::ptrdiff_t m_bBombTicking = 0x1170; // bool
			constexpr std::ptrdiff_t m_nBombSite = 0x1174; // int32
			constexpr std::ptrdiff_t m_bCannotBeDefused = 0x11A4; // bool
			constexpr std::ptrdiff_t m_bHasExploded = 0x11A5; // bool
			constexpr std::ptrdiff_t m_bBombDefused = 0x11C4; // bool
			constexpr std::ptrdiff_t m_flTimerLength = 0x11A8; // float32
		}
	}

	namespace engine2_dll {
		constexpr std::ptrdiff_t dwWindowHeight = 0x90D99C;
		constexpr std::ptrdiff_t dwWindowWidth = 0x90D998;
	}
}