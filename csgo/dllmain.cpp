#pragma once
#include <Windows.h>
#include <stdexcept>

#include "Utilities/Interfaces.h"
#include "Hooks/Hooks.h"
#include "Menu/Menu.h"
#include "Utilities/InputSystem.h"
#include "Utilities/Render.h"
#include "Utilities/Utilities.h"
#include "Utilities/Client.h"
#include "Utilities/NetVarManager.h"

#include "Features/Visuals.h"

DWORD WINAPI OnDllAttach(LPVOID lpParameter)
{
	HMODULE hModule = HMODULE(lpParameter);

	try
	{
		// Add cheat module to the trusted modules
		using InitSafeModuleFn = void(__fastcall*)(void*, void*);
		static auto oInitSafeModule = ((InitSafeModuleFn)Utilities::ScanSignature("client.dll", "56 8B 71 3C B8"));
		if (oInitSafeModule != nullptr)
			oInitSafeModule(hModule, nullptr);

		while (!GetModuleHandleA("serverbrowser.dll"))
			Sleep(200);

		AllocConsole();
		freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
		freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
		SetConsoleTitleA("=)");

		//Client::m_szUserName = "Username";

		//printf("Username: %s", Client::m_szUserName);

		// Capture interfaces from game/steam (not always) modules
		if (!Interfaces::Initialize())
			throw std::runtime_error("failed to capture interfaces");

		//NetVarManager::Initialize();

#ifdef PRINT_NETVARS
		NetVarManager::Dump();
#endif // PRINT_NETVARS

		if (!InputSystem::Setup())
			throw std::runtime_error("failed to setup inputsystem");

		Renderer::Initialize();

		Features::Visuals::MirrorCam::Initialize();

		// Do hooks
		if (!Hooks::Initialize())
			throw std::runtime_error("failed to do hooks");
	}
	catch (const std::exception& ex)
	{
		FreeLibraryAndExitThread(static_cast<HMODULE>(lpParameter), EXIT_FAILURE);
	}

	return TRUE;
}

DWORD WINAPI OnDllDetach(LPVOID lpParameter)
{
	// Free our library memory from process and exit from our thread
	FreeLibraryAndExitThread((HMODULE)lpParameter, EXIT_SUCCESS);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		// Disable DLL_THREAD_ATTACH and DLL_THREAD_DETACH reasons to call
		DisableThreadLibraryCalls(hModule);

		// Basic process check
		if (!GetModuleHandleA("csgo.exe"))
		{
			MessageBoxA(nullptr, "this cannot be injected in another process\nopen <csgo.exe> to inject", "qo0 base", MB_OK);
			return FALSE;
		}

		// Create main thread
		if (auto hThread = CreateThread(nullptr, 0U, OnDllAttach, hModule, 0UL, nullptr); hThread != nullptr)
			CloseHandle(hThread);

		// Create detach thread
		if (auto hThread = CreateThread(nullptr, 0U, OnDllDetach, hModule, 0UL, nullptr); hThread != nullptr)
			CloseHandle(hThread);
	}

	return TRUE;
}