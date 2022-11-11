#pragma once

#include "DetourHook.h"

#include <d3d9.h>
#include <string>
#include <vector>

class Vector;
class matrix3x4a_t;
class matrix3x4_t;
struct DrawModelInfo_t;
struct DrawModelResults_t;
class IGameEvent;
class CPlayer;
class CUserCmd;
class IMoveHelper;
class CViewSetup;
class CEffectData;
struct DrawModelState_t;
struct ModelRenderInfo_t;
class IMatRenderContext;

namespace Hooks
{
	// Indices
	enum
	{
		// CHLClient
		CREATE_MOVE_INDEX = 22,
		FRAME_STAGE_NOTIFY_INDEX = 37,

		// CPanel
		PAINT_TRAVERSE_INDEX = 41,

		// DirectD3DDevice9
		RESET_INDEX = 16,
		END_SCENE_INDEX = 42,

		// IGameEventManager2
		FIRE_EVENT_CLIENT_SIDE_INDEX = 9,

		// ClientModeShared
		OVERRIDE_VIEW_INDEX = 18,
		DO_POST_SCREEN_EFFECTS_INDEX = 44,

		// CEngineClient
		CLIENT_CMD_UNRESTRICTED_INDEX = 114,

		// IFileSystem
		UNVERIFIED_FILE_HASHES_INDEX = 101,
		UNK_FILE_CHECK_INDEX = 128,

		// CViewRender
		RENDER_VIEW_INDEX = 6,

		// CSurface
		LOCK_CURSOR_INDEX = 67,

		// IVModelRender
		DRAW_MODEL_EXECUTE_INDEX = 21,
	};

	namespace Managers
	{
		inline CDetourHook m_CreateMoveProxy;
		inline CDetourHook m_EndScene;
		inline CDetourHook m_Reset;
		inline CDetourHook m_PaintTraverse;
		inline CDetourHook m_FireEventClientSide;
		inline CDetourHook m_OverrideView;
		inline CDetourHook m_GetColorModulation;
		inline CDetourHook m_IsUsingStaticPropDebugModes;
		inline CDetourHook m_FrameStageNotify;
		inline CDetourHook m_ClientCmd_Unrestricted;
		inline CDetourHook m_StartParticleEffect;
		inline CDetourHook m_RenderView;
		inline CDetourHook m_UnverifiedFileHashes;
		inline CDetourHook m_UnkFileCheck;
		inline CDetourHook m_LockCursor;
		inline CDetourHook m_DrawModelExecute;
		inline CDetourHook m_DoPostScreenEffects;

		inline CDetourHook m_ClientValidAddr;
		inline CDetourHook m_EngineValidAddr;
		inline CDetourHook m_StudioRenderValidAddr;
		inline CDetourHook m_MaterialSystemValidAddr;
	}

	bool Initialize();

	LRESULT __stdcall hkWndProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam);

	// IDirect3DDevice9
	long __stdcall hkEndScene(IDirect3DDevice9* pDevice);
	long __stdcall hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParams);

	// CHLClient
	void __fastcall hkCreateMoveProxy(void* ecx, int edx, int nSequenceNumber, float flInputSampleFrametime, 
		bool bIsActive);
	void __stdcall hkFrameStageNotify(int nStage);

	// CPanel
	void __fastcall hkPaintTraverse(void* ecx, int edx, unsigned int nPanel, bool bForceRepaint, bool bAllowForce);

	// IGameEventManager2
	bool __fastcall hkFireEventClientSide(void* ecx, int edx, IGameEvent* pEvent);

	// ClientModeShared
	void __fastcall	hkOverrideView(void* ecx, int edx, CViewSetup* pSetup);
	int __fastcall hkDoPostScreenEffects(void* ecx, int edx, CViewSetup* pSetup);

	// CEngineClient
	void __fastcall hkClientCmd_Unrestricted(void* ecx, int edx, const char* szCmdString, bool bFromConsoleOrKeyBind);

	// IMaterial
	void __fastcall hkGetColorModulation(void* ecx, int edx, float* r, float* g, float* b);
	bool __stdcall hkIsUsingStaticPropDebugModes();

	// IFileSystem
	int __fastcall hkGetUnverifiedFileHashes(void* ecx, int edx, int nMaxFiles);
	int __fastcall hkUnkFileCheck(void* ecx, int edx);

	// CViewRender
	void __fastcall hkRenderView(void* ecx, int edx, const CViewSetup& View, const CViewSetup& Hud, int nClearFlags, int nWhatToDo);

	// CSurface
	void __stdcall hkLockCursor();

	// IVModelRender
	void __fastcall hkDrawModelExecute(void* ecx, int edx, IMatRenderContext* pCtx, const DrawModelState_t& State,
		const ModelRenderInfo_t& Info, matrix3x4_t* pMatrix);

	char __fastcall hkClientValidAddr(void* ecx, int edx, const char* szModuleName);
	char __fastcall hkEngineValidAddr(void* ecx, int edx, const char* szModuleName);
	char __fastcall hkStudioRenderValidAddr(void* ecx, int edx, const char* szModuleName);
	char __fastcall hkMaterialSystemValidAddr(void* ecx, int edx, const char* szModuleName);

	void __fastcall hkStartParticleEffect(const CEffectData& Data, int nSplitScreenSlot);
}