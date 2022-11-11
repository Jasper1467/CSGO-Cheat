#include "Addons.h"

#include "../Utilities/Color.h"

#include <array>

#include "../Dependencies/imgui/imgui.h"
#include "../Dependencies/imgui/imgui_internal.h"
#include "Config/Vars.h"
#include "../Utilities/Fonts.h"
#include "imgui_tricks.h"
#include "../Utilities/Render.h"
#include "../Utilities/InputSystem.h"
#include "../Utilities/Utilities.h"

bool ImGuiEx::ToggleButton(const char* szLabel, bool* v, const ImVec2& SizeArg)
{
	ImGuiWindow* pWindow = ImGui::GetCurrentWindow();
	if (pWindow->SkipItems)
		return false;

	int nFlags = 0;
	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = pWindow->GetID(szLabel);
	const ImVec2 label_size = ImGui::CalcTextSize(szLabel, NULL, true);

	ImVec2 pos = pWindow->DC.CursorPos;
	ImVec2 size = ImGui::CalcItemSize(SizeArg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	if (pWindow->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat) nFlags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, nFlags);

	// Render
	const ImU32 col = ImGui::GetColorU32((hovered && held || *v) ? ImGuiCol_ButtonActive : (hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button));
	ImGui::RenderFrame(bb.Min, bb.Max, col, true);
	ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max, szLabel, NULL, &label_size, style.ButtonTextAlign, &bb);
	if (pressed)
		*v = !*v;

	return pressed;
}

bool ImGuiEx::BeginGroupBox(const char* name, const ImVec2& size_arg)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;

	window->DC.CursorPos.y += GImGui->FontSize / 2;
	const ImVec2 content_avail = ImGui::GetContentRegionAvail();
	ImVec2 size = ImFloor(size_arg);
	if (size.x <= 0.0f) {
		size.x = ImMax(content_avail.x, 4.0f) - fabsf(size.x); // Arbitrary minimum zero-ish child size of 4.0f (0.0f causing too much issues)
	}
	if (size.y <= 0.0f) {
		size.y = ImMax(content_avail.y, 4.0f) - fabsf(size.y);
	}

	ImGui::SetNextWindowSize(size);
	bool ret;
	ImGui::Begin(name, &ret, flags);
	//bool ret = ImGui::Begin(name, NULL, size, -1.0f, flags);

	window = ImGui::GetCurrentWindow();

	auto padding = ImGui::GetStyle().WindowPadding;

	auto text_size = ImGui::CalcTextSize(name, NULL, true);

	if (text_size.x > 1.0f) {
		window->DrawList->PushClipRectFullScreen();
		//window->DrawList->AddRectFilled(window->DC.CursorPos - ImVec2{ 4, 0 }, window->DC.CursorPos + (text_size + ImVec2{ 4, 0 }), GetColorU32(ImGuiCol_ChildWindowBg));
		//RenderTextClipped(pos, pos + text_size, name, NULL, NULL, GetColorU32(ImGuiCol_Text));
		window->DrawList->PopClipRect();
	}
	//if (!(window->Flags & ImGuiWindowFlags_ShowBorders))
		//ImGui::GetCurrentWindow()->Flags &= ~ImGuiWindowFlags_ShowBorders;

	return ret;
}

void ImGuiEx::EndGroupBox()
{
	ImGui::EndChild();
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window->DC.CursorPosPrevLine.y -= GImGui->FontSize / 2;
}

bool ImGuiEx::Button(const char* szLabel, const ImVec2& SizeArg)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(szLabel);
	const ImVec2 label_size = ImGui::CalcTextSize(szLabel, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size = ImGui::CalcItemSize(SizeArg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ImGui::ItemSize(size, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, NULL);

	if (hovered || held)
		ImGui::SetMouseCursor(0);

	window->DrawList->AddRectFilled(ImVec2(bb.Min.x + 0, bb.Min.y + 0), ImVec2(bb.Max.x + 0, bb.Max.y + 0), ImColor(29, 28, 32, 244), 4, 15);
	window->DrawList->AddRect(ImVec2(bb.Min.x + 0, bb.Min.y + 0), ImVec2(bb.Max.x + 0, bb.Max.y + 0), ImColor(56, 54, 59, 255), 4, 15, 1.000000);
	window->DrawList->AddRect(ImVec2(bb.Min.x + -1, bb.Min.y + -1), ImVec2(bb.Max.x + 1, bb.Max.y + 1), ImColor(17, 17, 22, 255), 4, 15, 1.000000);
	window->DrawList->AddRect(ImVec2(bb.Min.x + 1, bb.Min.y + 1), ImVec2(bb.Max.x + -1, bb.Max.y + -1), ImColor(17, 17, 22, 255), 4, 15, 1.000000);

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(229 / 255.f, 229 / 255.f, 229 / 255.f, 255 / 255.f));
	ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, szLabel, NULL, &label_size, style.ButtonTextAlign, &bb);
	ImGui::PopStyleColor();

	return pressed;
}

void ImGuiEx::HelpMarker(const char* szDescription)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(450.f);
		ImGui::TextUnformatted(szDescription);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

static constexpr std::array<const char*, 166U> arrKeyNames =
{
	"",
	"mouse 1", "mouse 2", "cancel", "mouse 3", "mouse 4", "mouse 5", "",
	"backspace", "tab", "", "", "clear", "enter", "", "",
	"shift", "control", "alt", "pause", "caps", "", "", "", "", "", "",
	"escape", "", "", "", "", "space", "page up", "page down",
	"end", "home", "left", "up", "right", "down", "", "", "",
	"print", "insert", "delete", "",
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
	"", "", "", "", "", "", "",
	"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
	"l", "m", "n", "o", "p", "q", "r", "s", "t", "u",
	"v", "w", "x", "y", "z", "lwin", "rwin", "", "", "",
	"num0", "num1", "num2", "num3", "num4", "num5",
	"num6", "num7", "num8", "num9",
	"*", "+", "", "-", ".", "/",
	"f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8",
	"f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16",
	"f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24",
	"", "", "", "", "", "", "", "",
	"num lock", "scroll lock",
	"", "", "", "", "", "", "",
	"", "", "", "", "", "", "",
	"lshift", "rshift", "lctrl",
	"rctrl", "lmenu", "rmenu"
};

bool ImGuiEx::ColorPicker(float* pCol, bool bAlphaBar)
{
	const auto edge_size = 200; // = int(ImGui::GetWindowWidth() * 0.75f);
	const auto sv_picker_size = ImVec2(edge_size, edge_size);
	const auto spacing = ImGui::GetStyle().ItemInnerSpacing.x;
	const auto hue_picker_width = 20.f;
	const auto crosshair_size = 7.0f;

	ImColor color(pCol[0], pCol[1], pCol[2]);
	auto value_changed = false;
	auto draw_list = ImGui::GetWindowDrawList();
	const auto picker_pos = ImGui::GetCursorScreenPos();

	float hue, saturation, value;
	ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, hue, saturation, value);
	ImColor colors[] = {
		ImColor(255, 0, 0), ImColor(255, 255, 0), ImColor(0, 255, 0), ImColor(0, 255, 255), ImColor(0, 0, 255), ImColor(255, 0, 255), ImColor(255, 0, 0)
	};

	for (auto i = 0; i < 6; i++)
		draw_list->AddRectFilledMultiColor(ImVec2(picker_pos.x + sv_picker_size.x + spacing, picker_pos.y + i * (sv_picker_size.y / 6)), ImVec2(picker_pos.x + sv_picker_size.x + spacing + hue_picker_width, picker_pos.y + (i + 1) * (sv_picker_size.y / 6)), colors[i], colors[i], colors[i + 1], colors[i + 1]);

	draw_list->AddLine(ImVec2(picker_pos.x + sv_picker_size.x + spacing - 2, picker_pos.y + hue * sv_picker_size.y), ImVec2(picker_pos.x + sv_picker_size.x + spacing + 2 + hue_picker_width, picker_pos.y + hue * sv_picker_size.y), ImColor(255, 255, 255));
	if (bAlphaBar)
	{
		const auto alpha = pCol[3];
		draw_list->AddRectFilledMultiColor(ImVec2(picker_pos.x + sv_picker_size.x + 2 * spacing + hue_picker_width, picker_pos.y), ImVec2(picker_pos.x + sv_picker_size.x + 2 * spacing + 2 * hue_picker_width, picker_pos.y + sv_picker_size.y), ImColor(0, 0, 0), ImColor(0, 0, 0), ImColor(255, 255, 255), ImColor(255, 255, 255));
		draw_list->AddLine(ImVec2(picker_pos.x + sv_picker_size.x + 2 * (spacing - 2) + hue_picker_width, picker_pos.y + alpha * sv_picker_size.y), ImVec2(picker_pos.x + sv_picker_size.x + 2 * (spacing + 2) + 2 * hue_picker_width, picker_pos.y + alpha * sv_picker_size.y), ImColor(255.f - alpha, 255.f, 255.f));
	}

	const auto c_o_color_black = ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 1.f));
	const auto c_o_color_black_transparent = ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 0.f));
	const auto c_o_color_white = ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 1.f, 1.f, 1.f));

	ImVec4 c_hue_value(1, 1, 1, 1);
	ImGui::ColorConvertHSVtoRGB(hue, 1, 1, c_hue_value.x, c_hue_value.y, c_hue_value.z);

	const auto o_hue_color = ImGui::ColorConvertFloat4ToU32(c_hue_value);
	draw_list->AddRectFilledMultiColor(ImVec2(picker_pos.x, picker_pos.y), ImVec2(picker_pos.x + sv_picker_size.x, picker_pos.y + sv_picker_size.y), c_o_color_white, o_hue_color, o_hue_color, c_o_color_white);
	draw_list->AddRectFilledMultiColor(ImVec2(picker_pos.x, picker_pos.y), ImVec2(picker_pos.x + sv_picker_size.x, picker_pos.y + sv_picker_size.y), c_o_color_black_transparent, c_o_color_black_transparent, c_o_color_black, c_o_color_black);

	const auto x = saturation * sv_picker_size.x;
	const auto y = (1 - value) * sv_picker_size.y;
	const ImVec2 p(picker_pos.x + x, picker_pos.y + y);

	draw_list->AddLine(ImVec2(p.x - crosshair_size, p.y), ImVec2(p.x - 2, p.y), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(p.x + crosshair_size, p.y), ImVec2(p.x + 2, p.y), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(p.x, p.y + crosshair_size), ImVec2(p.x, p.y + 2), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(p.x, p.y - crosshair_size), ImVec2(p.x, p.y - 2), ImColor(255, 255, 255));

	ImGui::InvisibleButton(("saturation_value_selector"), sv_picker_size);

	if (ImGui::IsItemActive() && ImGui::GetIO().MouseDown[0])
	{
		auto mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);
		if (mouse_pos_in_canvas.x < 0)
			mouse_pos_in_canvas.x = 0;
		else if (mouse_pos_in_canvas.x >= sv_picker_size.x - 1)
			mouse_pos_in_canvas.x = sv_picker_size.x - 1;

		if (mouse_pos_in_canvas.y < 0)
			mouse_pos_in_canvas.y = 0;
		else if (mouse_pos_in_canvas.y >= sv_picker_size.y - 1)
			mouse_pos_in_canvas.y = sv_picker_size.y - 1;

		value = 1 - (mouse_pos_in_canvas.y / (sv_picker_size.y - 1));
		saturation = mouse_pos_in_canvas.x / (sv_picker_size.x - 1);
		value_changed = true;
	} // hue bar logic

	ImGui::SetCursorScreenPos(ImVec2(picker_pos.x + spacing + sv_picker_size.x, picker_pos.y));
	ImGui::InvisibleButton(("hue_selector"), ImVec2(hue_picker_width, sv_picker_size.y));

	if (ImGui::GetIO().MouseDown[0] && (ImGui::IsItemHovered() || ImGui::IsItemActive()))
	{
		auto mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);
		if (mouse_pos_in_canvas.y < 0)
			mouse_pos_in_canvas.y = 0;
		else if (mouse_pos_in_canvas.y >= sv_picker_size.y - 1)
			mouse_pos_in_canvas.y = sv_picker_size.y - 1;

		hue = mouse_pos_in_canvas.y / (sv_picker_size.y - 1);
		value_changed = true;
	}

	if (bAlphaBar)
	{
		ImGui::SetCursorScreenPos(ImVec2(picker_pos.x + spacing * 2 + hue_picker_width + sv_picker_size.x, picker_pos.y));
		ImGui::InvisibleButton(("alpha_selector"), ImVec2(hue_picker_width, sv_picker_size.y));

		if (ImGui::GetIO().MouseDown[0] && (ImGui::IsItemHovered() || ImGui::IsItemActive()))
		{
			auto mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);
			if (mouse_pos_in_canvas.y < 0)
				mouse_pos_in_canvas.y = 0;
			else if (mouse_pos_in_canvas.y >= sv_picker_size.y - 1)
				mouse_pos_in_canvas.y = sv_picker_size.y - 1;

			const auto alpha = mouse_pos_in_canvas.y / (sv_picker_size.y - 1);
			pCol[3] = alpha;
			value_changed = true;
		}
	}

	color = ImColor::HSV(hue >= 1 ? hue - 10 * 0.000001f : hue, saturation > 0 ? saturation : 10 * 0.000001f, value > 0 ? value : 0.000001f);
	pCol[0] = color.Value.x;
	pCol[1] = color.Value.y;
	pCol[2] = color.Value.z;
	ImGui::PushItemWidth((bAlphaBar ? spacing + hue_picker_width : 0) + sv_picker_size.x + spacing + hue_picker_width - 2 * ImGui::GetStyle().FramePadding.x);

	const auto widget_used = bAlphaBar ? ImGui::ColorEdit4("", pCol) : ImGui::ColorEdit3("", pCol);
	ImGui::PopItemWidth();

	float new_hue, new_sat, new_val;
	ImGui::ColorConvertRGBtoHSV(pCol[0], pCol[1], pCol[2], new_hue, new_sat, new_val);

	if (new_hue <= 0 && hue > 0)
	{
		if (new_val <= 0 && value != new_val)
		{
			color = ImColor::HSV(hue, saturation, new_val <= 0 ? value * 0.5f : new_val);
			pCol[0] = color.Value.x;
			pCol[1] = color.Value.y;
			pCol[2] = color.Value.z;
		}
		else if (new_sat <= 0)
		{
			color = ImColor::HSV(hue, new_sat <= 0 ? saturation * 0.5f : new_sat, new_val);
			pCol[0] = color.Value.x;
			pCol[1] = color.Value.y;
			pCol[2] = color.Value.z;
		}
	}

	return value_changed | widget_used;
}

bool ImGuiEx::ColorPicker3(float Col[3])
{
	return ColorPicker(Col, false);
}

bool ImGuiEx::ColorPicker4(float Col[4])
{
	return ColorPicker(Col, true);
}

struct ColorPickerClipBoardData_t
{
	ColorPickerClipBoardData_t(ImColor col, float flRainbowSpeed, bool bRainbow)
	{
		m_colColor = col;
		m_flRainbowSpeed = flRainbowSpeed;
		m_bRainbow = bRainbow;
	}

	ImColor m_colColor;
	float m_flRainbowSpeed;
	bool m_bRainbow;
};

static ColorPickerClipBoardData_t g_ColorClibBoard = ColorPickerClipBoardData_t(ImColor(-1.f, -1.f, -1.f, -1.f), -1.f, false);

bool ImGuiEx::ColorPicker(const char* szLabel, ColorVar* v, bool bAddLabelNextToButton/* = false*/)
{
	const auto window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	auto& g = *GImGui;
	const auto& style = g.Style;

	ImGui::PushID(szLabel);

	const ImVec4 col_display(static_cast<float>(v->GetColor().r()) / 255.f, static_cast<float>(v->GetColor().g()) / 255.f,
		static_cast<float>(v->GetColor().b()) / 255.f, static_cast<float>(v->GetColor().a()) / 255.f);

	bool bButtonRtn = ImGui::ColorButton(szLabel, col_display);
	if (bAddLabelNextToButton)
	{
		ImGui::SameLine();
		ImGui::Text(szLabel);
	}

	if (bButtonRtn)
		ImGui::OpenPopup(szLabel);

	if (ImGui::BeginPopup(szLabel, ImGuiWindowFlags_NoMove))
	{
		ImGui::Checkbox("Rainbow", &v->m_bRainbow);
		if (v->m_bRainbow)
		{
			ImGui::PushItemWidth(220);
			ImGui::SliderFloat(("##Rainbow"), &v->m_flRainbowSpeed, 0.f, 1.f, ("Rainbow Speed: %0.2f"));
			ImGui::PopItemWidth();
		}
		else
			ImGuiEx::ColorPicker4(reinterpret_cast<float*>(v));

		if (ImGui::Button("COPY"))
		{
			g_ColorClibBoard.m_colColor = v->m_colColor;
			g_ColorClibBoard.m_flRainbowSpeed = v->m_flRainbowSpeed;
			g_ColorClibBoard.m_bRainbow = v->m_bRainbow;
		}

		ImGui::SameLine();

		// Make button and check if data is valid
		if (ImGui::Button("PASTE") && g_ColorClibBoard.m_colColor != ImColor(-1.f, -1.f, -1.f, -1.f)
			|| g_ColorClibBoard.m_flRainbowSpeed != -1.f)
		{
			v->m_colColor = g_ColorClibBoard.m_colColor;
			v->m_flRainbowSpeed = g_ColorClibBoard.m_flRainbowSpeed;
			v->m_bRainbow = g_ColorClibBoard.m_bRainbow;
		}

		ImGui::EndPopup();
	}

	ImGui::PopID();

	return true;
}