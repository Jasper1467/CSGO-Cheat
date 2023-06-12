#include "Hooks.h"

#include "../Utilities/Interfaces.h"
#include "../Utilities/Render.h"
#include "../Utilities/Utilities.h"

#include "../Menu/Menu.h"

#include "../Dependencies/imgui/imgui.h"
#include "../Dependencies/imgui/impl/imgui_impl_dx9.h"
#include "../Dependencies/imgui/impl/imgui_impl_win32.h"

#include <intrin.h>
#include "../Utilities/InputSystem.h"
#include "../Utilities/Client.h"
#include "../Menu/Config/Vars.h"
#include "../Utilities/Math.h"

#include "../SDK/CEntity.h"
#include "../SDK/Effects.h"

#include "../Features/Misc.h"
#include "../Features/AntiAim.h"
#include "../Features/Events.h"
#include "../Features/Visuals.h"
#include "../Features/LegitBot.h"
#include "../Features/Commands.h"
#include "../Features/Chams.h"

#include "../Utilities/Profiler.h"
#include "../CompilerDefinitions.h"

template <typename T = void*>
constexpr T ____GetVFunc(void* thisptr, std::size_t nIndex)
{
	return (*static_cast<T**>(thisptr))[nIndex];
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

bool Hooks::Initialize()
{
	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize minhook");

	// ValidAddr Fixes
	{
		const char* szValidAddrSig = "55 8B EC 56 8B F1 33 C0 57 8B 7D 08 8B 8E";

		if (!Managers::m_ClientValidAddr.Create(SCAN_SIGNATURE("client.dll", szValidAddrSig), &hkClientValidAddr))
			return false;

		if (!Managers::m_EngineValidAddr.Create(SCAN_SIGNATURE("engine.dll", szValidAddrSig), &hkEngineValidAddr))
			return false;

		if (!Managers::m_StudioRenderValidAddr.Create(SCAN_SIGNATURE("studiorender.dll", szValidAddrSig), &hkStudioRenderValidAddr))
			return false;

		if (!Managers::m_MaterialSystemValidAddr.Create(SCAN_SIGNATURE("materialsystem.dll", szValidAddrSig),
			&hkMaterialSystemValidAddr))
			return false;
	}

	/*if (!Managers::m_UnkFileCheck.Create(____GetVFunc(Interfaces::m_pFileSystem, UNK_FILE_CHECK_INDEX), &hkUnkFileCheck))
		return false;

	if (!Managers::m_UnverifiedFileHashes.Create(____GetVFunc(Interfaces::m_pFileSystem, UNVERIFIED_FILE_HASHES_INDEX),
		&hkGetUnverifiedFileHashes))
		return false;*/

	if (!Managers::m_Reset.Create(____GetVFunc(Interfaces::m_pDevice, RESET_INDEX), hkReset))
		return false;

	if (!Managers::m_EndScene.Create(____GetVFunc(Interfaces::m_pDevice, END_SCENE_INDEX), hkEndScene))
		return false;

#ifdef PROFILER
	Profiler::m_CreateMove.Initialize("CreateMove");
#endif // PROFILER
	if (!Managers::m_CreateMoveProxy.Create(____GetVFunc(Interfaces::m_pClient, CREATE_MOVE_INDEX), &hkCreateMoveProxy))
		return false;

#ifdef PROFILER
	Profiler::m_PaintTraverse.Initialize("PaintTraverse");
#endif // PROFILER
	if (!Managers::m_PaintTraverse.Create(____GetVFunc(Interfaces::m_pPanel, PAINT_TRAVERSE_INDEX), &hkPaintTraverse))
		return false;

	if (!Managers::m_FireEventClientSide.Create(____GetVFunc(Interfaces::m_pGameEventManager, FIRE_EVENT_CLIENT_SIDE_INDEX),
		&hkFireEventClientSide))
		return false;

	if (!Managers::m_OverrideView.Create(____GetVFunc(Interfaces::m_pClientMode, OVERRIDE_VIEW_INDEX), &hkOverrideView))
		return false;

	if (!Managers::m_DoPostScreenEffects.Create(____GetVFunc(Interfaces::m_pClientMode, DO_POST_SCREEN_EFFECTS_INDEX),
		&hkDoPostScreenEffects))
		return false;

	if (!Managers::m_GetColorModulation.Create(SCAN_SIGNATURE("materialsystem.dll", "55 8B EC 83 EC ? 56 8B F1 8A 46"),
		hkGetColorModulation))
		return false;

	if (!Managers::m_IsUsingStaticPropDebugModes.Create(
		(void*)SCAN_SIGNATURE_RELATIVE("engine.dll", "E8 ? ? ? ? 84 C0 8B 45 08", 1), hkIsUsingStaticPropDebugModes))
		return false;

	if (!Managers::m_FrameStageNotify.Create(____GetVFunc(Interfaces::m_pClient, FRAME_STAGE_NOTIFY_INDEX), &hkFrameStageNotify))
		return false;

	if (!Managers::m_ClientCmd_Unrestricted.Create(____GetVFunc(Interfaces::m_pEngine, CLIENT_CMD_UNRESTRICTED_INDEX),
		&hkClientCmd_Unrestricted))
		return false;

	if (!Managers::m_RenderView.Create(____GetVFunc(Interfaces::m_pViewRender, RENDER_VIEW_INDEX), &hkRenderView))
		return false;

	if (!Managers::m_LockCursor.Create(____GetVFunc(Interfaces::m_pSurface, LOCK_CURSOR_INDEX), &hkLockCursor))
		return false;

	//if (!Managers::m_DrawModelExecute.Create(____GetVFunc(Interfaces::m_pModelRender, DRAW_MODEL_EXECUTE_INDEX), &hkDrawModelExecute))
		//return false;

	if (!MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		return false;

	return true;
}

HRESULT __stdcall Hooks::hkWndProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	InputSystem::Process(nMessage, wParam, lParam);

	if (InputSystem::IsKeyReleased(VK_INSERT))
		Menu::m_bIsVisible = !Menu::m_bIsVisible;

	// Disable game input when menu is opened
	Interfaces::m_pInputSystem->EnableInput(!Menu::m_bIsVisible);

	/*
	 * @note: we can use imgui input handler to our binds if remove menu state check
	 * with ImGui::IsKeyDown, ImGui::IsKeyPressed, etc functions
	 * but imgui api's keys down durations doesnt have forward compatibility
	 * and i dont want spend a lot of time on recode it
	 */
	if (Menu::m_bIsVisible && ImGui_ImplWin32_WndProcHandler(hWnd, nMessage, wParam, lParam))
		return TRUE;

	return CallWindowProcA(InputSystem::pOldWndProc, hWnd, nMessage, wParam, lParam);
}

long __stdcall Hooks::hkEndScene(IDirect3DDevice9* pDevice)
{
	static auto oEndScene = Managers::m_EndScene.GetOriginal<decltype(&hkEndScene)>();

	DWORD colorwrite, srgbwrite;
	IDirect3DVertexDeclaration9* vert_dec = nullptr;
	IDirect3DVertexShader9* vert_shader = nullptr;
	DWORD dwOld_D3DRS_COLORWRITEENABLE = NULL;
	pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
	pDevice->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);

	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	//removes the source engine color correction
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

	pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);
	pDevice->GetVertexDeclaration(&vert_dec);
	pDevice->GetVertexShader(&vert_shader);
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);
	
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	auto esp_drawlist = Renderer::RenderScene();

	Menu::Initialize();

	Menu::Render();
	Features::Visuals::MirrorCam::Draw();

	ImGui::Render(esp_drawlist);

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
	pDevice->SetVertexDeclaration(vert_dec);
	pDevice->SetVertexShader(vert_shader);

	return oEndScene(pDevice);
}

long __stdcall Hooks::hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParams)
{
	static auto oReset = Managers::m_Reset.GetOriginal<decltype(&hkReset)>();

	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto hr = oReset(pDevice, pParams);

	if (hr == D3D_OK)
		ImGui_ImplDX9_CreateDeviceObjects();

	return hr;
}

static void __stdcall CreateMove(int nSequenceNumber, float flInputSampleFrametime, bool bIsActive, bool& bSendPacket)
{
#ifdef PROFILER
	Profiler::m_CreateMove.Start();
#endif // PROFILER
	static auto oCreateMove = Hooks::Managers::m_CreateMoveProxy.GetOriginal<decltype(&Hooks::hkCreateMoveProxy)>();

	oCreateMove(Interfaces::m_pClient, 0, nSequenceNumber, flInputSampleFrametime, bIsActive);

	Client::m_pCmd = Interfaces::m_pInput->GetUserCmd(nSequenceNumber);
	if (!Client::m_pCmd)
		printf("Client::m_pCmd is nullptr");

	auto pVerified = Interfaces::m_pInput->GetVerifiedCmd(nSequenceNumber);
	if (!pVerified || !bIsActive)
		return;

	if (Menu::m_bIsVisible)
		Client::m_pCmd->m_nButtons &= ~IN_ATTACK;

	Client::m_pLocal = (CPlayer*)Interfaces::m_pEntityList->GetClientEntity(Interfaces::m_pEngine->GetLocalPlayer());
	if (!Client::m_pLocal)
		printf("Client::m_pLocal is nullptr");

	Client::m_angOldAngles = Client::m_pCmd->m_angViewAngles;

	if (Vars::Misc::m_bInfiniteDuckStamina)
		Client::m_pCmd->m_nButtons |= IN_BULLRUSH;

	/*Features::Misc::BunnyHop();
	Features::Misc::AutoStrafer();
	Features::Misc::ClanTag();

	//Features::GrenadePrediction::Trace();

	Features::LegitBot::TriggerBot();

	Features::AntiAim::Run(bSendPacket);

	Features::Misc::CorrectMovement();*/

	pVerified->m_Cmd = *Client::m_pCmd;
	pVerified->m_nCrc = Client::m_pCmd->GetChecksum();

#ifdef PROFILER
	Profiler::m_CreateMove.End();
#endif // PROFILER
}

__declspec(naked) void __fastcall Hooks::hkCreateMoveProxy(void* ecx, int edx, int nSequenceNumber, float flInputSampleFrametime,
	bool bIsActive)
{
	__asm
	{
		push	ebp
		mov		ebp, esp; // store the stack
		push	ebx; // bSendPacket
		push	esp; // restore the stack
		push	dword ptr[bIsActive]; // ebp + 16
		push	dword ptr[flInputSampleFrametime]; // ebp + 12
		push	dword ptr[nSequenceNumber]; // ebp + 8
		call	CreateMove
			pop		ebx
			pop		ebp
			retn	0Ch
	}
}

void __stdcall Hooks::hkFrameStageNotify(int nStage)
{
	static auto oFrameStageNotify = Managers::m_FrameStageNotify.GetOriginal<decltype(&hkFrameStageNotify)>();

	switch (nStage)
	{
	case FRAME_UNDEFINED:
		break;
	case FRAME_START:
		break;
	case FRAME_NET_UPDATE_START:
		break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
		break;
	case FRAME_NET_UPDATE_END:
		break;
	case FRAME_RENDER_START:
		//if (Vars::Visuals::Misc::m_nThirdPersonKey > 0)
		 {
			static bool bThirdPerson = false;
			if (InputSystem::IsKeyReleased(VK_MBUTTON))
			//if (!InputSystem::IsKeyReleased(Vars::Visuals::Misc::m_nThirdPersonKey))
				bThirdPerson = !bThirdPerson;

			//float cam_idealdist = Interfaces::m_pCVar->FindVar("cam_idealdist")->GetFloat();

			Interfaces::m_pInput->m_cameraInThirdPerson = bThirdPerson && Client::m_pLocal->IsAlive();
			Interfaces::m_pInput->m_cameraOffset.z = bThirdPerson ? 200.f : 150.f;
		}

		break;
	case FRAME_RENDER_END:
		break;
	case FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE:
		break;
	}

	oFrameStageNotify(nStage);
}

void __fastcall Hooks::hkPaintTraverse(void* ecx, int edx, unsigned int nPanel, bool bForceRepaint, bool bAllowForce)
{
#ifdef PROFILER
	Profiler::m_PaintTraverse.Start();
#endif // PROFILER

	static auto oPaintTraverse = Managers::m_PaintTraverse.GetOriginal<decltype(&hkPaintTraverse)>();

	// Setup screen size variables, we use a variable so we can store this once,
	// instead of calling the function often. (better performance)
	Interfaces::m_pEngine->GetScreenSize(Client::m_nScreenSizeX, Client::m_nScreenSizeY);

	const char* szPanelName = Interfaces::m_pPanel->GetName(nPanel);

	// Return, don't draw scope
	if (Vars::Visuals::Screen::m_bNoScope && !strcmp(szPanelName, "HudZoom"))
		return;

	oPaintTraverse(Interfaces::m_pPanel, edx, nPanel, bForceRepaint, bAllowForce);

	static unsigned int nPanelId = 0;
	if (!nPanelId)
	{
		if (!strcmp(szPanelName, "FocusOverlayPanel"))
			nPanelId = nPanel;
	}
	else if (nPanelId == nPanel)
	{
	//	Interfaces::m_pPanel->SetKeyBoardInputEnabled(nPanel, Menu::m_bIsVisible);
		//Interfaces::m_pPanel->SetMouseInputEnabled(nPanel, Menu::m_bIsVisible);

		// Ignore 50% cuz it's called very often
		static bool bFilter = false;
		bFilter = !bFilter;

		if (bFilter)
			return;

		Renderer::BeginScene();
	}

#ifdef PROFILER
	Profiler::m_PaintTraverse.End();
#endif // PROFILER
}

bool __fastcall Hooks::hkFireEventClientSide(void* ecx, int edx, IGameEvent* pEvent)
{
	static auto oFireEventClientSide = Managers::m_FireEventClientSide.GetOriginal<decltype(&hkFireEventClientSide)>();

	Features::Events::OnFireEventClientSide(pEvent);
	Features::Visuals::OnEvent(pEvent);

	return oFireEventClientSide(Interfaces::m_pGameEventManager, 0, pEvent);
}

void __fastcall	Hooks::hkOverrideView(void* ecx, int edx, CViewSetup* pSetup)
{
	static auto oOverrideView = Managers::m_OverrideView.GetOriginal<decltype(&hkOverrideView)>();

	if (Vars::Visuals::Misc::m_bFov && Interfaces::m_pEngine->IsInGame())
		pSetup->m_flFOV += Vars::Visuals::Misc::m_flFov - 50.f;

	oOverrideView(Interfaces::m_pClientMode, 0, pSetup);
}

int __fastcall Hooks::hkDoPostScreenEffects(void* ecx, int edx, CViewSetup* pSetup)
{
	static auto oDoPostScreenEffects = Managers::m_DoPostScreenEffects.GetOriginal<decltype(&hkDoPostScreenEffects)>();

	if (!Interfaces::m_pEngine->IsInGame())
		return oDoPostScreenEffects(ecx, 0, pSetup);

	Features::Visuals::Players::Glow();

	return oDoPostScreenEffects(ecx, 0, pSetup);
}

void __fastcall Hooks::hkClientCmd_Unrestricted(void* ecx, int edx, const char* szCmdString, bool bFromConsoleOrKeyBind)
{
	static auto oClientCmd_Unrestricted = Managers::m_ClientCmd_Unrestricted.GetOriginal<decltype(&hkClientCmd_Unrestricted)>();

	if (!bFromConsoleOrKeyBind)
		return oClientCmd_Unrestricted(Interfaces::m_pEngine, 0, szCmdString, bFromConsoleOrKeyBind);

	// NOTE: some commands crash like "clear", not sure why

#ifdef PRINT_COMMAND_RECIEVED
	printf("[Hooks::hkClientCmd_Unrestricted] %s\n", szCmdString);
#endif // PRINT_COMMAND_RECIEVED

	Features::Commands::Handler(szCmdString);

	oClientCmd_Unrestricted(Interfaces::m_pEngine, 0, szCmdString, bFromConsoleOrKeyBind);
}

void __fastcall Hooks::hkGetColorModulation(void* ecx, int edx, float* r, float* g, float* b)
{
	static auto oGetColorModulation = Managers::m_GetColorModulation.GetOriginal<decltype(&hkGetColorModulation)>();

	oGetColorModulation(ecx, 0, r, g, b);

	const auto pMaterial = (IMaterial*)ecx;

	const char* szName = pMaterial->GetName();
	const char* szGroup = pMaterial->GetTextureGroupName();

	// Exlude stuff we don't want to modulate
	if (strstr(szGroup, "Other") || strstr(szName, "player") || strstr(szName, "chams") || strstr(szName, "weapon")
		|| strstr(szName, "glow"))
		return;

	bool bIsProp = strstr(szGroup, "StaticProp");

	if (Vars::Visuals::World::m_bWorldModulation)
	{
		float flValueR = Vars::Visuals::World::m_colWorldModulation.GetColor().r() / 100.f;
		float flValueG = Vars::Visuals::World::m_colWorldModulation.GetColor().g() / 100.f;
		float flValueB = Vars::Visuals::World::m_colWorldModulation.GetColor().b() / 100.f;

		// Standard value = bIsProp ? 0.5f : 0.2f
		// 0.5f / 3.f = 0.16666...f
		// Good enough
		*r *= bIsProp ? flValueR : (flValueR / 3.f);
		*g *= bIsProp ? flValueG : (flValueG / 3.f);
		*b *= bIsProp ? flValueB : (flValueB / 3.f);
	}

	if (bIsProp && Vars::Visuals::World::m_bPropTransparency)
		pMaterial->AlphaModulate(Vars::Visuals::World::m_flPropTransperancy / 255.f);
}

bool __stdcall Hooks::hkIsUsingStaticPropDebugModes()
{
	// Needs to be true for world color modulation (nightmode / brightness)
	return Vars::Visuals::World::m_bWorldModulation || Vars::Visuals::World::m_bPropTransparency;
}

void __fastcall Hooks::hkStartParticleEffect(const CEffectData& Data, int nSplitScreenSlot)
{
	static auto oStartParticleEffect = Managers::m_StartParticleEffect.GetOriginal<decltype(&hkStartParticleEffect)>();

	oStartParticleEffect(Data, nSplitScreenSlot);
}

int __fastcall Hooks::hkGetUnverifiedFileHashes(void* ecx, int edx, int nMaxFiles)
{
	return 0;
}

int __fastcall Hooks::hkUnkFileCheck(void* ecx, int edx)
{
	return 1; // 2 is kick
}

void __fastcall Hooks::hkRenderView(void* ecx, int edx, const CViewSetup& View, const CViewSetup& Hud, int nClearFlags, int nWhatToDo)
{
	static auto oRenderView = Managers::m_RenderView.GetOriginal<decltype(&hkRenderView)>();

	Features::Visuals::MirrorCam::RenderCustomView(View);

	oRenderView(ecx, 0, View, Hud, nClearFlags, nWhatToDo);
}

void __stdcall Hooks::hkLockCursor()
{
	static auto oLockCursor = Managers::m_LockCursor.GetOriginal<decltype(&hkLockCursor)>();

	// Remove lock when menu is opened
	if (Menu::m_bIsVisible)
	{
		Interfaces::m_pSurface->UnLockCursor();
		return;
	}

	oLockCursor();
}

void __fastcall Hooks::hkDrawModelExecute(void* ecx, int edx, IMatRenderContext* pCtx, const DrawModelState_t& State,
	const ModelRenderInfo_t& Info, matrix3x4_t* pMatrix)
{
	static auto oDrawModelExecute = Managers::m_DrawModelExecute.GetOriginal<decltype(&hkDrawModelExecute)>();

	if (Interfaces::m_pModelRender->IsForcedMaterialOverride() &&
		!strstr(Info.pModel->szName, "arms") &&
		!strstr(Info.pModel->szName, "weapons/v_")) {
		return oDrawModelExecute(ecx, edx, pCtx, State, Info, pMatrix);
	}

	Features::g_Chams.OnDrawModelExecute(pCtx, State, Info, pMatrix);
	oDrawModelExecute(ecx, edx, pCtx, State, Info, pMatrix);
	Interfaces::m_pModelRender->ForcedMaterialOverride(nullptr);
}

char __fastcall Hooks::hkClientValidAddr(void* ecx, int edx, const char* szModuleName)
{
	return 1;
}
char __fastcall Hooks::hkEngineValidAddr(void* ecx, int edx, const char* szModuleName)
{
	return 1;
}
char __fastcall Hooks::hkStudioRenderValidAddr(void* ecx, int edx, const char* szModuleName)
{
	return 1;
}
char __fastcall Hooks::hkMaterialSystemValidAddr(void* ecx, int edx, const char* szModuleName)
{
	return 1;
}