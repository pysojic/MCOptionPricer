// StopWatch.hpp
// 
// Simple timer to record duration
// 
// Pierre-Yves Sojic

#pragma once

#include <chrono>

class StopWatch
{
public:
	StopWatch();
	StopWatch(const StopWatch&) = delete;
	StopWatch& operator=(const StopWatch&) = delete;

	void Start();
	void Stop();
	void Reset();

	double GetTime() const;

private:
	bool m_isRunning = false;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimePoint;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_endTimePoint;
	double m_duration;
};