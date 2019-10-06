#pragma once

#include <array>
#include <vector>
#include <glm/glm.hpp>
#include "animorph/BodySettings.h"

template<int rows, int cols>
struct Grid
{
	static constexpr auto flatSize = rows * cols;
	
	float maxValue;
	float cellRatio;
	
	// from left-bottom point to right-up point
	std::array<glm::ivec2, flatSize> points;
	
	void calcPoints(glm::ivec2 size)
	{
		for(int y = 0; y < rows; y++) {
			for(int x = 0; x < cols; x++) {
				glm::ivec2 tmp(x * size.x / (cols - 1),
				               y * size.y / (rows - 1));
				
				points[y * cols + x] = tmp;
			}
		}
		
		float cellWidth  = size.x / (cols - 1);
		float cellHeight = size.y / (rows - 1);
		
		cellRatio = cellWidth / cellHeight;
		maxValue  = glm::min(cellWidth, cellHeight * cellRatio);
	}
	
	void calculateDists(glm::ivec2 cursorPos)
	{
		for(int i = 0; i < (rows * cols); ++i) {
			const glm::ivec2 & tmp = points[i];
			
			float dist  = sqrt(pow(tmp.x - cursorPos.x, 2) +
                              pow((tmp.y - cursorPos.y) * cellRatio, 2));
			float value = 1 - (dist / maxValue);
			if(value > 0) {
				dists[i] = value;
			} else {
				dists[i] = 0.0f;
			}
		}
	}
	
	std::array<float, flatSize> dists;
};

struct Grids {
	glm::ivec2 agePos;
	glm::ivec2 muscleSizePos;
	glm::ivec2 breastPos;
	glm::ivec2 shapePos;
	
	Grid<2, 5> ageGrid;
	Grid<2, 2> muscleSizeGrid;
	Grid<2, 2> breastGrid;
	Grid<2, 2> shapeGrid;
	
	Grids()
	{
		ageGrid.calcPoints(glm::ivec2(192, 104));
		muscleSizeGrid.calcPoints(glm::ivec2(192, 104));
		breastGrid.calcPoints(glm::ivec2(192, 104));
		shapeGrid.calcPoints(glm::ivec2(192, 104));
	}
	
	void clearDists()
	{
		ageGrid.dists.fill(0.f);
		muscleSizeGrid.dists.fill(0.f);
		breastGrid.dists.fill(0.f);
		shapeGrid.dists.fill(0.f);
	}
	
	void calcDists();
	void applyCompositeMorphTargets() const;
	
	
	
	void fromSavedPositions(const Animorph::SavedCompSetting & saved);
};
