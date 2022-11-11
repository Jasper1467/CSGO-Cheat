#pragma once

#include "../SDK/Vectors.h"

#include <DirectXMath.h>

#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)
#define M_PI 3.14159265358979323846
#define PI_F ((float)(M_PI))

namespace Math
{
	Vector VectorTransform(const Vector& vecTransform, const matrix3x4_t& matMatrix);

	float NormalizeYaw(float flYaw);
	float NormalizePitch(float flPitch);

	void AngleVectors(const QAngle& angAngles, Vector& vecForward);
	void AngleVectors(const QAngle& angAngles, Vector& vecForward, Vector& vecRight, Vector& vecUp);

	void VectorAngles(const Vector& vecForward, QAngle& angAngles);

	bool ClampAngles(QAngle& angAngles);
	bool NormalizeAngles(QAngle& angAngles);

	QAngle& CalcAngle(Vector& src, Vector& dst);

	float GetFOV(const QAngle& angViewAngle, QAngle& angAimAngle);
	float GetFOV(Vector vecSource, Vector vecDestination, QAngle& angAimAngle);
}