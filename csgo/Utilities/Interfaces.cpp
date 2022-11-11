#include "Interfaces.h"

#include <string_view>
#include <Windows.h>
#include <stdexcept>

#include "Utilities.h"

template <typename T>
T* Capture(const char* szModuleName, const std::string_view szInterface)
{
	const auto GetRegisterList = [&szModuleName]() -> CInterfaceRegister*
	{
		void* oCreateInterface = nullptr;

		if (const auto hModule = GetModuleHandleA(szModuleName); hModule != nullptr)
			oCreateInterface = GetProcAddress(hModule, "CreateInterface");

		if (oCreateInterface == nullptr)
			throw std::runtime_error("failed get createinterface address");

		const std::uintptr_t uCreateInterfaceRelative = reinterpret_cast<std::uintptr_t>(oCreateInterface) + 0x5;
		const std::uintptr_t uCreateInterface = uCreateInterfaceRelative + 4U + *reinterpret_cast<std::int32_t*>(uCreateInterfaceRelative);
		return **reinterpret_cast<CInterfaceRegister***>(uCreateInterface + 0x6);
	};

	for (CInterfaceRegister* pRegister = GetRegisterList(); pRegister != nullptr; pRegister = pRegister->pNext)
	{
		// found needed interface
		if ((std::string_view(pRegister->szName).compare(0U, szInterface.length(), szInterface) == 0 &&
			// and it have digits after name
			std::atoi(pRegister->szName + szInterface.length()) > 0) ||
			// or given full name with hardcoded digits
			szInterface.compare(pRegister->szName) == 0)
		{
			// capture our interface
			auto pInterface = pRegister->pCreateFn();

			// log interface address
			//L::Print(XorStr("captured {} interface -> {:#08X}"), pRegister->szName, reinterpret_cast<std::uintptr_t>(pInterface));

			return static_cast<T*>(pInterface);
		}
	}

	/*#ifdef DEBUG_CONSOLE
		L::PushConsoleColor(FOREGROUND_INTENSE_RED);
		L::Print(XorStr("[error] failed to find interface \"{}\" in \"{}\""), szInterface, szModuleName);
		L::PopConsoleColor();
	#endif*/

	return nullptr;
}

bool Interfaces::Initialize()
{
	m_pClient = Capture<CHLClient>("client.dll", "VClient");
	m_pEntityList = Capture<CClientEntityList>("client.dll", "VClientEntityList");
	m_pPrediction = Capture<CPrediction>("client.dll", "VClientPrediction");

	m_pPanel = Capture<CPanel>("vgui2.dll", "VGUI_Panel");

	m_pSurface = Capture<CSurface>("vguimatsurface.dll", "VGUI_Surface");

	m_pInputSystem = Capture<CInputSystem>("inputsystem.dll", "InputSystemVersion");

	m_pEngine = Capture<CEngineClient>("engine.dll", "VEngineClient");
	m_pDebugOverlay = Capture<CDebugOverlay>("engine.dll", "VDebugOverlay");
	m_pModelInfo = Capture<IVModelInfoClient>("engine.dll", "VModelInfoClient");
	m_pTrace = Capture<CEngineTrace>("engine.dll", "EngineTraceClient");
	m_pGameEventManager = Capture<CGameEventManager>("engine.dll", "GAMEEVENTSMANAGER002");
	//m_pModelRender = Capture<IVModelRender>("engine.dll", "VEngineModel");

	m_pMDLCache = Capture<CMDLCache>("datacache.dll", "MDLCache");

	m_pMaterialSystem = Capture<CMaterialSystem>("materialsystem.dll", "VMaterialSystem");

	m_pCVar = Capture<ICVar>("vstdlib.dll", "VEngineCvar");

	m_pDevice = **reinterpret_cast<IDirect3DDevice9***>(
		SCAN_SIGNATURE("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);

	m_pInput = *(CInput**)(SCAN_SIGNATURE("client.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);
	m_pGlobals = **(CGlobalVarsBase***)(SCAN_SIGNATURE("client.dll", "A1 ? ? ? ? 5E 8B 40 10") + 1);
	m_pWeaponSystem = *(IWeaponSystem**)(SCAN_SIGNATURE("client.dll", "8B 35 ? ? ? ? FF 10 0F B7 C0") + 2);
	m_pViewRender = **(void****)(SCAN_SIGNATURE("client.dll", "8B 0D ? ? ? ? FF 75 0C 8B 45 08") + 2);
	m_pGlowManager = *(IGlowObjectManager**)(SCAN_SIGNATURE("client.dll", "0F 11 05 ? ? ? ? 83 C8 01") + 2);

	m_pClientState = **(CClientState***)(SCAN_SIGNATURE("engine.dll", "A1 ? ? ? ? 8B 88 ? ? ? ? 85 C9 75 07") + 1);

	// From CHLClient::HudProcessInput
	m_pClientMode = **(ClientModeShared***)(GetVFunc<uintptr_t>(m_pClient, 10) + 5);

	m_pMemAlloc = *(IMemAlloc**)(GetProcAddress(GetModuleHandleA("tier0.dll"), "g_pMemAlloc"));

	return true;
}