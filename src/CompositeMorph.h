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
	
	void calcPoints(glm::ivec2 size)
	{
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
	
	std::array<float, flatSize> dists;
};

struct Grids {
	void setAgeNorm(float age) {
		agePos.x = age;
	}
	void setSexNorm(float sex) {
		agePos.y = sex;
	}
	
	glm::vec2 agePos;
	glm::vec2 muscleSizePos;
	glm::vec2 breastPos;
	glm::vec2 shapePos;
	
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
	
	void clearPos()
	{
		auto defPos = glm::vec2(96, 52);
		
		agePos = defPos / foobarScale;
		muscleSizePos = defPos/ foobarScale;;
		breastPos = defPos / foobarScale;;
		shapePos = defPos / foobarScale;;
	}
	
	void clearDists()
	{
		ageGrid.dists.fill(0.f);
		muscleSizeGrid.dists.fill(0.f);
		breastGrid.dists.fill(0.f);
		shapeGrid.dists.fill(0.f);
	}
	
	void calcDists();
	void applyCompositeMorphTargets(Animorph::Mesh & mesh) const;
	
	
	
	void fromSavedPositions(const Animorph::SavedCompSetting & saved);
	void toSavedPositions(Animorph::SavedCompSetting & saved);
};

extern Grids g_grids;
