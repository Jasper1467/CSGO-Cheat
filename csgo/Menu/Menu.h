#pragma once

#include "../Dependencies/imgui/imgui.h"

namespace Menu
{
	void Render();
	void Initialize();

	inline bool m_bIsVisible = true;
}