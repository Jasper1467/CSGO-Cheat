#include "Math.h"

#include "Interfaces.h"

#include <algorithm>

Vector Math::VectorTransform(const Vector& vecTransform, const matrix3x4_t& matMatrix)
{
    return Vector(vecTransform.Dot(matMatrix[0]) + matMatrix[0][3],
        vecTransform.Dot(matMatrix[1]) + matMatrix[1][3],
        vecTransform.Dot(matMatrix[2]) + matMatrix[2][3]);
}

float Math::NormalizeYaw(float flYaw)
{
    float flNormalizedAngle = 0;

    float flRevolutions = flYaw / 360;

    if (flYaw > 180 || flYaw < -180)
    {
        if (flRevolutions < 0)
            flRevolutions = -flRevolutions;

        flRevolutions = round(flRevolutions);

        if (flYaw < 0)
            flNormalizedAngle = (flYaw + 360 * flRevolutions);
        else
            flNormalizedAngle = (flYaw - 360 * flRevolutions);
    }

    return flNormalizedAngle;
}

float Math::NormalizePitch(float flPitch)
{
    while (flPitch > 89.0f)
        flPitch -= 180.0f;

    while (flPitch < -89.0f)
        flPitch += 180.0f;

    return flPitch;
}

void Math::AngleVectors(const QAngle& angAngles, Vector& vecForward)
{
    float sp, sy, cp, cy;

    DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angAngles[0]));
    DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angAngles[1]));

    vecForward.x = cp * cy;
    vecForward.y = cp * sy;
    vecForward.z = -sp;
}

void Math::AngleVectors(const QAngle& angAngles, Vector& vecForward, Vector& vecRight, Vector& vecUp)
{
    float sr, sp, sy, cr, cp, cy;

    DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angAngles[0]));
    DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angAngles[1]));
    DirectX::XMScalarSinCos(&sr, &cr, DEG2RAD(angAngles[2]));

    vecForward.x = (cp * cy);
    vecForward.y = (cp * sy);
    vecForward.z = (-sp);
    vecRight.x = (-1 * sr * sp * cy + -1 * cr * -sy);
    vecRight.y = (-1 * sr * sp * sy + -1 * cr * cy);
    vecRight.z = (-1 * sr * cp);
    vecUp.x = (cr * sp * cy + -sr * -sy);
    vecUp.y = (cr * sp * sy + -sr * cy);
    vecUp.z = (cr * cp);
}

void Math::VectorAngles(const Vector& vecForward, QAngle& angAngles)
{
    float tmp, yaw, pitch;

    if (vecForward[1] == 0 && vecForward[0] == 0) {
        yaw = 0;
        if (vecForward[2] > 0)
            pitch = 270;
        else
            pitch = 90;
    }
    else {
        yaw = (atan2(vecForward[1], vecForward[0]) * 180 / DirectX::XM_PI);
        if (yaw < 0)
            yaw += 360;

        tmp = sqrt(vecForward[0] * vecForward[0] + vecForward[1] * vecForward[1]);
        pitch = (atan2(-vecForward[2], tmp) * 180 / DirectX::XM_PI);
        if (pitch < 0)
            pitch += 360;
    }

    angAngles[0] = pitch;
    angAngles[1] = yaw;
    angAngles[2] = 0;
}


bool Math::ClampAngles(QAngle& angAngles)
{
    if (isfinite(angAngles.pitch) && isfinite(angAngles.yaw) && isfinite(angAngles.roll))
    {
        std::clamp(angAngles.pitch, -89.f, 89.f);
        std::clamp(angAngles.yaw, -180.f, 180.f);
        angAngles.roll = 0.f;

        return true;
    }

    return false;
}

bool Math::NormalizeAngles(QAngle& angAngles)
{
    if (isfinite(angAngles.pitch) && isfinite(angAngles.yaw) && isfinite(angAngles.roll))
    {
        angAngles.pitch = remainderf(angAngles.pitch, 360.f);
        angAngles.yaw = remainderf(angAngles.pitch, 360.f);

        return true;
    }

    return false;
}

QAngle& Math::CalcAngle(Vector& src, Vector& dst)
{
    QAngle angAngles;
    VectorAngles(dst - src, angAngles);

    return angAngles;
}

float Math::GetFOV(const QAngle& angViewAngle, QAngle& angAimAngle)
{
    Vector vecView, vecAim;
    AngleVectors(angViewAngle, vecView);
    AngleVectors(angAimAngle, vecAim);

    float flRes = RAD2DEG(acos(vecAim.Dot(vecView) / vecAim.LengthSqr()));
    if (isnan(flRes))
        flRes = 0.f;

    return flRes;
}

float Math::GetFOV(Vector vecSource, Vector vecDestination, QAngle& angAimAngle)
{
    Vector vecForward;
    QAngle& angAngle = CalcAngle(vecSource, vecDestination);
    AngleVectors(angAngle, vecForward);

    Vector vecForward2;
    AngleVectors(angAngle, vecForward2);

    return (acosf(vecForward2.Dot(vecForward) / pow(vecForward2.Length(), 2.0f)) * (180.0f / M_PI));
}