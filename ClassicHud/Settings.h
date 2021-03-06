#pragma once
#include <plugin.h>
#include "IniReader.h"
#include "CRGBA.h"

#define CLASSICHUD_TXD_PATH "models\\ClassicHud\\classichud.txd"
#define DEFAULT_WEAPONS_TXD "models\\ClassicHud\\weapons.txd"
#define CLASSICHUD_DAT_PATH ".\\models\\ClassicHud\\classichud.dat"
#define CLASSICHUD_INI_PATH ".\\models\\ClassicHud\\classichud.ini"

enum eStyles {
	STYLE_SA,
	STYLE_III,
	STYLE_VC,
	STYLE_LCS,
	STYLE_VCS
};

class Settings {
public:
	// files
	char* HUD_TXD;
	char* FONTS_TXD;
	char* FONTS_DAT;
	char* WEAPONS_TXD;

	// Money
	int MONEY_R, MONEY_G, MONEY_B, MONEY_A, MONEY_SHADOW, MONEY_OUTLINE, MONEY_FONTSTYLE;
	float MONEY_X, MONEY_Y, MONEY_WIDTH, MONEY_HEIGHT;

	// Clock
	float CLOCK_X, CLOCK_Y, CLOCK_WIDTH, CLOCK_HEIGHT;
	int CLOCK_R, CLOCK_G, CLOCK_B, CLOCK_A, CLOCK_FONTSTYLE;

	// Health
	float HEALTHICON_X, HEALTHICON_Y, HEALTHICON_WIDTH, HEALTHICON_HEIGHT;
	float HEALTH_X, HEALTH_Y, HEALTH_WIDTH, HEALTH_HEIGHT;
	float HEALTHBAR_X, HEALTHBAR_Y, HEALTHBAR_SIZE_X, HEALTHBAR_SIZE_Y;
	int HEALTH_DISPLAY, HEALTH_R, HEALTH_G, HEALTH_B, HEALTH_A, HEALTHBAR_BORDER, HEALTH_TYPE;

	// Armour
	float ARMOUR_X, ARMOUR_Y, ARMOUR_SIZE_X, ARMOUR_SIZE_Y;
	float ARMOURBAR_X, ARMOURBAR_Y, ARMOURBAR_SIZE_X, ARMOURBAR_SIZE_Y;
	float ARMOURICON_X, ARMOURICON_Y, ARMOURICON_SIZE_X, ARMOURICON_SIZE_Y;
	int ARMOUR_R, ARMOUR_G, ARMOUR_B, ARMOUR_A, ARMOURBAR_BORDER, ARMOUR_TYPE;

	// Breath
	float BREATH_X, BREATH_Y, BREATH_SIZE_X, BREATH_SIZE_Y;
	float BREATHICON_X, BREATHICON_Y, BREATHICON_SIZE_X, BREATHICON_SIZE_Y;
	float BREATHBAR_X, BREATHBAR_Y, BREATHBAR_SIZE_X, BREATHBAR_SIZE_Y;
	int BREATH_R, BREATH_G, BREATH_B, BREATH_A, BREATHBAR_BORDER, BREATH_TYPE;

	// Ammo
	int AMMO_SHADOW, AMMO_OUTLINE, D_AMMO_R, D_AMMO_G, D_AMMO_B, AMMO_R, AMMO_G, AMMO_B;
	float AMMO_SIZE_X, AMMO_SIZE_Y, AMMO_X, AMMO_Y;

	// Weapon
	int WEAPON_A;
	float WEAPON_X, WEAPON_Y, WEAPON_SIZE_X, WEAPON_SIZE_Y;
	int STAT_WEP_ALPHA;
	float stat3_2;

	// Wanted Level
	float STAR_X, STAR_Y, STAR_SIZE_X, STAR_SIZE_Y, STAR_SPACE, STAR_X_OFFSET, STAR_Y_OFFSET;
	int STAR_R, STAR_G, STAR_B, STARBACK_R, STARBACK_G, STARBACK_B, SHADOWSTAR_A;
	int FLASHSTAR_R, FLASHSTAR_G, FLASHSTAR_B, STAR_A2, STAR_A3, SHADOWSTAR_A2, SHADOWSTAR_A3, STAR_STYLE;

	// Area
	int AREA_FONTSTYLE, AREA_R, AREA_G, AREA_B, AREA_SHADOW, AREA_OUTLINE;
	float AREA_SIZE_X, AREA_SIZE_Y, AREA_X, AREA_Y, AREA_SLANT;

	// Vehicle
	int VEHICLE_FONTSTYLE, VEHICLE_R, VEHICLE_G, VEHICLE_B, VEHICLE_SHADOW, VEHICLE_OUTLINE;
	float VEHICLE_SIZE_X, VEHICLE_SIZE_Y, VEHICLE_X, VEHICLE_Y, VEHICLE_SLANT;

	// Subtitles
	int SUBTITLES_SHADOW, SUBTITLES_OUTLINE, SUBTITLES_R, SUBTITLES_G, SUBTITLES_B;
	float SUBTITLES_X, SUBTITLES_Y, SUBTITLES_SIZE_X, SUBTITLES_SIZE_Y;
	float SUBTITLES_VITALSTATS_X, SUBTITLES_VITALSTATS_SIZE_X, SUBTITLES_LINEWIDTH;

	// Text box (Help Message)
	float fTextBoxPosnX;
	float fTextBoxPosnXWithRadar;
	float fTextBoxPosnXWithRadarAndPlane;
	float fTextBoxPosnY;
	float fTextBoxFontScaleX;
	float fTextBoxFontScaleY;
	float fTextBoxWidth;
	float fTextBoxBorderSize;

	// Text box for Stats
	int TEXTBOX_STATBAR_R, TEXTBOX_STATBAR_G, TEXTBOX_STATBAR_B;
	int TEXTBOX_STATBAR_ADD_R, TEXTBOX_STATBAR_ADD_G, TEXTBOX_STATBAR_ADD_B;
	int TEXTBOX_STATBAR_SUBS_R, TEXTBOX_STATBAR_SUBS_G, TEXTBOX_STATBAR_SUBS_B;
	float TEXTBOX_STAT_BORDER, TEXTBOX_STATBAR_Y, TEXTBOX_STATBAR_SIZE_X, TEXTBOX_STATBAR_SIZE_Y;

	// Radar
	float RADAR_RANGE, RADAR_X, RADAR_Y, RADAR_SIZE_X, RADAR_SIZE_Y, VC_RADAR_OFFSET;
	int RADAR_A;

	// Success Failed Message (Styled Text 1)
	int STYLED1_SHADOW, STYLED1_OUTLINE, STYLED1_R, STYLED1_G, STYLED1_B;
	float STYLED1_Y, STYLED1_Y1, STYLED1_Y2, STYLED1_SIZE_X, STYLED1_SIZE_Y;

	// Mission Title (Styled Text 2)
	int MT_R, MT_G, MT_B, MT_SHADOW, MT_OUTLINE, MT_FONTSTYLE;
	float MT_POS_X, MT_POS_Y, MT_SIZE_X, MT_SIZE_Y;

	// Wasted Busted Message (Styled Text 3)
	int WB_R, WB_G, WB_B, WB_SHADOW, WB_OUTLINE, WB_FONTSTYLE, WB_ALIGN;
	float WB_POS_X, WB_POS_Y, WB_SIZE_X, WB_SIZE_Y;

	// Odd Job Messages 1 (Styled Text 4)
	int STYLED4_FONTSTYLE, STYLED4_SHADOW, STYLED4_OUTLINE, STYLED4_R, STYLED4_G, STYLED4_B, STYLED4_ALIGN;
	float STYLED4_X, STYLED4_Y, STYLED4_SIZE_X, STYLED4_SIZE_Y;

	// Odd Job Messages 2 (Styled Text 5)
	int STYLED5_FONTSTYLE, STYLED5_SHADOW, STYLED5_OUTLINE, STYLED5_R, STYLED5_G, STYLED5_B;
	float STYLED5_Y, STYLED5_SIZE_X, STYLED5_SIZE_Y;

	// Odd Job Messages 3 (Styled Text 6)
	int STYLED6_FONTSTYLE, STYLED6_SHADOW, STYLED6_OUTLINE, STYLED6_R, STYLED6_G, STYLED6_B;
	float STYLED6_Y, STYLED6_SIZE_X, STYLED6_SIZE_Y;

	// Odd Job Messages 4 (Styled Text 7)
	int STYLED7_FONTSTYLE, STYLED7_SHADOW, STYLED7_OUTLINE, STYLED7_R, STYLED7_G, STYLED7_B;
	float STYLED7_Y, STYLED7_SIZE_X, STYLED7_SIZE_Y;

	// Mission Timers (Timer and Status Text)
	int M_TIMERS_FONTSTYLE, M_TIMERS_SHADOW, M_TIMERS_OUTLINE, M_TIMERS_R, M_TIMERS_G, M_TIMERS_B;
	float M_TIMERS_SIZE_X, M_TIMERS_SIZE_Y, M_TIMER_X, M_TIMER_Y, M_TIMER_DESC_X;
	int M_STATUSBAR_BORDER, M_STATUSBAR_R, M_STATUSBAR_G, M_STATUSBAR_B, M_STATUSBAR_A;
	float M_STATUSBAR_X, M_STATUSBAR_Y, M_STATUSBAR_SIZE_X, M_STATUSBAR_SIZE_Y;
	float M_STATUSTEXT_X, M_STATUSTEXT_Y, M_STATUSTEXT_DESC_X, M_STATUSTEXT_SPACE;

	// Radio Station Text
	int RS_FONTSTYLE, RS_SHADOW, RS_OUTLINE, RS_RED1, RS_GREEN1, RS_BLUE1, RS_RED2, RS_GREEN2, RS_BLUE2;
	float RS_POS_Y, RS_SIZE_X, RS_SIZE_Y;

	// Garage Messages
	int GARAGES_R, GARAGES_G, GARAGES_B, GARAGES_FONTSTYLE, GARAGES_SHADOW, GARAGES_OUTLINE;
	float GARAGES_Y, GARAGES_SIZE_X, GARAGES_SIZE_Y, GARAGES_LINEWIDTH;

	// Custom text (for Hud Editor)
	int CUSTOM_ALIGN, CUSTOM_SHADOW, CUSTOM_OUTLINE, CUSTOM_FONTSTYLE, CUSTOM_R, CUSTOM_G, CUSTOM_B, CUSTOM_A;
	float CUSTOM_X, CUSTOM_Y, CUSTOM_SIZE_X, CUSTOM_SIZE_Y, CUSTOM_LINEWIDTH;

	// Legal Text
	int LEGALTEXT_ENABLE;

	// Splash
	int SPLASH_STYLE, SPLASH_SCREENS, SPLASH_LOGO_SIZE;
	char* LOADSCS_TXD;

	// Loading Screen Bar
	int LOADBAR_R1, LOADBAR_G1, LOADBAR_B1, LOADBAR_R2, LOADBAR_G2, LOADBAR_B2;
	int LOADBAR_STYLE, LOADBAR_BORDER;
	float LOADBAR_X, LOADBAR_Y, LOADBAR_SIZE_X, LOADBAR_SIZE_Y, LOADBAR_SHADOW;

	// Menu Title
	int MENUTITLE_R, MENUTITLE_G, MENUTITLE_B, MENUTITLE_D_R, MENUTITLE_D_G, MENUTITLE_D_B;
	int MENUTITLE_FONTSTYLE, MENUTITLE_ALIGN, MENUTITLE_OUTLINE, MENUTITLE_SHADOW;
	float MENUTITLE_X, MENUTITLE_Y, MENUTITLE_SIZE_X, MENUTITLE_SIZE_Y, MENUTITLE_STATS_X, MENUTITLE_STATS_Y;
	
	// Menu Ask
	int MENUASK_R, MENUASK_G, MENUASK_B;
	int MENUASK_FONTSTYLE, MENUASK_ALIGN, MENUASK_OUTLINE, MENUASK_SHADOW;
	float MENUASK_X, MENUASK_Y, MENUASK_SIZE_X, MENUASK_SIZE_Y, MENUASK_LINEWIDTH;
	int MENUASK_YN_R, MENUASK_YN_G, MENUASK_YN_B, MENUASK_YN_SEL_R, MENUASK_YN_SEL_G, MENUASK_YN_SEL_B, MENUASK_YN_FONTSTYLE;

	// Menu Options
	int MENU_R, MENU_G, MENU_B, MENU_SEL_R, MENU_SEL_G, MENU_SEL_B;
	int MENU_FONTSTYLE, MENU_ALIGN, MENU_OUTLINE, MENU_SHADOW;
	float MENU_X, MENU_Y, MENU_SIZE_X, MENU_SIZE_Y, MENU_R_X, MENU_R_SIZE_X, MENU_R_SIZE_Y;
	float MENU_Y0, MENU_Y1, MENU_Y1_OFFSET, MENU_YSPACE, MENU_YK, MENU_YN, MENU_YY;
	int MENU_RECT_R, MENU_RECT_G, MENU_RECT_B, MENU_RECT_A;
	float MENU_RECT_X, MENU_RECT_Y, MENU_RECT_WIDTH, MENU_RECT_HEIGHT;

	// Menu Stats
	int MENU_SETTINGS_SHOW_RIGHT, MENU_STATS_FONTSTYLE;
	float MENU_STATS_X, MENU_STATS_Y_OFFSET, MENU_STATS_YSPACE, MENU_STATS_SIZE_X, MENU_STATS_SIZE_Y;
	float MENU_SLIDERS_X, MENU_SLIDERS_Y, MENU_SLIDERS_SIZE_X, MENU_SLIDERS_SIZE_Y;
	float MENU_STATS_RECT_WIDTH, MENU_STATS_RECT_X;

	// Stats: Player Rank
	int RANK_SHADOW, RANK_OUTLINE, RANK_R, RANK_G, RANK_B, RANK_ALIGN, RANK_FONTSTYLE;
	float RANK_X, RANK_Y, RANK_SIZE_X, RANK_SIZE_Y;

	// Stats
	int STATS_SHADOW, STATS_OUTLINE, STATS_R, STATS_G, STATS_B, STATS_FONTSTYLE;
	float STATS_X, STATS_Y_OFFSET, STATS_R_X, STATS_R_Y, STATS_SIZE_X, STATS_SIZE_Y, STATS_YSPACE;

	// Menu Settings
	int MENU_SETTINGS_FONTSTYLE;
	float MENU_SETTINGS_X, MENU_SETTINGS_Y_OFFSET, MENU_SETTINGS_YSPACE, MENU_SETTINGS_SIZE_X, MENU_SETTINGS_SIZE_Y;

	// Menu Save / Load
	int MENU_SAVELOAD_FONTSTYLE, MENU_SAVELOAD_R, MENU_SAVELOAD_G, MENU_SAVELOAD_B;
	int MENU_SAVELOAD_SEL_R, MENU_SAVELOAD_SEL_G, MENU_SAVELOAD_SEL_B;
	int MENU_SAVELOAD_SHADOW, MENU_SAVELOAD_OUTLINE;
	float MENU_SAVELOAD_X, MENU_SAVELOAD_Y_OFFSET, MENU_SAVELOAD_YSPACE, MENU_SAVELOAD_SIZE_X, MENU_SAVELOAD_SIZE_Y;
	float MENU_TEST_RECT_X, MENU_TEST_RECT_Y, MENU_TEST_RECT_W, MENU_TEST_RECT_H;

	// New Game Screen
	int MENU_NEWGAME_FONTSTYLE;

	// Menu Background
	char* BGMENU_TXD;
	int BG_STYLE, BG_SCREENS, BG_VC_SCREEN, BG_SEL_RECTS, BG_V_ALIGN_MIDDLE, BG_MAP_MASK_ID;
	int DISPLAY_LOGO_ON_LOADING_SCREEN, DISPLAY_LOGO_ON_MAIN_MENU, DISPLAY_LOGO_ON_FRONT_END, LOGO_ID;
	float LOGO_X, LOGO_Y, LOGO_SIZE_X, LOGO_SIZE_Y, LOGO_SHADOW_OFFSET;

	// Deletethis
	float D_W, D_H;
	float D_LOGO_X, D_LOGO_Y, D_LOGO_W, D_LOGO_H, D_LOGO_SHADOW_OFFSET;

	// Brief Messages
	int BRIEF_SHADOW, BRIEF_OUTLINE, BRIEF_R, BRIEF_G, BRIEF_B;
	float BRIEF_X, BRIEF_Y, BRIEF_YSPACE, BRIEF_SIZE_X, BRIEF_SIZE_Y;

	// Markers
	char* MARKERS_ARROW_MODEL;
	char* MARKERS_CYLINDER_MODEL;
	int MARKERS_R, MARKERS_G, MARKERS_B, ENEX_R, ENEX_G, ENEX_B;
	int MARKERS_USE_ARROWS, MARKERS_ROTATE, MARKERS_SIZE_CHANGE, MARKERS_CONE_ALPHA, MARKERS_CONE_GLOW;

public:
	void Init(int style);
};

extern Settings settings;