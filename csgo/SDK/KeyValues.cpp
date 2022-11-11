#include "KeyValues.h"

#include "../Utilities/Utilities.h"

KeyValues* KeyValues::FromString(const char* szName, const char* szValue/* = nullptr */)
{
	const static auto fromstring = SCAN_SIGNATURE("client.dll", "55 8B EC 81 EC ? ? ? ? 85 D2 53");

	KeyValues* res;
	__asm
	{
		push 0
		mov edx, szValue
		mov ecx, szName
		call fromstring
		add esp, 4
		mov res, eax
	}

	return res;
}