#pragma once

namespace Features
{
	namespace AntiAim
	{
		void Run(bool& bSendPacket);

		void Pitch();
		void Yaw(bool& bSendPacket);

		void FakeLag(bool& bSendPacket);
	}
}