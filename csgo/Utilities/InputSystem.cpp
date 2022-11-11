// used: std::this_thread
#include <thread>

#include "inputsystem.h"
#include "Interfaces.h"

#include <d3d9.h>
#include "../Menu/Menu.h"
#include "../Hooks/Hooks.h"

bool InputSystem::Setup()
{
	D3DDEVICE_CREATION_PARAMETERS creationParameters = { };
	while (FAILED(Interfaces::m_pDevice->GetCreationParameters(&creationParameters)))
		Sleep(200);

	hWindow = creationParameters.hFocusWindow;

	if (hWindow == nullptr)
		return false;

	pOldWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(hWindow, GWLP_WNDPROC,
		reinterpret_cast<LONG_PTR>(Hooks::hkWndProc)));

	if (pOldWndProc == nullptr)
		return false;

	return true;
}

void InputSystem::Restore()
{
	if (pOldWndProc != nullptr)
	{
		SetWindowLongPtrW(hWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(pOldWndProc));
		pOldWndProc = nullptr;
	}

	// reset input state
	Interfaces::m_pInputSystem->EnableInput(true);
}

bool InputSystem::Process(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// prevent process when e.g. binding something in-menu
	if (Menu::m_bIsVisible && wParam != VK_INSERT)
		return false;

	// current active key
	int nKey = 0;
	// current active key state
	EKeyState state = EKeyState::NONE;

	switch (uMsg)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (wParam < 256U)
		{
			nKey = wParam;
			state = EKeyState::DOWN;
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (wParam < 256U)
		{
			nKey = wParam;
			state = EKeyState::UP;
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
		nKey = VK_LBUTTON;
		state = uMsg == WM_LBUTTONUP ? EKeyState::UP : EKeyState::DOWN;
		break;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
		nKey = VK_RBUTTON;
		state = uMsg == WM_RBUTTONUP ? EKeyState::UP : EKeyState::DOWN;
		break;
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
		nKey = VK_MBUTTON;
		state = uMsg == WM_MBUTTONUP ? EKeyState::UP : EKeyState::DOWN;
		break;
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_XBUTTONDBLCLK:
		nKey = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2);
		state = uMsg == WM_XBUTTONUP ? EKeyState::UP : EKeyState::DOWN;
		break;
	default:
		return false;
	}

	// save key states
	if (state == EKeyState::UP && arrKeyState.at(nKey) == EKeyState::DOWN) // if swap states it will be pressed state
		arrKeyState.at(nKey) = EKeyState::RELEASED;
	else
		arrKeyState.at(nKey) = state;

	return true;
}

std::string InputSystem::GetKeyNameById(int nId)
{
	static std::unordered_map< int, std::string > key_names =
	{
		{ 0, ("None") },
		{ VK_LBUTTON, ("Mouse 1") },
		{ VK_RBUTTON, ("Mouse 2") },
		{ VK_MBUTTON, ("Mouse 3") },
		{ VK_XBUTTON1, ("Mouse 4") },
		{ VK_XBUTTON2, ("Mouse 5") },
		{ VK_BACK, ("Back") },
		{ VK_TAB, ("Tab") },
		{ VK_CLEAR, ("Clear") },
		{ VK_RETURN, ("Enter") },
		{ VK_SHIFT, ("Shift") },
		{ VK_CONTROL, ("Ctrl") },
		{ VK_MENU, ("Alt") },
		{ VK_PAUSE, ("Pause") },
		{ VK_CAPITAL, ("Caps Lock") },
		{ VK_ESCAPE, ("Escape") },
		{ VK_SPACE, ("Space") },
		{ VK_PRIOR, ("Page Up") },
		{ VK_NEXT, ("Page Down") },
		{ VK_END, ("End") },
		{ VK_HOME, ("Home") },
		{ VK_LEFT, ("Left Key") },
		{ VK_UP, ("Up Key") },
		{ VK_RIGHT, ("Right Key") },
		{ VK_DOWN, ("Down Key") },
		{ VK_SELECT, ("Select") },
		{ VK_PRINT, ("Print Screen") },
		{ VK_INSERT, ("Insert") },
		{ VK_DELETE, ("Delete") },
		{ VK_HELP, ("Help") },
		{ VK_SLEEP, ("Sleep") },
		{ VK_MULTIPLY, ("*") },
		{ VK_ADD, ("+") },
		{ VK_SUBTRACT, ("-") },
		{ VK_DECIMAL, (".") },
		{ VK_DIVIDE, ("/") },
		{ VK_NUMLOCK, ("Num Lock") },
		{ VK_SCROLL, ("Scroll") },
		{ VK_LSHIFT, ("Left Shift") },
		{ VK_RSHIFT, ("Right Shift") },
		{ VK_LCONTROL, ("Left Ctrl") },
		{ VK_RCONTROL, ("Right Ctrl") },
		{ VK_LMENU, ("Left Alt") },
		{ VK_RMENU, ("Right Alt") },
	};

	// check for 0-9, A-Z
	if (nId >= 0x30 && nId <= 0x5A)
	{
		return std::string(1, (char)nId);
	}
	// check for numpad
	else if (nId >= 0x60 && nId <= 0x69)
	{
		std::string ret = ("Num ") + std::to_string(nId - 0x60);
		return ret;
	}
	// check for function keys
	else if (nId >= 0x70 && nId <= 0x87)
	{
		return std::string(std::to_string((nId - 0x70) + 1));
	}

	return key_names[nId];
	//VMP_END;
}