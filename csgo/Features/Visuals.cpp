#include "Visuals.h"

#include "../Utilities/Render.h"
#include "../Utilities/Client.h"
#include "../Utilities/Interfaces.h"
#include "../Utilities/Math.h"

#include "../SDK/CEntity.h"
#include "../SDK/ClientClass.h"

#include "../Menu/Config/Vars.h"

#include <vector>
#include "../Utilities/Utilities.h"
#include "../Utilities/InputSystem.h"

bool GetBoundingBox(CPlayer* pPlayer, Box_t* pBox)
{
	const ICollideable* pCollideable = pPlayer->GetCollideable();
	if (!pCollideable)
		return false;

	const Vector vecMin = pCollideable->OBBMins();
	const Vector vecMax = pCollideable->OBBMaxs();

	Vector vecPoints[8] =
	{
		Vector(vecMin.x, vecMin.y, vecMin.z),
		Vector(vecMin.x, vecMax.y, vecMin.z),
		Vector(vecMax.x, vecMax.y, vecMin.z),
		Vector(vecMax.x, vecMin.y, vecMin.z),
		Vector(vecMax.x, vecMax.y, vecMax.z),
		Vector(vecMin.x, vecMax.y, vecMax.z),
		Vector(vecMin.x, vecMin.y, vecMax.z),
		Vector(vecMax.x, vecMin.y, vecMax.z)
	};

	const matrix3x4_t& matTransformed = pPlayer->m_rgflCoordinateFrame();

	float flLeft = FLT_MAX;
	float flTop = FLT_MAX;
	float flRight = FLT_MIN;
	float flBottom = FLT_MIN;

	ImVec2 vecScreen[8] = {};
	for (int i = 0; i < 8; i++)
	{
		if (!Renderer::WorldToScreen(Math::VectorTransform(vecPoints[i], matTransformed), vecScreen[i]))
			return false;

		flLeft = min(flLeft, vecScreen[i].x);
		flTop = min(flTop, vecScreen[i].y);
		flRight = max(flRight, vecScreen[i].x);
		flBottom = max(flBottom, vecScreen[i].y);
	}

	pBox->m_flLeft = flLeft;
	pBox->m_flTop = flTop;
	pBox->m_flRight = flRight;
	pBox->m_flBottom = flBottom;
	pBox->m_flWidth = flRight - flLeft;
	pBox->m_flHeight = flBottom - flTop;
	return true;
}

void Features::Visuals::Players::RenderBox(CPlayer* pPlayer)
{
	Box_t Box;
	GetBoundingBox(pPlayer, &Box);

	if (pPlayer->IsEnemy() && Vars::Visuals::Players::Enemies::m_bBoxEsp)
	{
		Renderer::Rect(ImVec2(Box.m_flLeft, Box.m_flTop), ImVec2(Box.m_flRight, Box.m_flBottom),
			Vars::Visuals::Players::Enemies::m_colBoxEsp.GetColor());
	}
	else if (!pPlayer->IsEnemy() && pPlayer->GetIndex() != Client::m_pLocal->GetIndex()
		&& Vars::Visuals::Players::Teammates::m_bBoxEsp)
	{
		Renderer::Rect(ImVec2(Box.m_flLeft, Box.m_flTop), ImVec2(Box.m_flRight, Box.m_flBottom),
			Vars::Visuals::Players::Teammates::m_colBoxEsp.GetColor());
	}
}

void Features::Visuals::Players::RenderFlags(CPlayer* pPlayer)
{
	Box_t Box;
	GetBoundingBox(pPlayer, &Box);

	float flFontSize = m_flFontSizeScaled[pPlayer->GetIndex()];

	if (!pPlayer->IsEnemy())
	{
		if (pPlayer->GetIndex() == Client::m_pLocal->GetIndex())
			return;

		if (Vars::Visuals::Players::Teammates::m_bPlayerName)
		{
			std::string szPlayerName = pPlayer->GetPlayerName();
			Renderer::Text(szPlayerName, ImVec2(Box.m_flLeft + (Box.m_flRight - Box.m_flLeft) * 0.5f, Box.m_flTop - 17),
				12.f, Vars::Visuals::Players::Teammates::m_colName.GetColor());
		}

		if (Vars::Visuals::Players::Teammates::m_bWeaponName)
		{
			const char* szWeaponName = Utilities::GetWeaponName(pPlayer->GetActiveWeapon()->m_iItemDefinitionIndex());
			Renderer::Text(szWeaponName, ImVec2(Box.m_flLeft + (Box.m_flRight - Box.m_flLeft) * 0.5f, Box.m_flBottom - 17),
				12.f, Vars::Visuals::Players::Teammates::m_colWeapon.GetColor());
		}

		if (Vars::Visuals::Players::Teammates::m_bWeaponIcon)
		{
			const char8_t* szWeaponIcon = Utilities::GetWeaponIcon(pPlayer->GetActiveWeapon()->m_iItemDefinitionIndex());
			Renderer::Text((const char*)szWeaponIcon, ImVec2(Box.m_flLeft + (Box.m_flRight - Box.m_flLeft) * 0.5f, Box.m_flBottom - 17),
				12.f, Vars::Visuals::Players::Teammates::m_colWeapon.GetColor());
		}

		std::vector<std::pair<std::string, Color>> vecFlags = {};

		if (Vars::Visuals::Players::Teammates::m_bHasC4 && pPlayer->HasC4())
			vecFlags.emplace_back("BOMB", Color(255, 0, 0, 255));

		if (Vars::Visuals::Players::Teammates::m_bHealthFlag && pPlayer->IsAlive())
			vecFlags.emplace_back(Utilities::Format("Health: %i", pPlayer->m_iHealth()),
				Utilities::GetHealthColor(pPlayer->m_iHealth()));

		ImVec2 vecOut;
		if (Renderer::WorldToScreen(pPlayer->GetEyePos(), vecOut))
		{
			ImVec2 Pos = ImVec2(vecOut.x + 30.f, vecOut.y + 30.f);

			for (auto& Flag : vecFlags)
			{
				Renderer::Text(Flag.first, Pos, 12.f, Flag.second);
				Pos.y += Fonts::m_pDefault->FontSize + 2.f;
			}
		}
	}
	else if (pPlayer->IsEnemy())
	{
		if (Vars::Visuals::Players::Enemies::m_bPlayerName)
		{
			std::string szPlayerName = pPlayer->GetPlayerName();
			Renderer::Text(szPlayerName, ImVec2(Box.m_flLeft + (Box.m_flRight - Box.m_flLeft) * 0.5f, Box.m_flTop - 17),
				flFontSize, Vars::Visuals::Players::Enemies::m_colName.GetColor());
		}

		if (Vars::Visuals::Players::Enemies::m_bWeaponName)
		{
			const char* szWeaponName = Utilities::GetWeaponName(pPlayer->GetActiveWeapon()->m_iItemDefinitionIndex());
			Renderer::Text(szWeaponName, ImVec2(Box.m_flLeft + (Box.m_flRight - Box.m_flLeft) * 0.5f, Box.m_flBottom - 17),
				flFontSize, Vars::Visuals::Players::Enemies::m_colWeapon.GetColor());
		}

		if (Vars::Visuals::Players::Enemies::m_bWeaponIcon)
		{
			const char8_t* szWeaponIcon = Utilities::GetWeaponIcon(pPlayer->GetActiveWeapon()->m_iItemDefinitionIndex());
			Renderer::Text((const char*)szWeaponIcon, ImVec2(Box.m_flLeft + (Box.m_flRight - Box.m_flLeft) * 0.5f, Box.m_flBottom - 17),
				12.f, Vars::Visuals::Players::Enemies::m_colWeapon.GetColor());
		}

		std::vector<std::pair<std::string, Color>> vecFlags = {};

		if (Vars::Visuals::Players::Enemies::m_bHasC4 && pPlayer->HasC4())
			vecFlags.emplace_back("BOMB", Color(255, 0, 0, 255));

		if (Vars::Visuals::Players::Enemies::m_bHealthFlag && pPlayer->IsAlive())
			vecFlags.emplace_back(Utilities::Format("Health: %i", pPlayer->m_iHealth()),
				Utilities::GetHealthColor(pPlayer->m_iHealth()));

		ImVec2 Pos = ImVec2(Box.m_flRight + 17.f, Box.m_flTop + 17.f);
		for (auto& Flag : vecFlags)
		{
			Renderer::Text(Flag.first, Pos, flFontSize, Flag.second);
			Pos.y += Fonts::m_pDefault->FontSize + 2.f;
		}
	}
}

void Features::Visuals::Players::RenderSnapLines(CPlayer* pPlayer)
{
	if (Vars::Visuals::Players::Enemies::m_nSnapLines > 0 && pPlayer->IsEnemy())
	{
		ImVec2 vecScreen;
		switch (Vars::Visuals::Players::Enemies::m_nSnapLines)
		{
		case 0: // None
			break;
		case 1: // Head
			if (!Renderer::WorldToScreen(pPlayer->GetEyePos(), vecScreen))
				return;
			break;
		case 2: // Origin
			if (!Renderer::WorldToScreen(pPlayer->m_vecOrigin(), vecScreen))
				return;
			break;
		}

		Renderer::Line(ImVec2(Client::m_nScreenSizeX / 2, Client::m_nScreenSizeY),
			ImVec2(vecScreen.x, vecScreen.y), Vars::Visuals::Players::Enemies::m_colSnapLines.GetColor());
	}
	else if (Vars::Visuals::Players::Teammates::m_nSnapLines > 0 && !pPlayer->IsEnemy()
		&& pPlayer->GetIndex() != Client::m_pLocal->GetIndex())
	{
		ImVec2 vecScreen;
		switch (Vars::Visuals::Players::Teammates::m_nSnapLines)
		{
		case 0: // None
			break;
		case 1: // Head
			if (!Renderer::WorldToScreen(pPlayer->GetEyePos(), vecScreen))
				return;
			break;
		case 2: // Origin
			if (!Renderer::WorldToScreen(pPlayer->m_vecOrigin(), vecScreen))
				return;
			break;
		}

		Renderer::Line(ImVec2(Client::m_nScreenSizeX / 2, Client::m_nScreenSizeY),
			ImVec2(vecScreen.x, vecScreen.y), Vars::Visuals::Players::Teammates::m_colSnapLines.GetColor());
	}
}

void Features::Visuals::Players::Glow()
{
	for (int i = 0; i < Interfaces::m_pGlowManager->vecGlowObjectDefinitions.Count(); i++)
	{
		IGlowObjectManager::GlowObject_t& Obj = Interfaces::m_pGlowManager->vecGlowObjectDefinitions[i];

		// Is current object not used
		if (Obj.IsEmpty())
			continue;

		CEntity* pEntity = Obj.pEntity;
		if (!pEntity)
			continue;

		auto pClientClass = pEntity->GetClientClass();

		switch (pClientClass->m_ClassID)
		{
		case ClassId_CCSPlayer:
			CPlayer* pPlayer = (CPlayer*)pEntity;

			if (pPlayer->IsDormant() || !pPlayer->IsAlive())
				break;

			if (pPlayer->IsEnemy() && Vars::Visuals::Players::Enemies::m_bGlow)
			{
				Obj.Set(Vars::Visuals::Players::Enemies::m_colGlow.GetColor());
			}

			if (!pPlayer->IsEnemy() && pPlayer->GetIndex() != Client::m_pLocal->GetIndex()
				&& Vars::Visuals::Players::Teammates::m_bGlow)
			{
				Obj.Set(Vars::Visuals::Players::Teammates::m_colGlow.GetColor());
			}
			break;
		}
	}
}

void Features::Visuals::Screen::RenderCrosshair()
{
	if (!Client::m_pLocal)
		return;

	if (!Client::m_pLocal->IsAlive())
		return;

	// Crosshair size, turn this into a checkbox?
	int nMoveCross = 8;

	switch (Vars::Visuals::Screen::m_nCrosshairMode)
	{
	case 0: // Normal / ingame crosshair
	{
		break;
	}
	case 1: // Static
	{
		int x = Client::m_nScreenSizeX;
		int y = Client::m_nScreenSizeY;
		x /= 2;
		y /= 2;

		// Outline
		Renderer::Line(x - nMoveCross, y, x + nMoveCross, y, Color(0, 0, 0, 255), 3.f);
		Renderer::Line(x, y - nMoveCross, x, y + nMoveCross, Color(0, 0, 0, 255), 3.f);

		nMoveCross -= 1.5f;

		Renderer::Line(x - nMoveCross, y, x + nMoveCross, y, Color(255, 255, 255, 255), 1.f);
		Renderer::Line(x, y - nMoveCross, x, y + nMoveCross, Color(255, 255, 255, 255), 1.f);

		break;
	}
	case 2: // Recoil
		auto GetPunchPos = []() -> Vector
		{
			QAngle va;
			Interfaces::m_pEngine->GetViewAngles(&va);

			const static float flScale = 2.f; // weapon_recoil_scale cvar
			va += Client::m_pLocal->m_angPunchAngle() * flScale;

			Vector vecViewAngle;
			Math::AngleVectors(va, vecViewAngle);

			Vector vecStart = Client::m_pLocal->GetEyePos();
			Vector vecEnd = vecStart + vecViewAngle * Client::m_pLocal->GetActiveWeapon()->GetWeaponData()->flRange;

			return vecEnd;
		};

		ImVec2 vecScreen;
		if (Renderer::WorldToScreen(GetPunchPos(), vecScreen))
		{
			int x, y;
			// When attacking, use recoil position
			if (Client::m_pCmd->m_nButtons & IN_ATTACK)
			{
				x = vecScreen.x;
				y = vecScreen.y;
			}
			// Other wise use static crosshair
			else
			{
				x = Client::m_nScreenSizeX;
				y = Client::m_nScreenSizeY;
				x /= 2;
				y /= 2;
			}

			// Outline
			Renderer::Line(x - nMoveCross, y, x + nMoveCross, y, Color(0, 0, 0, 255), 3.f);
			Renderer::Line(x, y - nMoveCross, x, y + nMoveCross, Color(0, 0, 0, 255), 3.f);

			nMoveCross -= 1.5f;

			Renderer::Line(x - nMoveCross, y, x + nMoveCross, y, Color(255, 255, 255, 255), 1.f);
			Renderer::Line(x, y - nMoveCross, x, y + nMoveCross, Color(255, 255, 255, 255), 1.f);
		}
	{break; };
	}
}

void Features::Visuals::Screen::RenderNoScope()
{
	if (!Vars::Visuals::Screen::m_bNoScope)
		return;

	if (!Client::m_pLocal)
		return;

	auto pWeapon = Client::m_pLocal->GetActiveWeapon();
	if (!pWeapon)
		return;

	if (!(pWeapon->GetWeaponData()->iWeaponType == WEAPONTYPE_SNIPER))
		return;

	if (Client::m_pLocal->m_bIsScoped())
	{
		int x = Client::m_nScreenSizeX;
		int y = Client::m_nScreenSizeY;

		Renderer::Line(x / 2, 0, x / 2, y, Vars::Visuals::Screen::m_colNoScope.GetColor(), 
			Vars::Visuals::Screen::m_flNoScopeThickness);

		Renderer::Line(0, y / 2, x, y / 2, Vars::Visuals::Screen::m_colNoScope.GetColor(),
			Vars::Visuals::Screen::m_flNoScopeThickness);
	}
}

void Features::Visuals::MirrorCam::Initialize()
{
	m_pTexture = Interfaces::m_pMaterialSystem->CreateFullFrameRenderTarget("mirrorcam");

	m_bInitialized = true;
}

void Features::Visuals::MirrorCam::Draw()
{
	if (!m_bInitialized)
		return;

	if (!Vars::Visuals::Misc::m_bMirrorCam)
		return;

	if (!Interfaces::m_pEngine->IsInGame())
		return;

	if (ImGui::Begin("Mirror Cam", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar 
		| ImGuiWindowFlags_NoScrollWithMouse))
	{
		if (auto size = ImGui::GetContentRegionAvail(); size.x != 0.0f && size.y != 0.0f) // / 0
		{
			float xRatio = static_cast<float>(m_pTexture->GetActualWidth()) / size.x;
			float yRatio = static_cast<float>(m_pTexture->GetActualHeight()) / size.y;

			m_vecSize = Vector2D(size.x * xRatio, size.y * yRatio);
			ImGui::Image(GetTexture(), size);
		}

		ImGui::End();
	}
}

#include "../Hooks/Hooks.h"

void Features::Visuals::MirrorCam::RenderCustomView(const CViewSetup& View)
{
	if (!Vars::Visuals::Misc::m_bMirrorCam)
		return;

	CViewSetup v = std::move(View);

	v.m_angView.yaw += 180.f; // Set angle backwards
	v.m_nX = v.m_nUnscaledX;
	v.m_nY = v.m_nUnscaledY;
	v.m_nWidth = v.m_nUnscaledWidth = static_cast<int>(m_vecSize.x);
	v.m_nHeight = v.m_nUnscaledHeight = static_cast<int>(m_vecSize.y);
	v.m_flAspectRatio = float(v.m_nWidth / v.m_nHeight);
	v.m_flNearZ = v.m_flNearViewmodelZ = 7.f;
	v.m_flFOV = 50.f;

	auto pCtx = Interfaces::m_pMaterialSystem->GetRenderContext();
	pCtx->PushRenderTargetAndViewport();
	pCtx->SetRenderTarget(m_pTexture);

	Hooks::Managers::m_RenderView.GetOriginal<decltype(&Hooks::hkRenderView)>()(Interfaces::m_pViewRender, 0, v, v,
		VIEW_CLEAR_COLOR | VIEW_CLEAR_DEPTH | VIEW_CLEAR_STENCIL, 0);

	pCtx->PopRenderTargetAndViewport();
	pCtx->Release();
}

IDirect3DTexture9* Features::Visuals::MirrorCam::GetTexture()
{
	return m_pTexture->m_ppHandle[0]->m_pTexture;
}

int g_nAttackTime = 0;
void Features::Visuals::DrawHitMarker()
{
	if (!Vars::Visuals::Screen::m_bHitMarker)
		return;

	const float SIZE = 8.f; // TODO: Make this a customizable thing
	const int INTERVAL = 250; // TODO: Make this a customizable thing

	if (g_nAttackTime + 250 >= GetTickCount())
	{
		int x = Client::m_nScreenSizeX / 2;
		int y = Client::m_nScreenSizeY / 2;

		Renderer::Line(x - SIZE, y - SIZE, x - (SIZE / 4), y - (SIZE / 4), Vars::Visuals::Screen::m_colHitMarker.GetColor());
		Renderer::Line(x - SIZE, y + SIZE, x - (SIZE / 4), y + (SIZE / 4), Vars::Visuals::Screen::m_colHitMarker.GetColor());
		Renderer::Line(x + SIZE, y + SIZE, x + (SIZE / 4), y + (SIZE / 4), Vars::Visuals::Screen::m_colHitMarker.GetColor());
		Renderer::Line(x + SIZE, y - SIZE, x + (SIZE / 4), y - (SIZE / 4), Vars::Visuals::Screen::m_colHitMarker.GetColor());
	}
}

void Features::Visuals::OnEvent(IGameEvent* pEvent)
{
	const char* szEvent = pEvent->GetName();
	if (!strcmp(szEvent, "player_hurt"))
	{
		int nAttacker = Interfaces::m_pEngine->GetPlayerForUserID(pEvent->GetInt("attacker"));

		// We're attacking someone
		if (nAttacker == Client::m_pLocal->GetIndex())
			g_nAttackTime = GetTickCount();
	}
}

void Features::Visuals::AddToDrawList()
{
	for (int i = 0; i < 64; i++)
	{
		auto pPlayer = (CPlayer*)Interfaces::m_pEntityList->GetClientEntity(i);

		if (!pPlayer)
			continue;

		switch (pPlayer->GetClientClass()->m_ClassID)
		{
		case ClassId_CCSPlayer:

			if (!pPlayer->IsAlive() || pPlayer->IsDormant())
				break;

			Players::m_flFontSizeScaled[pPlayer->GetIndex()] =
				Renderer::ScaleDistance(Client::m_pLocal->m_vecOrigin().DistTo(pPlayer->m_vecOrigin()));

			Players::RenderBox(pPlayer);
			Players::RenderFlags(pPlayer);
			Players::RenderSnapLines(pPlayer);

			// Engine radar for enemies
			if (pPlayer->IsEnemy() && Vars::Visuals::Players::Enemies::m_bEngineRadar)
				pPlayer->m_bSpottedByMask() = true;

			break;
		}
	}

	Screen::RenderNoScope();
	Screen::RenderCrosshair();

	DrawHitMarker();

	static bool& s_bOverridePostProcessingDisable = **(bool**)(Utilities::ScanSignature("client.dll", "83 EC 4C 80 3D") + 5);
	s_bOverridePostProcessingDisable = Vars::Visuals::Screen::m_bRemovePostProcessing;
}