#pragma once

class QAngle;
class CUserCmd;

namespace Features
{
	namespace Misc
	{
		void ClanTag();

		void BunnyHop();

		void AutoStrafer();
		void RotateMovement(float flYawToRotateTowards);
		float GetIdealRotation(float flSpeed);
		float GetDegreeFromVelocity(float flSpeed);

		void CorrectMovement();
	}
}