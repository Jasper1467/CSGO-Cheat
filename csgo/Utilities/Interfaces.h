#pragma once

#include <d3d9.h>

using InstantiateInterfaceFn = void* (__cdecl*)();
class CInterfaceRegister
{
public:
	InstantiateInterfaceFn	pCreateFn;		//0x0
	const char* szName;			//0x4
	CInterfaceRegister* pNext;			//0x8
};

#include "../SDK/CPanel.h"
#include "../SDK/CInputSystem.h"
#include "../SDK/CEngineClient.h"
#include "../SDK/CDebugOverlay.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CInput.h"
#include "../SDK/CGlobalVarsBase.h"
#include "../SDK/CMDLCache.h"
#include "../SDK/IWeaponSystem.h"
#include "../SDK/CSurface.h"
#include "../SDK/CHLClient.h"
#include "../SDK/CModelInfoClient.h"
#include "../SDK/CEngineTrace.h"
#include "../SDK/CClientState.h"
#include "../SDK/CGameEvent.h"
#include "../SDK/CPrediction.h"
#include "../SDK/ClientModeShared.h"
#include "../SDK/IMemAlloc.h"
#include "../SDK/ICVar.h"
#include "../SDK/CMaterialSystem.h"
#include "../SDK/IVModelRender.h"
#include "../SDK/iglowmanager.h"

namespace Interfaces
{
	bool Initialize();

	inline CHLClient* m_pClient = nullptr;
	inline CPanel* m_pPanel = nullptr;
	inline IDirect3DDevice9* m_pDevice = nullptr;
	inline CInputSystem* m_pInputSystem = nullptr;
	inline CEngineClient* m_pEngine = nullptr;
	inline CDebugOverlay* m_pDebugOverlay = nullptr;
	inline CClientEntityList* m_pEntityList = nullptr;
	inline CInput* m_pInput = nullptr;
	inline CGlobalVarsBase* m_pGlobals = nullptr;
	inline CMDLCache* m_pMDLCache = nullptr;
	inline IWeaponSystem* m_pWeaponSystem = nullptr;
	inline CSurface* m_pSurface = nullptr;
	inline IVModelInfoClient* m_pModelInfo = nullptr;
	inline CEngineTrace* m_pTrace = nullptr;
	inline CClientState* m_pClientState = nullptr;
	inline CGameEventManager* m_pGameEventManager = nullptr;
	inline CPrediction* m_pPrediction = nullptr;
	inline ClientModeShared* m_pClientMode = nullptr;
	inline IMemAlloc* m_pMemAlloc = nullptr;
	inline ICVar* m_pCVar = nullptr;
	inline CMaterialSystem* m_pMaterialSystem = nullptr;
	inline void* m_pViewRender = nullptr;
	inline IVModelRender* m_pModelRender = nullptr;
	inline IGlowObjectManager* m_pGlowManager = nullptr;
}

#define TICK_INTERVAL			( Interfaces::m_pGlobals->m_flIntervalPerTick )
#define TIME_TO_TICKS( t )		( static_cast<int>( 0.5f + static_cast<float>( t ) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL * static_cast<float>( t ) )
#define ROUND_TO_TICKS( t )		( TICK_INTERVAL * TIME_TO_TICKS( t ) )
#define TICK_NEVER_THINK		( -1 )