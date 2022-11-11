#pragma once

#include "VFunc.h"
#include "Vectors.h"
#include "../Utilities/Color.h"

typedef unsigned long HScheme, HPanel, HTexture, HCursor, HFont;

enum FontDrawType_e : int
{
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,
	FONT_DRAW_TYPE_COUNT = 2
};

enum FontFlags_e
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

struct Vertex_t
{
	Vertex_t() = default;

	Vertex_t(const Vector2D& vecPosition, const Vector2D& vecCoordinate = Vector2D(0, 0))
	{
		m_vecPosition = vecPosition;
		m_vecCoordinate = vecCoordinate;
	}

	void Init(const Vector2D& vecPosition, const Vector2D& vecCoordinate = Vector2D(0, 0))
	{
		m_vecPosition = vecPosition;
		m_vecCoordinate = vecCoordinate;
	}

	Vector2D m_vecPosition = { };
	Vector2D m_vecCoordinate = { };
};


class CSurface
{
public:
	void UnLockCursor()
	{
		CallVFunc<void>(this, 66);
	}

	void LockCursor()
	{
		CallVFunc<void>(this, 67);
	}

	void PlaySoundSurface(const char* szFileName)
	{
		CallVFunc<void>(this, 82, szFileName);
	}
};