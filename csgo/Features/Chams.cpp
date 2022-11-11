#include "Chams.h"

#include "../SDK/KeyValues.h"

#include "../Utilities/Interfaces.h"
#include "../Hooks/Hooks.h"
#include "../Utilities/Client.h"

Features::CChams::CChams() {
	//materialRegular = Interfaces::m_pMaterialSystem->FindMaterial("debug/debugambientcube");
	//materialFlat = Interfaces::m_pMaterialSystem->FindMaterial("debug/debugdrawflat");
}

Features::CChams::~CChams() {
}


void Features::CChams::OverrideMaterial(bool ignoreZ, bool flat, bool wireframe, bool glass, const Color& rgba) {
	IMaterial* material = nullptr;

	if (flat) {
		material = materialFlat;
	}
	else {
		material = materialRegular;
	}

	material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, ignoreZ);


	if (glass) {
		material = materialFlat;
		material->AlphaModulate(0.45f);
	}
	else {
		material->AlphaModulate(
			rgba.Get<COLOR_A>() / 255.0f);
	}

	material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireframe);
	material->ColorModulate(
		rgba.Get<COLOR_R>() / 255.0f,
		rgba.Get<COLOR_G>() / 255.0f,
		rgba.Get<COLOR_B>() / 255.0f);

	Interfaces::m_pModelRender->ForcedMaterialOverride(material);
}


void Features::CChams::OnDrawModelExecute(
	IMatRenderContext* ctx,
	const DrawModelState_t& state,
	const ModelRenderInfo_t& info,
	matrix3x4_t* matrix)
{
	static auto fnDME = Hooks::Managers::m_DrawModelExecute.GetOriginal<decltype(&Hooks::hkDrawModelExecute)>();

	const auto mdl = info.pModel;

	bool is_arm = strstr(mdl->szName, "arms") != nullptr;
	bool is_player = strstr(mdl->szName, "models/player") != nullptr;
	bool is_sleeve = strstr(mdl->szName, "sleeve") != nullptr;
	//bool is_weapon = strstr(mdl->szName, "weapons/v_")  != nullptr;

	if (is_player) {//&& g_Options.chams_player_enabled) {
		// 
		// Draw player Chams.
		// 
		auto ent = (CPlayer*)Interfaces::m_pEntityList->GetClientEntity(info.entity_index);

		if (ent && Client::m_pLocal && ent->IsAlive()) {
			auto enemy = ent->IsEnemy(); //ent->m_iTeamNum() != Client::m_pLocal->m_iTeamNum();
			if (!enemy)//&& g_Options.chams_player_enemies_only)
				return;

			//const auto clr_front = enemy ? g_Options.color_chams_player_enemy_visible : g_Options.color_chams_player_ally_visible;
			//const auto clr_back = enemy ? g_Options.color_chams_player_enemy_occluded : g_Options.color_chams_player_ally_occluded;

			//if (g_Options.chams_player_ignorez) {
			OverrideMaterial(
				true, true, true,
				//g_Options.chams_player_flat,
				//g_Options.chams_player_wireframe,
				false,
				Color(0, 255, 0, 255));
			fnDME(Interfaces::m_pModelRender, 0, ctx, state, info, matrix);
			OverrideMaterial(
				false, true, true,
				//g_Options.chams_player_flat,
				//g_Options.chams_player_wireframe,
				false,
				Color(255, 0, 0, 255));
		}
		else {
			OverrideMaterial(
				false, true, true, false,
				Color(255, 0, 0, 255));
		}
	}
}