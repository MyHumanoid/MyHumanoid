## Process this file with automake to produce Makefile.in

INCLUDES =

AM_CXXFLAGS =\
         @PACKAGE_CFLAGS@\
         @GLUT_CFLAGS@\
	 -Wall 

bin_PROGRAMS = makehuman

if WINDRES
  RESOURCES = makehuman_res.rc
endif

makehuman_SOURCES = \
	${RESOURCES}\
	Animation.cpp\
	Animation.h\
	BodyPanel.cpp\
	BodyPanel.h\
	FacePanel.cpp\
	FacePanel.h\
	TeethPanel.cpp\
	TeethPanel.h\
	HandsPanel.cpp\
	HandsPanel.h\
	FooterPanel.cpp\
	FooterPanel.h\
	ViewPanel.cpp\
	ViewPanel.h\
	TooltipPanel.cpp\
	TooltipPanel.h\
	ImageListener.cpp\
	ImageListener.h\
	makehuman.cpp\
	UtilitybarPanel.cpp\
	UtilitybarPanel.h\
	ConsoleListener.cpp\
	ConsoleListener.h\
	Global.cpp\
	Global.h\
	ConsoleCommands.h\
	TargetSliderListener.cpp\
	TargetSliderListener.h\
	TargetSelectionListener.cpp\
	TargetSelectionListener.h\
	ToolbarPanel.cpp\
	ToolbarPanel.h\
	TargetPanel.cpp\
	TargetPanel.h\
	ComponentID.h\
	TargetSlider.h\
	util.cpp\
	util.h\
	BottomPanel.cpp \
	BottomPanel.h\
	PoseTargetPanel.cpp\
	PoseTargetPanel.h\
	PoseTargetSelectionListener.cpp\
	PoseTargetSelectionListener.h\
	PosesBodyPanel.cpp\
	PosesBodyPanel.h\
	PoseTargetSliderListener.cpp\
	PoseTargetSliderListener.h\
	BsPanel.cpp\
	BsPanel.h\
	BsSelectionListener.cpp\
	BsSelectionListener.h\
	CharacterSettingPanel.cpp\
	CharacterSettingPanel.h\
	ClothesPanel.cpp\
	ClothesPanel.h\
    	PageTargetPanel.cpp\
	PageTargetPanel.h\
	FileTools.cpp\
	FileTools.h\
	SplashPanel.cpp\
	SplashPanel.h\
	UtilitiesPanel.cpp\
	UtilitiesPanel.h\
	UtilitiesSliderListener.cpp\
	UtilitiesSliderListener.h\
	SelectorListener.cpp\
	SelectorListener.h\
	BsCategoryPanel.cpp\
	BsCategoryPanel.h\
	PosesBsCategoryPanel.cpp\
	PosesBsCategoryPanel.h\
	PosesBsSelectionListener.cpp\
	PosesBsSelectionListener.h\
	PagePosesBsPanel.cpp\
	PagePosesBsPanel.h\
	PosesBsPanel.cpp\
	PosesBsPanel.h\
	PageBsPanel.cpp\
	PageBsPanel.h\
	ImageButton.h\
	PosesBsPanelSelectionListener.cpp\
	PosesBsPanelSelectionListener.h\
	BsPanelSelectionListener.cpp\
	BsPanelSelectionListener.h
	
makehuman_LDFLAGS = 

makehuman_LDADD = @PACKAGE_LIBS@\
		  @GLUT_LIBS@

.rc.o:
	windres $^ -o $@
%.o : %.rc
	windres $^ -o $@
