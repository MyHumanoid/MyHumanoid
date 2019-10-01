#pragma once

#define PROFILER_ENABLED 1

#if PROFILER_ENABLED
#include <chrono>

void ProfilerLog(const char * name, long ms);
void ProfilerPrint();
void ProfilerClear();

struct ProfileScope {
	using Clock = std::chrono::steady_clock;
	using TimePoint = Clock::time_point;
	
	using us = std::chrono::microseconds;
	
	const char * m_name;
	const TimePoint m_start;
	
	ProfileScope(const char * name)
	    : m_name(name)
	    , m_start(Clock::now())
	{}
	
	~ProfileScope()
	{
		TimePoint end = Clock::now();
		ProfilerLog(m_name, std::chrono::duration_cast<us>(end - m_start).count());
	}
};

#define PROFILE auto profScope##__LINE__ = ProfileScope(__func__);

#else

#define PROFILE

#endif
