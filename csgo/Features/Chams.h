#pragma once
#include "../SDK/Vectors.h"
#include "../SDK/IVModelRender.h"
#include <vector>
#include "../Utilities/Color.h"

namespace Features
{
	class CChams
	{
	public:
		CChams();
		~CChams();

		void OnDrawModelExecute(
			IMatRenderContext* ctx,
			const DrawModelState_t& state,
			const ModelRenderInfo_t& pInfo,
			matrix3x4_t* pCustomBoneToWorld);
	private:
		void OverrideMaterial(bool ignoreZ, bool flat, bool wireframe, bool glass, const Color& rgba);

		IMaterial* materialRegular = nullptr;
		IMaterial* materialFlat = nullptr;
	};

	inline CChams g_Chams;
}