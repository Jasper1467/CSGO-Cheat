#pragma once
#include "Vectors.h"
#include "VFunc.h"
#include "Studio.h"
#include "IMatRenderContext.h"

struct model_t;
struct Studiohdr_t;
class IMaterial;
class IClientRenderable;
typedef unsigned short ModelInstanceHandle_t;
struct ModelRenderInfo_t
{
    Vector                  origin;
    QAngle                  angles;
    char                    pad[4];
    IClientRenderable* pRenderable;
    const model_t* pModel;
    const matrix3x4_t* pModelToWorld;
    const matrix3x4_t* pLightingOffset;
    const Vector* pLightingOrigin;
    int                     flags;
    int                     entity_index;
    int                     skin;
    int                     body;
    int                     hitboxset;
    ModelInstanceHandle_t   instance;

    ModelRenderInfo_t()
    {
        pModelToWorld = NULL;
        pLightingOffset = NULL;
        pLightingOrigin = NULL;
    }
};

struct DrawModelState_t
{
	Studiohdr_t* m_studioHdr;
	void* m_studioHWData;
	void* m_renderable;
	const matrix3x4_t* m_modelToWorld;
	void* m_decals;
	int m_drawFlags;
	int m_lod;
};

typedef void* LightCacheHandle_t;
typedef void* StudioDecalHandle_t;
typedef int OverrideType_t;
typedef int MaterialPropertyTypes_t;

struct StaticPropRenderInfo_t
{
    const matrix3x4_t* pModelToWorld;
    const model_t* pModel;
    IClientRenderable* pRenderable;
    Vector* pLightingOrigin;
    short skin;
    ModelInstanceHandle_t   instance;
};

struct LightingQuery_t
{
    Vector m_LightingOrigin;
    ModelInstanceHandle_t   m_InstanceHandle;
    bool m_bAmbientBoost;
};

struct StaticLightingQuery_t : public LightingQuery_t
{
    IClientRenderable* m_pRenderable;
};

struct MaterialLightingState_t;
class DataCacheHandle_t;
struct ColorMeshInfo_t;
struct DrawModelInfo_t;
class ITexture;
struct Ray_t;
class IVModelRender
{
public:
	void ForcedMaterialOverride(IMaterial* pMaterial, int overrideType = 0, int nOverrides = 0)
	{
		return CallVFunc<void>(this, 1, pMaterial, overrideType, nOverrides);
	}

	bool IsForcedMaterialOverride()
	{
		return CallVFunc<bool>(this, 2);
	}
};