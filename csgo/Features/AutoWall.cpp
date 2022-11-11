#include "AutoWall.h"

#include "../Utilities/Interfaces.h"
#include "../Utilities/Utilities.h"

// TODO: Make this more readable / understandable
bool Features::AutoWall::IsBreakableEntity(CEntity* pEntity)
{
	if (!pEntity || !pEntity->GetIndex())
		return false;

	static auto IsBreakable = SCAN_SIGNATURE("client.dll", "55 8B EC 51 56 8B F1 85 F6 74 68");

	auto TakeDamage = *(uintptr_t*)(uintptr_t(IsBreakable) + 0x26);
	auto Backup = *(uintptr_t*)(uintptr_t(pEntity) + TakeDamage);

	const char* szNetworkName = pEntity->GetClientClass()->m_pNetworkName;

	if (!strcmp(szNetworkName, "CBreakableSurface"))
		*(uint8_t*)((uintptr_t)pEntity + TakeDamage) = 2;
	else if (!strcmp(szNetworkName, "CBaseDoor") || !strcmp(szNetworkName, "CDynamicProp"))
		*(uint8_t*)((uintptr_t)pEntity + TakeDamage) = 0;

	using Fn = bool(__thiscall*)(IClientEntity*);
	auto result = ((Fn)IsBreakable)(pEntity);

	*(uint8_t*)((uintptr_t)pEntity + TakeDamage) = Backup;
	return result;
}