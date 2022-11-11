#pragma once

#include <Windows.h>
#include <cstdint>
#include "../Dependencies/imgui/imgui.h"
#include "Color.h"

namespace Utilities
{
	template <typename T>
	inline static constexpr auto RelativeToAbsolute(uintptr_t address) noexcept
	{
		return (T)(address + 4 + *reinterpret_cast<std::int32_t*>(address));
	}

	inline std::string CharsToString(std::vector<char> x)
	{
		std::string szOut;
		for (int i = 0; i < x.size(); i++)
			szOut.push_back(x.at(i));

		return szOut;
	}

	uint8_t* ScanSignature(HMODULE hModule, const char* szSignature);
	uint8_t* ScanSignature(const char* szModule, const char* szSignature);

	const char* Format(const char* szFmt, ...);

	Color GetHealthColor(int nHealth);

	const char8_t* GetWeaponIcon(short nItemDefinitionIndex);
	const char* GetWeaponName(short nItemDefinitionIndex);
}

#define SCAN_SIGNATURE(dll, signature) Utilities::ScanSignature(dll, signature)

// Used for jumps, ...
#define SCAN_SIGNATURE_RELATIVE(dll, signature, offset) Utilities::RelativeToAbsolute<uintptr_t>( \
				(uintptr_t)SCAN_SIGNATURE(dll, signature) + offset)