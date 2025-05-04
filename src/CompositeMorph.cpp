#include "CompositeMorph.h"

#include <array>

using std::string;

Grids g_grids;

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

void Grids::applyCompositeMorphTargets(Animorph::Mesh & mesh)
{
	static_assert(std::tuple_size<decltype(ageGrid.dists)>::value ==
	              std::tuple_size<decltype(ageLabels)>::value,
	              "");
	
	static_assert(std::tuple_size<decltype(muscleSizeGrid.dists)>::value ==
	              std::tuple_size<decltype(muscleSizeLabels)>::value,
	              "");
	
	static_assert(std::tuple_size<decltype(breastGrid.dists)>::value ==
	              std::tuple_size<decltype(breastLabels)>::value,
	              "");
	
	static_assert(std::tuple_size<decltype(shapeGrid.dists)>::value ==
	              std::tuple_size<decltype(shapeLabels)>::value,
	              "");
	
	ageGrid.calculateDists(agePos);
	breastGrid.calculateDists(breastPos);
	muscleSizeGrid.calculateDists(muscleSizePos);
	shapeGrid.calculateDists(shapePos);
	
	// std::cout << "--------------------------" << std::endl;
	for(size_t i = 0; i < ageGrid.dists.size(); ++i) {
		
		string tmpTargetName("ages/" + ageLabels[i] + ".target");
		mesh.setMorphTarget(tmpTargetName, ageGrid.dists[i]);
	}
	
	size_t muscle = 0;
	for(const float & ms_it : muscleSizeGrid.dists) {
		size_t age = 0;
		for(const float & di_it : ageGrid.dists) {
			if(muscle < muscleSizeLabels.size() && age < ageLabels.size()) {
				{
					string muscleName = "muscleSize/" + ageLabels[age] + "_" + muscleSizeLabels[muscle] + ".target";
					float  muscleValue = di_it * ms_it;
					mesh.setMorphTarget(muscleName, muscleValue);
				}
				
				if(age <= 4) {
					size_t breast = 0;
					for(const float & br_it : breastGrid.dists) {
						if(breast < breastLabels.size()) {
							string name = "breast/" + ageLabels[age] + "_" + muscleSizeLabels[muscle] + "_" + breastLabels[breast] + ".target";
							float value = di_it * ms_it * br_it;
							
							mesh.setMorphTarget(name, value);
						}
						breast++;
					}
				}
			}
			age++;
		}
		muscle++;
	}
	
	for(size_t shape = 0; shape < shapeGrid.dists.size(); ++shape) {
		string name = "shapes/" + shapeLabels[shape] + ".target";
		float value = shapeGrid.dists[shape];
		mesh.setMorphTarget(name, value);
	}
}



void Grids::fromSavedPositions(const Animorph::SavedCompSetting & saved)
{
	using glm::ivec2;
	using glm::clamp;
	
	int yOff = 16;
	auto size = ivec2(192, 104);
	auto defPos = ivec2(96, 52);
	
	if(saved.m_kAge != InvalidPoint){
		const int bottom = yOff + 10 + size.y;
		
		agePos = clamp(ivec2(saved.m_kAge.x, bottom - saved.m_kAge.y),
		               ivec2(0), size);
	} else {
		agePos = defPos;
	}
	agePos /= foobarScale;
	
	if(saved.m_kMuscleSize != InvalidPoint){
		const int bottom = yOff + 124 + size.y;
		
		muscleSizePos = clamp(ivec2(saved.m_kMuscleSize.x, bottom - saved.m_kMuscleSize.y),
		                      ivec2(0), size);
	} else {
		muscleSizePos = defPos;
	}
	muscleSizePos /= foobarScale;
	
	if(saved.m_kBreast != InvalidPoint){
		const int bottom = yOff + 238 + size.y;
		
		breastPos = clamp(ivec2(saved.m_kBreast.x, bottom - saved.m_kBreast.y),
		                  ivec2(0), size);
	} else {
		breastPos = defPos;
	}
	breastPos /= foobarScale;
	
	if(saved.m_kShape != InvalidPoint){
		const int bottom = yOff + 352 + size.y;
		
		shapePos = clamp(ivec2(saved.m_kShape.x, bottom - saved.m_kShape.y),
		                 ivec2(0), size);
	} else {
		shapePos = defPos;
	}
	shapePos /= foobarScale;
}

void Grids::toSavedPositions(Animorph::SavedCompSetting & saved)
{
	using glm::ivec2;
	
	int yOff = 16;
	auto size = ivec2(192, 104);
	
	glm::vec2 a = agePos * foobarScale;
	glm::vec2 m = muscleSizePos * foobarScale;
	glm::vec2 b = breastPos * foobarScale;
	glm::vec2 s = shapePos * foobarScale;
	
	saved.m_kAge = ivec2(a.x, yOff + 10 + size.y - a.y);
	saved.m_kMuscleSize = ivec2(m.x, yOff + 124 + size.y - m.y);
	saved.m_kBreast = ivec2(b.x, yOff + 238 + size.y - b.y);
	saved.m_kShape = ivec2(s.x, yOff + 352 + size.y - s.y);
}
