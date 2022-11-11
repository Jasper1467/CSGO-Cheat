#pragma once

using GetSymbolProcFn = bool(__cdecl*)(const char*);

class KeyValues
{
public:
	static KeyValues* FromString(const char* szName, const char* szValue = nullptr);
};