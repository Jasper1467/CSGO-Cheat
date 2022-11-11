#pragma once
#include <cstddef>

template <typename T = void*>
constexpr T GetVFunc(void* thisptr, std::size_t nIndex)
{
	return (*static_cast<T**>(thisptr))[nIndex];
}

template <typename T, typename ... Args_t>
constexpr T CallVFunc(void* thisptr, std::size_t nIndex, Args_t... argList)
{
	using VirtualFn = T(__thiscall*)(void*, decltype(argList)...);
	return (*static_cast<VirtualFn**>(thisptr))[nIndex](thisptr, argList...);
}