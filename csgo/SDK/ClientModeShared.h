#pragma once

#include "Vectors.h"

enum ClientFrameStage_e
{
    FRAME_START = 0x0,
    FRAME_NET_UPDATE_START = 0x1,
    FRAME_NET_UPDATE_POSTDATAUPDATE_START = 0x2,
    FRAME_NET_UPDATE_POSTDATAUPDATE_END = 0x3,
    FRAME_NET_UPDATE_END = 0x4,
    FRAME_RENDER_START = 0x5,
    FRAME_RENDER_END = 0x6,
    FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE = 0x7,
    FRAME_UNDEFINED = 0xFFFFFFFF,
};

class CViewSetup
{
public:
    int m_nX;
    int m_nUnscaledX;
    int m_nY;
    int m_nUnscaledY;
    int m_nWidth;
    int m_nUnscaledWidth;
    int m_nHeight;
    int m_nUnscaledHeight;
    bool m_bOrtho;
    float m_flOrthoLeft;
    float m_flOrthoTop;
    float m_flOrthoRight;
    float m_flOrthoBottom;
    char pad0[124];
    float m_flFOV;
    float m_flViewModelFOV;
    Vector m_vecOrigin;
    QAngle m_angView;
    float m_flNearZ;
    float m_flFarZ;
    float m_flNearViewmodelZ;
    float m_flFarViewmodelZ;
    float m_flAspectRatio;
    float m_flNearBlurDepth;
    float m_flNearFocusDepth;
    float m_flFarFocusDepth;
    float m_flFarBlurDepth;
    float m_flNearBlurRadius;
    float m_flFarBlurRadius;
    float m_flDoFQuality;
    int m_nMotionBlurMode;
    float m_flShutterTime;
    Vector m_vecShutterOpenPosition;
    QAngle m_vecShutterOpenAngles;
    Vector m_vecShutterClosePosition;
    QAngle m_vecShutterCloseAngles;
    float m_flOffCenterTop;
    float m_flOffCenterBottom;
    float m_flOffCenterLeft;
    float m_flOffCenterRight;
    bool m_bOffCenter;
    bool m_bRenderToSubrectOfLargerScreen;
    bool m_bDoBloomAndToneMapping;
    bool m_bDoDepthOfField;
    bool m_bHDRTarget1;
    bool m_bDrawWorldNormal;
    bool m_bCullFontFaces;
    bool m_bCacheFullSceneState;
    bool m_bCSMView;
};


class ClientModeShared
{
public:

};