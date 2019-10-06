#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "animorph/BodySettings.h"

struct Grid
{
	int rows = 2;
	int cols = 2;
	
	float maxValue;
	float cellRatio;
	
	// from left-bottom point to right-up point
	std::vector<glm::ivec2> points;
	
	void calcPoints(glm::ivec2 size, int inRows, int inCols);
	std::vector<float> calculateDists(glm::ivec2 cursorPos) const;
};

struct Grids {
	glm::ivec2 agePos;
	glm::ivec2 muscleSizePos;
	glm::ivec2 breastPos;
	glm::ivec2 shapePos;
	
	Grid ageGrid;
	Grid muscleSizeGrid;
	Grid breastGrid;
	Grid shapeGrid;
	
	std::vector<float> ageDists;
	std::vector<float> muscleSizeDists;
	std::vector<float> breastDists;
	std::vector<float> shapeDists;
	
	Grids()
	{
		ageGrid.calcPoints(glm::ivec2(192, 104), 2, 5);
		muscleSizeGrid.calcPoints(glm::ivec2(192, 104), 2, 2);
		breastGrid.calcPoints(glm::ivec2(192, 104), 2, 2);
		shapeGrid.calcPoints(glm::ivec2(192, 104), 2, 2);
	}
	
	void calcDists();
	void applyCompositeMorphTargets() const;
	
	
	
	void fromSavedPositions(const Animorph::SavedCompSetting & saved);
};
