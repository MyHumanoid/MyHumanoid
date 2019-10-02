#include "MhConfig.h"

#include <physfs.h>

#include "log/log.h"
#include "Global.h"

#include "SimpleIni.h"

#include "Vfs.h"

MhConfig g_config;

static const char fileName[] = "MyHumanoid.ini";


void LoadConfig()
{
	CSimpleIniA ini;
	ini.SetUnicode(true);
	ini.SetMultiKey(true);
	ini.SetMultiLine(true);
	
	vfs::loadString(fileName, [&ini](auto & data)->SI_Error{
		return ini.LoadData(data);
	});
	
	auto ok = ini.LoadFile(fileName);
	if(ok < 0) {
		log_error("Config load failed");
		return;
	} else {
		log_info("Config loaded");
	}
	
	std::string section;
	auto getVal = [&ini, &section](auto & field, const auto & name) {
		if constexpr(std::is_same<typeof field, bool>::value) {
			field = ini.GetBoolValue(section.c_str(), name, field);
		} else {
			field = ini.GetLongValue(section.c_str(), name, field);
		}
	};
	
	section = "window-main";
	getVal(g_config.windowMain.pos.x, "pos-x");
	getVal(g_config.windowMain.pos.y, "pos-y");
	getVal(g_config.windowMain.siz.x, "siz-x");
	getVal(g_config.windowMain.siz.y, "siz-y");
	
	section = "window-open";
	getVal(g_config.characterSettings.visible,   "characterSettings.visible");
	getVal(g_config.morphTargets.visible,        "morphTargets.visible");
	getVal(g_config.morphTargetsApplied.visible, "morphTargetsApplied.visible");
	getVal(g_config.poseTargets.visible,         "poseTargets.visible");
	getVal(g_config.poseTargetsApplied.visible,  "poseTargetsApplied.visible");
}

void SaveConfig()
{
	CSimpleIniA ini;
	ini.SetUnicode(true);
	ini.SetMultiKey(true);
	ini.SetMultiLine(true);
	
	std::string section;
	auto setVal = [&ini, &section](const auto & field, const auto & name) {
		if constexpr(std::is_same<typeof field, bool>::value) {
			ini.SetBoolValue(section.c_str(), name, field);
		} else {
			ini.SetLongValue(section.c_str(), name, field);
		}
	};
	
	section = "window-main";
	setVal(g_config.windowMain.pos.x, "pos-x");
	setVal(g_config.windowMain.pos.y, "pos-y");
	setVal(g_config.windowMain.siz.x, "siz-x");
	setVal(g_config.windowMain.siz.y, "siz-y");
	
	section = "window-open";
	setVal(g_config.characterSettings.visible,   "characterSettings.visible");
	setVal(g_config.morphTargets.visible,        "morphTargets.visible");
	setVal(g_config.morphTargetsApplied.visible, "morphTargetsApplied.visible");
	setVal(g_config.poseTargets.visible,         "poseTargets.visible");
	setVal(g_config.poseTargetsApplied.visible,  "poseTargetsApplied.visible");
	
	std::string buffer;
	ini.Save(buffer);
	
	vfs::writeString(fileName, buffer);
	
//	auto ok = ini.SaveFile(fileName);
//	if(ok < 0) {
//		log_error("Config save failed");
//	} else {
		log_info("Config saved");
//	}
}
