#pragma once

#include "../SDK/Vectors.h"

#include <Windows.h>
#include <d3d9.h>
#include <vector>

class CPlayer;
class CEntity;
class CWeapon;

class CViewSetup;
class ITexture;
class IGameEvent;

struct Box_t
{
	float m_flLeft;
	float m_flTop;
	float m_flRight;
	float m_flBottom;
	float m_flWidth;
	float m_flHeight;
};

namespace Features
{
	namespace Visuals
	{
		namespace Players
		{
			void RenderBox(CPlayer* pPlayer);
			void RenderFlags(CPlayer* pPlayer);
			void RenderSnapLines(CPlayer* pPlayer);

			void Glow();

			inline float m_flFontSizeScaled[65];
		}

		namespace World
		{
			
		}

		namespace Screen
		{
			void RenderCrosshair();
			void RenderNoScope();
		}

		namespace MirrorCam
		{
			void Initialize();

			void Draw();
			void RenderCustomView(const CViewSetup& View);

			IDirect3DTexture9* GetTexture();

			inline Vector2D m_vecSize = Vector2D(1.f, 1.f);
			inline ITexture* m_pTexture;
			inline bool m_bInitialized = false;
		}

		void DrawHitMarker();
		void OnEvent(IGameEvent* pEvent);

		void AddToDrawList();
	}
}