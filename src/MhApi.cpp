// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2025 Eli2

#include "MhApi.h"

#include "CompositeMorph.h"

static void update() {
	g_grids.calcDists();
	g_grids.applyCompositeMorphTargets();
}

void mh_setAge(float age) {
	float clampedAge = glm::clamp(age, 10.f, 90.f);
	float normAge = (clampedAge - 10.f) / 90.f;
	g_grids.setAgeNorm(normAge);
	update();
}

void mh_setSex(float sex) {
	float clampedSex = glm::clamp(sex, -1.f, 1.f);
	float normSex = (clampedSex + 1.f) / 2.f;
	g_grids.setSexNorm(normSex);
	update();
}
