#pragma once
#include <cstddef>

namespace offsets {
	namespace client_dll {
		constexpr std::ptrdiff_t dwEntityList = 0x1D13CE8;
		constexpr std::ptrdiff_t dwLocalPlayerController = 0x1E1DC18;
		constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x1BEEF28;
		constexpr std::ptrdiff_t dwViewMatrix = 0x1E323D0;

		namespace CBasePlayerController {
			constexpr std::ptrdiff_t m_iszPlayerName = 0x6E8; // char[128]
		}

		namespace CBasePlayerPawn {
			constexpr std::ptrdiff_t m_vOldOrigin = 0x15A0; // Vector
		}

		namespace CCSPlayerController {
			constexpr std::ptrdiff_t m_hPlayerPawn = 0x8FC; // CHandle<C_CSPlayerPawn>
		}

		namespace C_CSPlayerPawn {
			constexpr std::ptrdiff_t m_ArmorValue = 0x274C; // int32
		}

		namespace C_BaseEntity{
			constexpr std::ptrdiff_t m_pGameSceneNode = 0x330; // CGameSceneNode*
			constexpr std::ptrdiff_t m_iTeamNum = 0x3EB; // uint8
			constexpr std::ptrdiff_t m_iHealth = 0x34C; // int32
		}

		namespace C_BaseModelEntity {
			constexpr std::ptrdiff_t m_vecViewOffset = 0xD80; // CNetworkViewOffsetVector
		}

		namespace CGameSceneNode {
			constexpr std::ptrdiff_t m_vecOrigin = 0x88; // CNetworkOriginCellCoordQuantizedVector
		}

		namespace CSkeletonInstance {
			constexpr std::ptrdiff_t m_modelState = 0x190; // CModelState
		}
	}

	namespace engine2_dll {
		constexpr std::ptrdiff_t dwWindowHeight = 0x8EF844;
		constexpr std::ptrdiff_t dwWindowWidth = 0x8EF840;
	}
}