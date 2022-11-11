#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "../../Utilities/Color.h"
#include <chrono>
#include "../../Dependencies/imgui/imgui.h"
#include <variant>

#define CONFIG_ADD_VARIABLE(type, var, val) inline type var = val;

namespace Vars
{
	namespace RageBot
	{

	}

	namespace LegitBot
	{
		CONFIG_ADD_VARIABLE(bool, m_bEnabled, false);
		CONFIG_ADD_VARIABLE(bool, m_bTriggerBot, false);
	}

	namespace AntiAim
	{
		CONFIG_ADD_VARIABLE(bool, m_bEnabled, false);

		CONFIG_ADD_VARIABLE(int, m_nPitchMode, 0);

		CONFIG_ADD_VARIABLE(int, m_nFakeLagMode, 0);
		CONFIG_ADD_VARIABLE(int, m_nFakeLagModeStaticAmount, 0);
		CONFIG_ADD_VARIABLE(int, m_nFakeLagRandomMin, 0);
		CONFIG_ADD_VARIABLE(int, m_nFakeLagRandomMax, 0);
	}

	namespace Visuals
	{
		namespace Players
		{
			namespace Enemies
			{
				CONFIG_ADD_VARIABLE(bool, m_bPlayerName, false);
				CONFIG_ADD_VARIABLE(ColorVar, m_colName, ColorVar(ImColor(1.f, 1.f, 1.f, 1.f)));

				CONFIG_ADD_VARIABLE(bool, m_bWeaponName, false);
				CONFIG_ADD_VARIABLE(bool, m_bWeaponIcon, false);
				CONFIG_ADD_VARIABLE(ColorVar, m_colWeapon, ColorVar(ImColor(1.f, 1.f, 1.f, 1.f)));

				CONFIG_ADD_VARIABLE(bool, m_bBoxEsp, false);
				CONFIG_ADD_VARIABLE(ColorVar, m_colBoxEsp, ColorVar(ImColor(1.f, 1.f, 1.f, 1.f)));

				CONFIG_ADD_VARIABLE(int, m_nSnapLines, 0);
				CONFIG_ADD_VARIABLE(ColorVar, m_colSnapLines, ColorVar(ImColor(1.f, 1.f, 1.f, 1.f)));

				CONFIG_ADD_VARIABLE(bool, m_bHasC4, false);
				CONFIG_ADD_VARIABLE(bool, m_bHealthFlag, false);

				CONFIG_ADD_VARIABLE(bool, m_bGlow, false);
				CONFIG_ADD_VARIABLE(ColorVar, m_colGlow, ColorVar(ImColor(1.f, 1.f, 1.f, 1.f)));

				CONFIG_ADD_VARIABLE(bool, m_bEngineRadar, false);
			}

			namespace Teammates
			{
				CONFIG_ADD_VARIABLE(bool, m_bPlayerName, false);
				CONFIG_ADD_VARIABLE(ColorVar, m_colName, ColorVar(ImColor(1.f, 1.f, 1.f, 1.f)));

				CONFIG_ADD_VARIABLE(bool, m_bWeaponName, false);
				CONFIG_ADD_VARIABLE(bool, m_bWeaponIcon, false);
				CONFIG_ADD_VARIABLE(ColorVar, m_colWeapon, ColorVar(ImColor(1.f, 1.f, 1.f, 1.f)));

				CONFIG_ADD_VARIABLE(bool, m_bBoxEsp, false);
				CONFIG_ADD_VARIABLE(ColorVar, m_colBoxEsp, ColorVar(ImColor(1.f, 1.f, 1.f, 1.f)));

				CONFIG_ADD_VARIABLE(int, m_nSnapLines, 0);
				CONFIG_ADD_VARIABLE(ColorVar, m_colSnapLines, ColorVar(ImColor(1.f, 1.f, 1.f, 1.f)));

				CONFIG_ADD_VARIABLE(bool, m_bHasC4, false);
				CONFIG_ADD_VARIABLE(bool, m_bHealthFlag, false);

				CONFIG_ADD_VARIABLE(bool, m_bGlow, false);
				CONFIG_ADD_VARIABLE(ColorVar, m_colGlow, ColorVar(ImColor(1.f, 1.f, 1.f, 1.f)));
			}
		}

		namespace World
		{
			CONFIG_ADD_VARIABLE(bool, m_bWorldModulation, false);
			CONFIG_ADD_VARIABLE(ColorVar, m_colWorldModulation, ColorVar(ImColor(1.f, 1.f, 1.f, 1.f)));

			CONFIG_ADD_VARIABLE(bool, m_bPropTransparency, false);
			CONFIG_ADD_VARIABLE(float, m_flPropTransperancy, 255.f);

			CONFIG_ADD_VARIABLE(bool, m_bRemoveBlood, false);

			CONFIG_ADD_VARIABLE(bool, m_bGrenadePrediction, false);
			CONFIG_ADD_VARIABLE(bool, m_bGrenadePredictionOnClick, false);
		}

		namespace Screen
		{
			CONFIG_ADD_VARIABLE(int, m_nCrosshairMode, 0);

			CONFIG_ADD_VARIABLE(bool, m_bNoScope, false);
			CONFIG_ADD_VARIABLE(float, m_flNoScopeThickness, 1.f);
			CONFIG_ADD_VARIABLE(ColorVar, m_colNoScope, ColorVar(ImColor(0.f, 0.f, 0.f, 1.f)));
			
			CONFIG_ADD_VARIABLE(bool, m_bRemovePostProcessing, false);

			CONFIG_ADD_VARIABLE(bool, m_bEventLogs, false);
			CONFIG_ADD_VARIABLE(ColorVar, m_colEventLogs, ColorVar(ImColor(1.f, 1.f, 1.f, 1.f)));

			CONFIG_ADD_VARIABLE(bool, m_bHitMarker, false);
			CONFIG_ADD_VARIABLE(bool, m_bHitMarkerSound, false);
			CONFIG_ADD_VARIABLE(ColorVar, m_colHitMarker, ColorVar(ImColor(1.f, 1.f, 1.f, 1.f)));
		}

		namespace Misc
		{
			CONFIG_ADD_VARIABLE(bool, m_bFov, false);
			CONFIG_ADD_VARIABLE(float, m_flFov, 90.f);

			CONFIG_ADD_VARIABLE(bool, m_bMirrorCam, false);
		}
	}

	namespace Misc
	{
		CONFIG_ADD_VARIABLE(bool, m_bBunnyHop, false);
		CONFIG_ADD_VARIABLE(int, m_nBunnyHopHitChance, 100);
		
		CONFIG_ADD_VARIABLE(int, m_nAutoStrafer, 0);

		CONFIG_ADD_VARIABLE(bool, m_bInfiniteDuckStamina, false);

		CONFIG_ADD_VARIABLE(bool, m_bClanTag, false);

		CONFIG_ADD_VARIABLE(int, m_nCheatSafety, 0);

		CONFIG_ADD_VARIABLE(ColorVar, m_colMenuAccent, ColorVar(ImColor(1.f, 0.f, 0.f, 1.f)));
	}
}