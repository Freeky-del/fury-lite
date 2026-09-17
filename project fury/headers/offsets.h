#pragma once
#include <cstddef>

namespace offsets {
	namespace client_dll {
		constexpr std::ptrdiff_t dwEntityList = 0x2577BE0;
		constexpr std::ptrdiff_t dwLocalPlayerController = 0x23A78D0;
		constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x23CCC08;
		constexpr std::ptrdiff_t dwPlantedC4 = 0x23973B8;
		constexpr std::ptrdiff_t dwViewMatrix = 0x23D21F0;
		constexpr std::ptrdiff_t dwWeaponC4 = 0x2345728;

		namespace CBasePlayerController {
			constexpr std::ptrdiff_t m_iszPlayerName = 0x6F4; // char[128]
		}

		namespace CBasePlayerPawn {
			constexpr std::ptrdiff_t m_vOldOrigin = 0x13B8; // Vector
		}

		namespace CCSPlayerController {
			constexpr std::ptrdiff_t m_hPlayerPawn = 0x914; // CHandle<C_CSPlayerPawn>
			constexpr std::ptrdiff_t m_bPawnHasDefuser = 0x928; // bool
		}

		namespace C_CSPlayerPawn {
			constexpr std::ptrdiff_t m_ArmorValue = 0x1CA4; // int32
		}

		namespace C_BaseEntity{
			constexpr std::ptrdiff_t m_pGameSceneNode = 0x330; // CGameSceneNode*
			constexpr std::ptrdiff_t m_iTeamNum = 0x3E7; // uint8
			constexpr std::ptrdiff_t m_iHealth = 0x34C; // int32
		}

		namespace C_BaseModelEntity {
			constexpr std::ptrdiff_t m_vecViewOffset = 0xE78; // CNetworkViewOffsetVector
		}

		namespace CGameSceneNode {
			constexpr std::ptrdiff_t m_vecOrigin = 0x80; // CNetworkOriginCellCoordQuantizedVector
		}

		namespace CSkeletonInstance {
			constexpr std::ptrdiff_t m_modelState = 0x140; // CModelState
		}
		
		namespace C_C4 {
			constexpr std::ptrdiff_t m_activeLightParticleIndex = 0x1CE0; // ParticleIndex_t
			constexpr std::ptrdiff_t m_eActiveLightEffect = 0x1CE4; // C4LightEffect_t
			constexpr std::ptrdiff_t m_bStartedArming = 0x1CE8; // bool
			constexpr std::ptrdiff_t m_fArmedTime = 0x1CEC; // GameTime_t
			constexpr std::ptrdiff_t m_bBombPlacedAnimation = 0x1CF0; // bool
			constexpr std::ptrdiff_t m_bIsPlantingViaUse = 0x1CF1; // bool
			constexpr std::ptrdiff_t m_entitySpottedState = 0x1CF8; // EntitySpottedState_t
			constexpr std::ptrdiff_t m_nSpotRules = 0x1D10; // int32
			constexpr std::ptrdiff_t m_bPlayedArmingBeeps = 0x1D14; // bool[7]
			constexpr std::ptrdiff_t m_bBombPlanted = 0x1D1B; // bool
		}

		namespace C_PlantedC4 {
			constexpr std::ptrdiff_t m_bBombTicking = 0x11A0; // bool
			constexpr std::ptrdiff_t m_nBombSite = 0x11A4; // int32
			constexpr std::ptrdiff_t m_nSourceSoundscapeHash = 0x11A8; // int32
			constexpr std::ptrdiff_t m_entitySpottedState = 0x11B0; // EntitySpottedState_t
			constexpr std::ptrdiff_t m_flNextGlow = 0x11C8; // GameTime_t
			constexpr std::ptrdiff_t m_flNextBeep = 0x11CC; // GameTime_t
			constexpr std::ptrdiff_t m_flC4Blow = 0x11D0; // GameTime_t
			constexpr std::ptrdiff_t m_bCannotBeDefused = 0x11D4; // bool
			constexpr std::ptrdiff_t m_bHasExploded = 0x11D5; // bool
			constexpr std::ptrdiff_t m_flTimerLength = 0x11D8; // float32
			constexpr std::ptrdiff_t m_bBeingDefused = 0x11DC; // bool
			constexpr std::ptrdiff_t m_bTriggerWarning = 0x11E0; // float32
			constexpr std::ptrdiff_t m_bExplodeWarning = 0x11E4; // float32
			constexpr std::ptrdiff_t m_bC4Activated = 0x11E8; // bool
			constexpr std::ptrdiff_t m_bTenSecWarning = 0x11E9; // bool
			constexpr std::ptrdiff_t m_flDefuseLength = 0x11EC; // float32
			constexpr std::ptrdiff_t m_flDefuseCountDown = 0x11F0; // GameTime_t
			constexpr std::ptrdiff_t m_bBombDefused = 0x11F4; // bool
			constexpr std::ptrdiff_t m_hBombDefuser = 0x11F8; // CHandle<C_CSPlayerPawn>
			constexpr std::ptrdiff_t m_AttributeManager = 0x1200; // C_AttributeContainer
			constexpr std::ptrdiff_t m_hDefuserMultimeter = 0x16D0; // CHandle<C_Multimeter>
			constexpr std::ptrdiff_t m_flNextRadarFlashTime = 0x16D4; // GameTime_t
			constexpr std::ptrdiff_t m_bRadarFlash = 0x16D8; // bool
			constexpr std::ptrdiff_t m_pBombDefuser = 0x16DC; // CHandle<C_CSPlayerPawn>
			constexpr std::ptrdiff_t m_fLastDefuseTime = 0x16E0; // GameTime_t
			constexpr std::ptrdiff_t m_pPredictionOwner = 0x16E8; // CBasePlayerController*
			constexpr std::ptrdiff_t m_vecC4ExplodeSpectatePos = 0x16F0; // VectorWS
			constexpr std::ptrdiff_t m_vecC4ExplodeSpectateAng = 0x16FC; // QAngle
			constexpr std::ptrdiff_t m_flC4ExplodeSpectateDuration = 0x1708; // float32
		}
	}

	namespace engine2_dll {
		constexpr std::ptrdiff_t dwWindowHeight = 0x912AC4;
		constexpr std::ptrdiff_t dwWindowWidth = 0x912AC0;
	}
}