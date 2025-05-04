#pragma once

#include <array>
#include <vector>
#include <glm/glm.hpp>
#include "animorph/BodySettings.h"
#include "animorph/Mesh.h"


constexpr glm::vec2 foobarScale = glm::vec2(192, 104);

template<int rows, int cols>
struct Grid
{
	static constexpr auto flatSize = rows * cols;
	
	float maxValue;
	float cellRatio;
	
	// from left-bottom point to right-up point
	std::array<glm::ivec2, flatSize> points;
	std::array<float, flatSize> dists;
	
	Grid()
	{
		auto size = glm::ivec2(192, 104);
		
		for(int y = 0; y < rows; y++) {
			for(int x = 0; x < cols; x++) {
				glm::ivec2 tmp(x * size.x / (cols - 1),
				               y * size.y / (rows - 1));
				
				points[y * cols + x] = tmp;
			}
		}
		
		float cellWidth  = float(size.x) / (cols - 1);
		float cellHeight = float(size.y) / (rows - 1);
		
		cellRatio = cellWidth / cellHeight;
		maxValue  = glm::min(cellWidth, cellHeight * cellRatio);
	}
	
	void calculateDists(glm::vec2 cursorPos)
	{
		glm::vec2 scaledPos = cursorPos * foobarScale;
		
		for(int i = 0; i < (rows * cols); ++i) {
			const glm::ivec2 & tmp = points[i];
			
			float dist  = sqrt(pow(tmp.x - scaledPos.x, 2) +
                              pow((tmp.y - scaledPos.y) * cellRatio, 2));
			float value = 1 - (dist / maxValue);
			if(value > 0) {
				dists[i] = value;
			} else {
				dists[i] = 0.0f;
			}
		}
	}
};

struct Grids {
	void setAgeNorm(float age) {
		agePos.x = age;
	}
	void setSexNorm(float sex) {
		agePos.y = sex;
	}
	
	// XXX auto defPos = glm::vec2(96, 52) / foobarScale;
	glm::vec2 agePos;
	glm::vec2 muscleSizePos;
	glm::vec2 breastPos;
	glm::vec2 shapePos;
	
	Grid<2, 5> ageGrid;
	Grid<2, 2> muscleSizeGrid;
	Grid<2, 2> breastGrid;
	Grid<2, 2> shapeGrid;
	
	void applyCompositeMorphTargets(Animorph::Mesh & mesh);
	
	void fromSavedPositions(const Animorph::SavedCompSetting & saved);
	void toSavedPositions(Animorph::SavedCompSetting & saved);
};

extern Grids g_grids;
