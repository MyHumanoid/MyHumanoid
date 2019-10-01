#pragma once

#include <glm/vec2.hpp>

struct MhConfig {
	using ivec2 = glm::ivec2;
	
	struct Window {
		ivec2 pos;
		ivec2 siz;
	};
	struct ChildWin {
		bool visible = false;
	};
	
	Window windowMain = {
		ivec2(0, 0),
		ivec2(800, 600)
	};
	
	ChildWin characterSettings;
	ChildWin morphTargets;
	ChildWin morphTargetsApplied;
	ChildWin poseTargets;
	ChildWin poseTargetsApplied;
};

extern MhConfig g_config;

void LoadConfig();
void SaveConfig();
