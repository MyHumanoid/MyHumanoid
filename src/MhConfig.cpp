#include "MhConfig.h"

#include <physfs.h>

#include "log/log.h"
#include "Global.h"

#include "SimpleIni.h"

#include "Vfs.h"

MhConfig g_config;

static const char fileName[] = "MyHumanoid.ini";


struct Section {
	CSimpleIniA & m_ini;
	const std::string m_section;
	Section(CSimpleIniA & ini, const std::string & name)
	    : m_ini(ini)
	    , m_section(name)
	{}
	
	void getVal(bool & field, const char * name) {
		field = m_ini.GetBoolValue(m_section.c_str(), name, field);
	}
	void getVal(int & field, const char * name) {
		field = m_ini.GetLongValue(m_section.c_str(), name, field);
	}
	
	void setVal(bool & field, const char * name) {
		m_ini.SetBoolValue(m_section.c_str(), name, field);
	};
	void setVal(int & field, const char * name) {
		m_ini.SetLongValue(m_section.c_str(), name, field);
	};
};

void LoadConfig()
{
	CSimpleIniA ini;
	ini.SetUnicode(true);
	ini.SetMultiKey(true);
	ini.SetMultiLine(true);
	
	std::string data;
	
	if(!vfs::loadString(fileName, data)) {
		log_error("Config data load failed");
		return;
	}
	
	ini.LoadData(data);
	
	auto ok = ini.LoadFile(fileName);
	if(ok < 0) {
		log_error("Config load failed");
		return;
	} else {
		log_info("Config loaded");
	}
	
	
	{
		Section s(ini, "window-main");
		s.getVal(g_config.windowMain.pos.x, "pos-x");
		s.getVal(g_config.windowMain.pos.y, "pos-y");
		s.getVal(g_config.windowMain.siz.x, "siz-x");
		s.getVal(g_config.windowMain.siz.y, "siz-y");
	}
	
	{
		Section s(ini, "window-open");
		s.getVal(g_config.characterSettings.visible,   "characterSettings.visible");
		s.getVal(g_config.morphTargets.visible,        "morphTargets.visible");
		s.getVal(g_config.morphTargetsApplied.visible, "morphTargetsApplied.visible");
		s.getVal(g_config.poseTargets.visible,         "poseTargets.visible");
		s.getVal(g_config.poseTargetsApplied.visible,  "poseTargetsApplied.visible");
	}
}

void SaveConfig()
{
	CSimpleIniA ini;
	ini.SetUnicode(true);
	ini.SetMultiKey(true);
	ini.SetMultiLine(true);
	
	{
		Section s(ini, "window-main");
		s.setVal(g_config.windowMain.pos.x, "pos-x");
		s.setVal(g_config.windowMain.pos.y, "pos-y");
		s.setVal(g_config.windowMain.siz.x, "siz-x");
		s.setVal(g_config.windowMain.siz.y, "siz-y");
	}
	
	{
		Section s(ini, "window-open");
		s.setVal(g_config.characterSettings.visible,   "characterSettings.visible");
		s.setVal(g_config.morphTargets.visible,        "morphTargets.visible");
		s.setVal(g_config.morphTargetsApplied.visible, "morphTargetsApplied.visible");
		s.setVal(g_config.poseTargets.visible,         "poseTargets.visible");
		s.setVal(g_config.poseTargetsApplied.visible,  "poseTargetsApplied.visible");
	}
	
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
