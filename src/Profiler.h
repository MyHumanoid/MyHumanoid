#pragma once

#include "Logger.h"

#ifdef MH_ENABLE_TRACY

#include "tracy/Tracy.hpp"
#include "GlInclude.h"
#include "tracy/TracyOpenGL.hpp"

inline void printTracingStatus() {
	log_info("Tracing Enabled");
}

#else

#define ZoneScoped
#define FrameMark

#define TracyGpuContext
#define TracyGpuCollect

#define TracyGpuZone(x)

inline void printTracingStatus() {
	log_info("Tracing Disabled");
}

#endif
