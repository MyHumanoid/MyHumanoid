// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include "Global.h"

extern bool g_userRequestedQuit;
extern bool g_userAcceptedQuit;

void CreateCaractersIconTextures();
void DisplayMainMenu(AppState &app);

void ExecuteDeferredActions();
