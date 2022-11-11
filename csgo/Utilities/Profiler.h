#pragma once
#include <string>
#include <chrono>

class CTimeProfiler
{
public:
	void Initialize(std::string szProfilerName)
	{
		m_szProfilerName = szProfilerName;
		m_bIsRecording = false;
	}

	void Start()
	{
		m_bIsRecording = true;
		m_StartTime = std::chrono::high_resolution_clock::now();
	}

	void End()
	{
		m_EndTime = std::chrono::high_resolution_clock::now();
		m_bIsRecording = false;

		Print();
	}

	void Print()
	{
		std::cout << m_szProfilerName << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(m_EndTime 
			- m_StartTime).count() << " ms\n";
	}

private:

	std::chrono::high_resolution_clock::time_point m_StartTime;
	std::chrono::high_resolution_clock::time_point m_EndTime;
	bool m_bIsRecording;
	std::string m_szProfilerName;
};

namespace Profiler
{
	inline CTimeProfiler m_CreateMove;
	inline CTimeProfiler m_PaintTraverse;
}