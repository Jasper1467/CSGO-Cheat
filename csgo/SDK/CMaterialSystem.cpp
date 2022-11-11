#include "CMaterialSystem.h"

#include "../Utilities/Utilities.h"

bool& CMaterialSystem::DisableRenderTargetAllocationForever()
{
	// @xref: "Tried BeginRenderTargetAllocation after game startup. If I let you do this, all users would suffer.\n"
	static auto uDisableRenderTargetAllocationForever = *reinterpret_cast<std::uintptr_t*>(
		SCAN_SIGNATURE("materialsystem.dll", "80 B9 ? ? ? ? ? 74 0F") + 0x2);

	return *reinterpret_cast<bool*>(reinterpret_cast<std::uintptr_t>(this) + uDisableRenderTargetAllocationForever);
}