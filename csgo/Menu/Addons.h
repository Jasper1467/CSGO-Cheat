#pragma once

#include "../Dependencies/imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../Dependencies/imgui/imgui_internal.h"

#include "../Utilities/Color.h"

#include <Windows.h>
#include <string>
#include <functional>
#include <vector>
#include <span>

namespace ImGuiEx
{
	bool ToggleButton(const char* szLabel, bool* v, const ImVec2& SizeArg = ImVec2(0, 0));

	bool BeginGroupBox(const char* name, const ImVec2& size_arg = ImVec2(0, 0));
	void EndGroupBox();

	bool Button(const char* szLabel, const ImVec2& SizeArg = ImVec2(0, 0));

	void HelpMarker(const char* szDescription);

	bool ColorPicker(float* pCol, bool bAlphaBar);
	bool ColorPicker3(float Col[3]);
	bool ColorPicker4(float Col[4]);
	bool ColorPicker(const char* szLabel, ColorVar* v, bool bAddLabelNextToButton = false);
}