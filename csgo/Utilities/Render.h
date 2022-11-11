#pragma once
#include <string>
#include <sstream>
#include <stdint.h>
#include <d3d9.h>

#define IMGUI_DEFINE_MATH_OPERATORS

#include "Singleton.h"
#include "../Dependencies/imgui/imgui.h"
#include "../Dependencies/imgui/imgui_internal.h"
#include "../Dependencies/imgui/impl/imgui_impl_dx9.h"
#include "../Dependencies/imgui/impl/imgui_impl_win32.h"

#include "../SDK/Vectors.h"
#include "../Utilities/Color.h"

#include "Fonts.h"

namespace Renderer
{
	void Initialize();
	void GetFonts();
	void ClearDrawList();
	void BeginScene();
	ImDrawList* RenderScene();

	ImU32 GetU32(Color color);

	float Text(const std::string& szText, ImVec2 vecPos, float flSize, Color color, bool bCenter = false, 
		bool bOutline = true, ImFont* pFont = Fonts::m_pDefault);

	void Line(ImVec2 a, ImVec2 b, Color color, float flTickness = 1.f);
	void Line(int x, int y, int x2, int y2, Color color, float flTickness = 1.f);

	void BoundingBox(int nBaseX, int nBaseY, int nWidth, int nHeight,  Color color, float flThickness = 1.f);

	void Circle(ImVec2 vecCenter, float flRadius, Color color, int nSegments = 12, float flThickness = 1.f);
	void CircleFilled(ImVec2 vecCenter, float flRadius, Color color, int nSegments = 12);
	void Circle3D(Vector vecPos, float flPoints, float flRadius, Color color);

	void Image(ImTextureID nUserTextureId, const ImVec2& a, const ImVec2& b, Color color, const ImVec2& uv_a = ImVec2(0, 0),
		const ImVec2& uv_b = ImVec2(1, 1));

	void Rect(ImVec2 a, ImVec2 b, Color color, float flRounding = 0.f, int nRoundingFlags = 15, float flThickness = 1.f);
	void RectFilled(ImVec2 a, ImVec2 b, Color color, float flRounding = 0.f, int nRoundingFlags = 15);

	void PolyLine(const ImVec2* pPoints, const int nPointsCount, Color color, bool bClosed, float flThickness = 1.f, 
		Color FillColor = Color(0, 0, 0, 0));

	bool WorldToScreen(const Vector& vecOrigin, ImVec2& vecScreen);

	// Scale of font based on distance
	float ScaleDistance(float flDistance);

	inline ImDrawList* m_pDrawListAct;
	inline ImDrawList* m_pDrawListRendering;
	inline ImDrawList* m_pDrawList;
	inline ImDrawData m_DrawData;
}