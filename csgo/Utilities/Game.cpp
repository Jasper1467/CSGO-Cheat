#include "Game.h"

#include "Utilities.h"

CHudElement* Game::FindElement(const char* szName)
{
	auto pHud = SCAN_SIGNATURE("client.dll", "B9 ? ? ? ? 68 ? ? ? ? E8 ? ? ? ? 89") + 1;

	using Fn = CHudElement * (__thiscall*)(void*, const char* szName);
	return ((Fn)SCAN_SIGNATURE("client.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28 7E 1E"))(pHud, szName);
}