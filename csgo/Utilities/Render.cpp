#include "Render.h"

#include "InputSystem.h"
#include "Interfaces.h"
#include "Client.h"
#include "../Features/Visuals.h"
#include "../Features/Events.h"
#include "../Features/GrenadePrediction.h"
#include "../Menu/Config/Vars.h"

#include "ByteArrays/Droid.h"
#include "ByteArrays/WeaponIcons.h"

#include <mutex>
#include "Math.h"
#include "Utilities.h"

ImDrawListSharedData _data;

std::mutex render_mutex;

void Renderer::Initialize()
{
	ImGui::CreateContext();

	ImGui_ImplWin32_Init(InputSystem::hWindow);
	ImGui_ImplDX9_Init(Interfaces::m_pDevice);

	m_pDrawList = new ImDrawList(ImGui::GetDrawListSharedData());
	m_pDrawListAct = new ImDrawList(ImGui::GetDrawListSharedData());
	m_pDrawListRendering = new ImDrawList(ImGui::GetDrawListSharedData());

	GetFonts();
}

void Renderer::GetFonts() 
{
	Fonts::m_pDefault = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(ByteArrays::m_nDroidCompressedData,
		ByteArrays::m_nDroidCompressedSize, 24.f);

	ImFontConfig WeaponIconsConfig;
	constexpr ImWchar IconRanges[] =
	{
		0xE000, 0xF8FF, // Private Use Area
		0
	};

	Fonts::m_pWeaponIcons = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(ByteArrays::m_nWeaponIconsCompressedData,
		ByteArrays::m_nWeaponIconsCompressedSize, 40.f, &WeaponIconsConfig, IconRanges);
}

void Renderer::ClearDrawList()
{
	render_mutex.lock();
	m_pDrawListAct->Clear();
	render_mutex.unlock();
}

void Renderer::BeginScene() 
{
	m_pDrawList->Clear();
	m_pDrawList->PushClipRectFullScreen();

	if (Interfaces::m_pEngine->IsInGame())
	{
		Features::Visuals::AddToDrawList();
		Features::Events::Draw();
		Features::GrenadePrediction::Draw();
	}

	RectFilled(ImVec2(Client::m_nScreenSizeX, 0.f), ImVec2(Client::m_nScreenSizeX - 250.f, 30.f), Color(0, 0, 0, 255));
	Text(Utilities::Format("%s | csgo cheat", Client::m_szUserName), ImVec2(Client::m_nScreenSizeX - 230.f, 6.f), 16.f,
		Color(255, 255, 255, 255));

	render_mutex.lock();
	*m_pDrawListAct = *m_pDrawList;
	render_mutex.unlock();
}

ImDrawList* Renderer::RenderScene()
{
	if (render_mutex.try_lock()) 
	{
		*m_pDrawListRendering = *m_pDrawListAct;
		render_mutex.unlock();
	}

	return m_pDrawListRendering;
}

ImU32 Renderer::GetU32(Color color)
{
	return ((color[3] & 0xff) << 24) + ((color[2] & 0xff) << 16) + ((color[1] & 0xff) << 8)
		+ (color[0] & 0xff);
}

float Renderer::Text(const std::string& szText, ImVec2 vecPos, float flSize, Color color, bool bCenter, bool bOutline, ImFont* pFont)
{
	ImVec2 vecTextSize = pFont->CalcTextSizeA(flSize, FLT_MAX, 0.0f, szText.c_str());
	if (!pFont->ContainerAtlas)
		return 0.f;

	m_pDrawList->PushTextureID(pFont->ContainerAtlas->TexID);

	if (bCenter)
		vecPos.x -= vecTextSize.x / 2.0f;

	if (bOutline) 
	{
		m_pDrawList->AddText(pFont, flSize, ImVec2(vecPos.x + 1, vecPos.y + 1), GetU32(Color(0, 0, 0, 255)), szText.c_str());
		m_pDrawList->AddText(pFont, flSize, ImVec2(vecPos.x - 1, vecPos.y - 1), GetU32(Color(0, 0, 0, 255)), szText.c_str());
		m_pDrawList->AddText(pFont, flSize, ImVec2(vecPos.x + 1, vecPos.y - 1), GetU32(Color(0, 0, 0, 255)), szText.c_str());
		m_pDrawList->AddText(pFont, flSize, ImVec2(vecPos.x - 1, vecPos.y + 1), GetU32(Color(0, 0, 0, 255)), szText.c_str());
	}

	m_pDrawList->AddText(pFont, flSize, vecPos, GetU32(color), szText.c_str());

	m_pDrawList->PopTextureID();

	return vecPos.y + vecTextSize.y;
}

void Renderer::Line(ImVec2 a, ImVec2 b, Color color, float flTickness)
{
	m_pDrawList->AddLine(a, b, GetU32(color), flTickness);
}

void Renderer::Line(int x, int y, int x2, int y2, Color color, float flTickness)
{
	Line(ImVec2(x, y), ImVec2(x2, y2), color, flTickness);
}

void Renderer::BoundingBox(int nBaseX, int nBaseY, int nWidth, int nHeight, Color color, float flThickness)
{
	Line(nBaseX - nWidth / 2, nBaseY, nBaseX + nWidth / 2, nBaseY, color, flThickness);
	Line(nBaseX - nWidth / 2, nBaseY - nHeight, nBaseX - nWidth, nBaseY, color, flThickness);
	Line(nBaseX + nWidth / 2, nBaseY - nHeight, nBaseX + nWidth, nBaseY, color, flThickness);
	Line(nBaseX - nWidth / 2, nBaseY - nHeight, nBaseX + nWidth, nBaseY - nHeight, color, flThickness);
}

void Renderer::Circle(ImVec2 vecCenter, float flRadius, Color color, int nSegments, float flThickness)
{
	m_pDrawList->AddCircle(vecCenter, flRadius, GetU32(color), nSegments, flThickness);
}

void Renderer::CircleFilled(ImVec2 vecCenter, float flRadius, Color color, int nSegments)
{
	m_pDrawList->AddCircleFilled(vecCenter, flRadius, GetU32(color), nSegments);
}

void Renderer::Circle3D(Vector vecPos, float flPoints, float flRadius, Color color)
{
	float step = (float)M_PI * 2.0f / flPoints;

	for (float a = 0; a < (M_PI * 2.0f); a += step)
	{
		Vector start(flRadius * cosf(a) + vecPos.x, flRadius * sinf(a) + vecPos.y, vecPos.z);
		Vector end(flRadius * cosf(a + step) + vecPos.x, flRadius * sinf(a + step) + vecPos.y, vecPos.z);

		Vector start2d, end2d;
		if (Interfaces::m_pDebugOverlay->ScreenPosition(start, start2d)
			|| Interfaces::m_pDebugOverlay->ScreenPosition(end, end2d))
			return;

		Line(ImVec2(start2d.x, start2d.y), ImVec2(end2d.x, end2d.y), color);
	}
}

void Renderer::Image(ImTextureID nUserTextureId, const ImVec2& a, const ImVec2& b, Color color, const ImVec2& uv_a, 
	const ImVec2& uv_b)
{
	m_pDrawList->AddImage(nUserTextureId, a, b, uv_a, uv_b, GetU32(color));
}

void Renderer::Rect(ImVec2 a, ImVec2 b, Color color, float flRounding, int nRoundingFlags, float flThickness)
{
	m_pDrawList->AddRect(a, b, GetU32(color), flRounding, nRoundingFlags, flThickness);
}

void Renderer::RectFilled(ImVec2 a, ImVec2 b, Color color, float flRounding, int nRoundingFlags)
{
	m_pDrawList->AddRectFilled(a, b, GetU32(color), flRounding, nRoundingFlags);
}

void Renderer::PolyLine(const ImVec2* pPoints, const int nPointsCount, Color color, bool bClosed, float flThickness, Color FillColor)
{
	m_pDrawList->AddConvexPolyFilled(pPoints, nPointsCount, GetU32(FillColor));
	m_pDrawList->AddPolyline(pPoints, nPointsCount, GetU32(color), bClosed, flThickness);
}

bool Renderer::WorldToScreen(const Vector& vecOrigin, ImVec2& vecScreen)
{
	const VMatrix& matWorldToScreen = Interfaces::m_pEngine->WorldToScreenMatrix();
	const float flWidth = matWorldToScreen[3][0] * vecOrigin.x + matWorldToScreen[3][1] * vecOrigin.y 
		+ matWorldToScreen[3][2] * vecOrigin.z + matWorldToScreen[3][3];

	if (flWidth < 0.001f)
		return false;

	const float flInverse = 1.0f / flWidth;

	vecScreen.x = (matWorldToScreen[0][0] * vecOrigin.x + matWorldToScreen[0][1] * vecOrigin.y 
		+ matWorldToScreen[0][2] * vecOrigin.z + matWorldToScreen[0][3]) * flInverse;

	vecScreen.y = (matWorldToScreen[1][0] * vecOrigin.x + matWorldToScreen[1][1] * vecOrigin.y 
		+ matWorldToScreen[1][2] * vecOrigin.z + matWorldToScreen[1][3]) * flInverse;

	const ImVec2 vecDisplaySize = ImGui::GetIO().DisplaySize;
	vecScreen.x = (vecDisplaySize.x * 0.5f) + (vecScreen.x * vecDisplaySize.x) * 0.5f;
	vecScreen.y = (vecDisplaySize.y * 0.5f) - (vecScreen.y * vecDisplaySize.y) * 0.5f;
	return true;
}

float Renderer::ScaleDistance(float flDistance)
{
	return std::clamp(90.f / (flDistance / 90.f), 10.f, 40.f);
}