#pragma once

#include "IMaterial.h"
#include "VFunc.h"
#include "IMatRenderContext.h"

enum CreateRenderTargetFlags_e : unsigned int
{
	CREATERENDERTARGETFLAGS_HDR = 0x00000001,
	CREATERENDERTARGETFLAGS_AUTOMIPMAP = 0x00000002,
	CREATERENDERTARGETFLAGS_UNFILTERABLE_OK = 0x00000004,
	CREATERENDERTARGETFLAGS_NOEDRAM = 0x00000008,
	CREATERENDERTARGETFLAGS_TEMP = 0x00000010
};

enum TextureFlags_e : unsigned int
{
	TEXTUREFLAGS_POINTSAMPLE = 0x00000001,
	TEXTUREFLAGS_TRILINEAR = 0x00000002,
	TEXTUREFLAGS_CLAMPS = 0x00000004,
	TEXTUREFLAGS_CLAMPT = 0x00000008,
	TEXTUREFLAGS_ANISOTROPIC = 0x00000010,
	TEXTUREFLAGS_HINT_DXT5 = 0x00000020,
	TEXTUREFLAGS_SRGB = 0x00000040,
	TEXTUREFLAGS_NORMAL = 0x00000080,
	TEXTUREFLAGS_NOMIP = 0x00000100,
	TEXTUREFLAGS_NOLOD = 0x00000200,
	TEXTUREFLAGS_ALL_MIPS = 0x00000400,
	TEXTUREFLAGS_PROCEDURAL = 0x00000800,
	TEXTUREFLAGS_ONEBITALPHA = 0x00001000,
	TEXTUREFLAGS_EIGHTBITALPHA = 0x00002000,
	TEXTUREFLAGS_ENVMAP = 0x00004000,
	TEXTUREFLAGS_RENDERTARGET = 0x00008000,
	TEXTUREFLAGS_DEPTHRENDERTARGET = 0x00010000,
	TEXTUREFLAGS_NODEBUGOVERRIDE = 0x00020000,
	TEXTUREFLAGS_SINGLECOPY = 0x00040000,
	TEXTUREFLAGS_UNUSED_00080000 = 0x00080000,
	TEXTUREFLAGS_UNUSED_00100000 = 0x00100000,
	TEXTUREFLAGS_UNUSED_00200000 = 0x00200000,
	TEXTUREFLAGS_UNUSED_00400000 = 0x00400000,
	TEXTUREFLAGS_NODEPTHBUFFER = 0x00800000,
	TEXTUREFLAGS_UNUSED_01000000 = 0x01000000,
	TEXTUREFLAGS_CLAMPU = 0x02000000,
	TEXTUREFLAGS_VERTEXTEXTURE = 0x04000000,
	TEXTUREFLAGS_SSBUMP = 0x08000000,
	TEXTUREFLAGS_UNUSED_10000000 = 0x10000000,
	TEXTUREFLAGS_BORDER = 0x20000000,
	TEXTUREFLAGS_UNUSED_40000000 = 0x40000000,
	TEXTUREFLAGS_UNUSED_80000000 = 0x80000000
};

enum ClearFlags_e : unsigned int
{
	VIEW_CLEAR_COLOR = 0x1,
	VIEW_CLEAR_DEPTH = 0x2,
	VIEW_CLEAR_FULL_TARGET = 0x4,
	VIEW_NO_DRAW = 0x8,
	VIEW_CLEAR_OBEY_STENCIL = 0x10,
	VIEW_CLEAR_STENCIL = 0x20
};

enum RenderTargetSizeMode_e
{
	RT_SIZE_NO_CHANGE = 0,					// only allowed for render targets that don't want a depth buffer
	RT_SIZE_DEFAULT,						// don't play with the specified width and height other than making sure it fits in the framebuffer.
	RT_SIZE_PICMIP,							// apply picmip to the render target's width and height.
	RT_SIZE_HDR,							// frame_buffer_width / 4
	RT_SIZE_FULL_FRAME_BUFFER,				// same size as frame buffer, or next lower power of 2 if we can't do that.
	RT_SIZE_OFFSCREEN,						// target of specified size, don't mess with dimensions
	RT_SIZE_FULL_FRAME_BUFFER_ROUNDED_UP,	// same size as the frame buffer, rounded up if necessary for systems that can't do non-power of two textures.
	RT_SIZE_REPLAY_SCREENSHOT,				// rounded down to power of 2, essentially...
	RT_SIZE_LITERAL
};

enum MaterialRenderTargetDepth_e : unsigned int
{
	MATERIAL_RT_DEPTH_SHARED = 0x0,
	MATERIAL_RT_DEPTH_SEPARATE = 0x1,
	MATERIAL_RT_DEPTH_NONE = 0x2,
	MATERIAL_RT_DEPTH_ONLY = 0x3
};

class CMaterialSystem
{
public:
	ImageFormat_e GetBackBufferFormat()
	{
		return CallVFunc<ImageFormat_e>(this, 36);
	}

	IMaterial* CreateMaterial(const char* szName, KeyValues* pKeyValues)
	{
		return CallVFunc<IMaterial*>(this, 83, szName, pKeyValues);
	}

	IMaterial* FindMaterial(char const* szMaterialName, const char* szTextureGroupName = TEXTURE_GROUP_MODEL, bool bComplain = true, const char* pComplainPrefix = nullptr)
	{
		return CallVFunc<IMaterial*>(this, 84, szMaterialName, szTextureGroupName, bComplain, pComplainPrefix);
	}

	MaterialHandle_t FirstMaterial()
	{
		return CallVFunc<MaterialHandle_t>(this, 86);
	}

	MaterialHandle_t NextMaterial(MaterialHandle_t hMaterial)
	{
		return CallVFunc<MaterialHandle_t>(this, 87, hMaterial);
	}

	MaterialHandle_t InvalidMaterial()
	{
		return CallVFunc<MaterialHandle_t>(this, 88);
	}

	IMaterial* GetMaterial(MaterialHandle_t hMaterial)
	{
		return CallVFunc<IMaterial*>(this, 89, hMaterial);
	}

	int	GetNumMaterials()
	{
		return CallVFunc<int>(this, 90);
	}

	ITexture* FindTexture(char const* szTextureName, const char* szTextureGroupName, bool bComplain = true, int nAdditionalCreationFlags = 0)
	{
		return CallVFunc<ITexture*>(this, 91, szTextureName, szTextureGroupName, bComplain, nAdditionalCreationFlags);
	}

	ITexture* CreateNamedRenderTargetTextureEx(const char* name, int w, int h, RenderTargetSizeMode_e sizeMode, ImageFormat_e format,
		MaterialRenderTargetDepth_e depth, unsigned int textureFlags, unsigned int renderTargetFlags)
	{
		return CallVFunc<ITexture*>(this, 97, name, w, h, sizeMode, format, depth, textureFlags, renderTargetFlags);
	}

	void BeginRenderTargetAllocation()
	{
		CallVFunc<void>(this, 94);
	}

	void EndRenderTargetAllocation()
	{
		CallVFunc<void>(this, 95);
	}

	IMatRenderContext* GetRenderContext()
	{
		return CallVFunc<IMatRenderContext*>(this, 115);
	}

	ITexture* CreateFullFrameRenderTarget(const char* name)
	{
		return CreateNamedRenderTargetTextureEx(
			name, 1, 1, RT_SIZE_FULL_FRAME_BUFFER,
			IMAGE_FORMAT_RGB888, MATERIAL_RT_DEPTH_SHARED,
			TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT,
			CREATERENDERTARGETFLAGS_HDR);
	}

	void ForceBeginRenderTargetAllocation()
	{
		const bool bOldState = DisableRenderTargetAllocationForever();

		DisableRenderTargetAllocationForever() = false;
		BeginRenderTargetAllocation();
		DisableRenderTargetAllocationForever() = bOldState;
	}

	void ForceEndRenderTargetAllocation()
	{
		const bool bOldState = DisableRenderTargetAllocationForever();

		DisableRenderTargetAllocationForever() = false;
		EndRenderTargetAllocation();
		DisableRenderTargetAllocationForever() = bOldState;
	}

	bool& DisableRenderTargetAllocationForever();
};