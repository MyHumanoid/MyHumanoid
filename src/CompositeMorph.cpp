#include "CompositeMorph.h"

#include <array>
#include "Global.h"

using std::string;


static std::array<string, 10> ageLabels = {
    "female_10",
    "female_30",
    "female_50",
    "female_70",
    "female_90",
    "male_10",
    "male_30",
    "male_50",
    "male_70",
    "male_90"
};

static std::array<string, 4> muscleSizeLabels = {
    "skinny_nomuscle",
    "big_nomuscle",
    "skinny_muscle",
    "big_muscle"
};

static std::array<string, 4> breastLabels = {
    "cone_little",
    "cone_big",
    "sphere_little",
    "sphere_big"
};

static std::array<string, 4> shapeLabels = {
    "brevilinear_vshape",
    "brevilinear_peershape",
    "longilinear_vshape",
    "longilinear_peershape"
};


void Grid::calcPoints(glm::ivec2 size, int inRows, int inCols)
{
	rows = inRows;
	cols = inCols;
	
	points.clear();
	
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			glm::ivec2 tmp(j * size.x / (cols - 1),
			               i * size.y / (rows - 1));
			points.push_back(tmp);
		}
	}
	
	float cellWidth  = size.x / (cols - 1);
	float cellHeight = size.y / (rows - 1);
	
	cellRatio = cellWidth / cellHeight;
	maxValue  = glm::min(cellWidth, cellHeight * cellRatio);
}

std::vector<float> Grid::calculateDists(glm::ivec2 cursorPos) const
{
	std::vector<float> ret;
	
	for(const auto & vp_it : points) {
		const glm::ivec2 & tmp(vp_it);
		
		float dist  = sqrt(pow(tmp.x - cursorPos.x, 2) +
                          pow((tmp.y - cursorPos.y) * cellRatio, 2));
		float value = 1 - (dist / maxValue);
		if(value > 0) {
			ret.push_back(value);
		} else {
			ret.push_back(0.0f);
		}
	}
	
	return ret;
}

void Grids::calcDists()
{
	ageDists = ageGrid.calculateDists(agePos);
	breastDists = breastGrid.calculateDists(breastPos);
	muscleSizeDists = muscleSizeGrid.calculateDists(muscleSizePos);
	shapeDists = shapeGrid.calculateDists(shapePos);
}

void Grids::applyCompositeMorphTargets() const
{
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int k = 0;
	
	// std::cout << "--------------------------" << std::endl;
	for(const float & di_it : ageDists) {
		if(i < ageLabels.size()) {
			string tmpTargetName("ages/" + ageLabels[i++] + ".target");
			
			g_mesh.setMorphTarget(tmpTargetName, di_it);
		}
	}
	
	for(const float & ms_it : muscleSizeDists) {
		
		i = 0;
		
		for(const float & di_it : ageDists) {
			if(j < muscleSizeLabels.size() && i < ageLabels.size()) {
				string tmpTargetName("muscleSize/" + ageLabels[i] + "_" +
				                     muscleSizeLabels[j] + ".target");
				float  tmpTargetValue = di_it * ms_it;
				
				g_mesh.setMorphTarget(tmpTargetName, tmpTargetValue);
				
				// breast widget
				
				k = 0;
				if(i <= 4) {
					
					for(const float & br_it : breastDists) {
						
						if(k < breastLabels.size()) {
							string tmpTargetName(
							    "breast/" + ageLabels[i] + "_" +
							    muscleSizeLabels[j] + "_" +
							    breastLabels[k] + ".target");
							float tmpTargetValue =
							    di_it * ms_it * br_it;
							
							if(tmpTargetValue > 0) {
								//log_info("{} {}", tmpTargetName, tmpTargetValue);
							}
							
							g_mesh.setMorphTarget(tmpTargetName, tmpTargetValue);
						}
						k++;
					}
				}
			}
			i++;
		}
		j++;
	}
	
	i = 0;
	
	for(const float & sh_it : shapeDists) {
		if(i < shapeLabels.size()) {
			string tmpTargetName("shapes/" + shapeLabels[i++] + ".target");
			
			g_mesh.setMorphTarget(tmpTargetName, sh_it);
		}
	}
}



void Grids::fromSavedPositions(const Animorph::SavedCompSetting & saved)
{
	using glm::ivec2;
	
	constexpr auto size = ivec2(192, 104);
	
	if(saved.m_kAge != InvalidPoint){
		constexpr auto pos = ivec2(0, 10);
		
		agePos = glm::ivec2(saved.m_kAge.x,
		                    pos.y + size.y - saved.m_kAge.y);
	} else {
		agePos = glm::ivec2(96, 52);
	}
	
	
	if(saved.m_kMuscleSize != InvalidPoint){
		constexpr auto pos = ivec2(0, 124);
		
		muscleSizePos = glm::ivec2(saved.m_kAge.x,
		                           pos.y + size.y - saved.m_kMuscleSize.y);
	} else {
		muscleSizePos = glm::ivec2(96, 52);
	}
	
	
	if(saved.m_kBreast != InvalidPoint){
		constexpr auto pos = ivec2(0, 238);
		
		breastPos = glm::ivec2(saved.m_kAge.x,
		                           pos.y + size.y - saved.m_kBreast.y);
	} else {
		breastPos = glm::ivec2(96, 52);
	}
	
	
	if(saved.m_kShape != InvalidPoint){
		constexpr auto pos = ivec2(0, 352);
		
		shapePos = glm::ivec2(saved.m_kAge.x,
		                       pos.y + size.y - saved.m_kShape.y);
	} else {
		shapePos = glm::ivec2(96, 52);
	}
}
