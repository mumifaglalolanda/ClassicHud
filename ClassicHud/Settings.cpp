#include "Settings.h"

Settings settings;

void Settings::Init(int style)
{
	char section[20];
	char* prefix[5] = { "SA", "III", "VC", "LCS", "VCS" };
	CIniReader iniReader(CLASSICHUD_DAT_PATH);

	sprintf(section, "CUSTOM_TEXT");
	CUSTOM_ALIGN = iniReader.ReadInt(section, "ALIGN", 0);
	CUSTOM_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	CUSTOM_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	CUSTOM_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);
	CUSTOM_R = iniReader.ReadInt(section, "RED", 0);
	CUSTOM_G = iniReader.ReadInt(section, "GREEN", 0);
	CUSTOM_B = iniReader.ReadInt(section, "BLUE", 0);
	CUSTOM_A = iniReader.ReadInt(section, "ALPHA", 0);
	CUSTOM_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	CUSTOM_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	CUSTOM_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	CUSTOM_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
	CUSTOM_LINEWIDTH = iniReader.ReadFloat(section, "LINEWIDTH", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "FILES");
	HUD_TXD = iniReader.ReadString(section, "HUD_TXD", "404");
	FONTS_TXD = iniReader.ReadString(section, "FONTS_TXD", "404");
	FONTS_DAT = iniReader.ReadString(section, "FONTS_DAT", "404");
	WEAPONS_TXD = iniReader.ReadString(section, "WEAPONS_TXD", "404");

	sprintf(section, "%s_%s", prefix[style], "MONEY");
	MONEY_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	MONEY_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	MONEY_WIDTH = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	MONEY_HEIGHT = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
	MONEY_R = iniReader.ReadInt(section, "RED", 47);
	MONEY_G = iniReader.ReadInt(section, "GREEN", 90);
	MONEY_B = iniReader.ReadInt(section, "BLUE", 38);
	MONEY_A = iniReader.ReadInt(section, "ALPHA", 255);
	MONEY_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 2);
	MONEY_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	MONEY_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);

	sprintf(section, "%s_%s", prefix[style], "CLOCK");
	CLOCK_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	CLOCK_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	CLOCK_WIDTH = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	CLOCK_HEIGHT = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
	CLOCK_R = iniReader.ReadInt(section, "RED", 194);
	CLOCK_G = iniReader.ReadInt(section, "GREEN", 165);
	CLOCK_B = iniReader.ReadInt(section, "BLUE", 120);
	CLOCK_A = iniReader.ReadInt(section, "ALPHA", 255);
	CLOCK_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);

	sprintf(section, "%s_%s", prefix[style], "HEALTH");
	HEALTH_DISPLAY = iniReader.ReadInt(section, "DISPLAY", 0);
	HEALTH_R = iniReader.ReadInt(section, "RED", 180);
	HEALTH_G = iniReader.ReadInt(section, "GREEN", 100);
	HEALTH_B = iniReader.ReadInt(section, "BLUE", 50);
	HEALTH_A = iniReader.ReadInt(section, "ALPHA", 255);
	HEALTHICON_X = iniReader.ReadFloat(section, "ICON_X", 0.0f);
	HEALTHICON_Y = iniReader.ReadFloat(section, "ICON_Y", 0.0f);
	HEALTHICON_WIDTH = iniReader.ReadFloat(section, "ICON_SIZE_X", 0.0f);
	HEALTHICON_HEIGHT = iniReader.ReadFloat(section, "ICON_SIZE_Y", 0.0f);
	HEALTH_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	HEALTH_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	HEALTH_WIDTH = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	HEALTH_HEIGHT = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
	HEALTHBAR_X = iniReader.ReadFloat(section, "BAR_X", 0.0f);
	HEALTHBAR_Y = iniReader.ReadFloat(section, "BAR_Y", 0.0f);
	HEALTHBAR_SIZE_X = iniReader.ReadFloat(section, "BAR_SIZE_X", 0.0f);
	HEALTHBAR_SIZE_Y = iniReader.ReadFloat(section, "BAR_SIZE_Y", 0.0f);
	HEALTHBAR_BORDER = iniReader.ReadInt(section, "BORDER", 0);
	HEALTH_TYPE = iniReader.ReadInt(section, "TYPE", 0);

	sprintf(section, "%s_%s", prefix[style], "ARMOUR");
	ARMOUR_R = iniReader.ReadInt(section, "RED", 121);
	ARMOUR_G = iniReader.ReadInt(section, "GREEN", 136);
	ARMOUR_B = iniReader.ReadInt(section, "BLUE", 93);
	ARMOUR_A = iniReader.ReadInt(section, "ALPHA", 255);
	ARMOUR_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	ARMOUR_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	ARMOUR_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	ARMOUR_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
	ARMOURBAR_X = iniReader.ReadFloat(section, "BAR_X", 0.0f);
	ARMOURBAR_Y = iniReader.ReadFloat(section, "BAR_Y", 0.0f);
	ARMOURBAR_SIZE_X = iniReader.ReadFloat(section, "BAR_SIZE_X", 0.0f);
	ARMOURBAR_SIZE_Y = iniReader.ReadFloat(section, "BAR_SIZE_Y", 0.0f);
	ARMOURICON_X = iniReader.ReadFloat(section, "ICON_X", 0.0f);
	ARMOURICON_Y = iniReader.ReadFloat(section, "ICON_Y", 0.0f);
	ARMOURICON_SIZE_X = iniReader.ReadFloat(section, "ICON_SIZE_X", 0.0f);
	ARMOURICON_SIZE_Y = iniReader.ReadFloat(section, "ICON_SIZE_Y", 0.0f);
	ARMOURBAR_BORDER = iniReader.ReadInt(section, "BORDER", 0);
	ARMOUR_TYPE = iniReader.ReadInt(section, "TYPE", 0);

	sprintf(section, "%s_%s", prefix[style], "BREATH");
	BREATH_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	BREATH_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	BREATH_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	BREATH_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
	BREATHICON_X = iniReader.ReadFloat(section, "ICON_X", 0.0f);
	BREATHICON_Y = iniReader.ReadFloat(section, "ICON_Y", 0.0f);
	BREATHICON_SIZE_X = iniReader.ReadFloat(section, "ICON_SIZE_X", 0.0f);
	BREATHICON_SIZE_Y = iniReader.ReadFloat(section, "ICON_SIZE_Y", 0.0f);
	BREATHBAR_X = iniReader.ReadFloat(section, "BAR_X", 0.0f);
	BREATHBAR_Y = iniReader.ReadFloat(section, "BAR_Y", 0.0f);
	BREATHBAR_SIZE_X = iniReader.ReadFloat(section, "BAR_SIZE_X", 0.0f);
	BREATHBAR_SIZE_Y = iniReader.ReadFloat(section, "BAR_SIZE_Y", 0.0f);
	BREATH_R = iniReader.ReadInt(section, "RED", 121);
	BREATH_G = iniReader.ReadInt(section, "GREEN", 136);
	BREATH_B = iniReader.ReadInt(section, "BLUE", 93);
	BREATH_A = iniReader.ReadInt(section, "ALPHA", 255);
	BREATHBAR_BORDER = iniReader.ReadInt(section, "BORDER", 0);
	BREATH_TYPE = iniReader.ReadInt(section, "TYPE", 0);

	sprintf(section, "%s_%s", prefix[style], "AMMO");
	AMMO_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	AMMO_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	D_AMMO_R = iniReader.ReadInt(section, "D_RED", 0);
	D_AMMO_G = iniReader.ReadInt(section, "D_GREEN", 0);
	D_AMMO_B = iniReader.ReadInt(section, "D_BLUE", 0);
	AMMO_R = iniReader.ReadInt(section, "RED", 0);
	AMMO_G = iniReader.ReadInt(section, "GREEN", 0);
	AMMO_B = iniReader.ReadInt(section, "BLUE", 0);
	AMMO_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	AMMO_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
	AMMO_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	AMMO_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "WEAPON");
	WEAPON_A = iniReader.ReadInt(section, "ALPHA", 0);
	WEAPON_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	WEAPON_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	WEAPON_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	WEAPON_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "WANTED");
	STAR_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	STAR_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	STAR_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	STAR_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
	STAR_SPACE = iniReader.ReadFloat(section, "SPACE", 0.0f);
	STAR_R = iniReader.ReadInt(section, "ACTIVE_R", 0);
	STAR_G = iniReader.ReadInt(section, "ACTIVE_G", 0);
	STAR_B = iniReader.ReadInt(section, "ACTIVE_B", 0);
	STARBACK_R = iniReader.ReadInt(section, "INACTIVE_R", 0);
	STARBACK_G = iniReader.ReadInt(section, "INACTIVE_G", 0);
	STARBACK_B = iniReader.ReadInt(section, "INACTIVE_B", 0);
	FLASHSTAR_R = iniReader.ReadInt(section, "FLASH_R", 0);
	FLASHSTAR_G = iniReader.ReadInt(section, "FLASH_G", 0);
	FLASHSTAR_B = iniReader.ReadInt(section, "FLASH_B", 0);
	SHADOWSTAR_A = iniReader.ReadInt(section, "SHADOW_A1", 0);
	SHADOWSTAR_A2 = iniReader.ReadInt(section, "SHADOW_A2", 0);
	SHADOWSTAR_A3 = iniReader.ReadInt(section, "SHADOW_A3", 0);
	STAR_A2 = iniReader.ReadInt(section, "STAR_A2", 0);
	STAR_A3 = iniReader.ReadInt(section, "STAR_A3", 0);
	STAR_STYLE = iniReader.ReadInt(section, "STYLE", 0);
	STAR_X_OFFSET = iniReader.ReadFloat(section, "OFFSET_X", 0.0f);
	STAR_Y_OFFSET = iniReader.ReadFloat(section, "OFFSET_Y", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "AREA");
	AREA_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);
	AREA_R = iniReader.ReadInt(section, "RED", 0);
	AREA_G = iniReader.ReadInt(section, "GREEN", 0);
	AREA_B = iniReader.ReadInt(section, "BLUE", 0);
	AREA_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	AREA_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	AREA_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	AREA_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
	AREA_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	AREA_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	AREA_SLANT = iniReader.ReadFloat(section, "SLANT", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "VEHICLE");
	VEHICLE_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);
	VEHICLE_R = iniReader.ReadInt(section, "RED", 0);
	VEHICLE_G = iniReader.ReadInt(section, "GREEN", 0);
	VEHICLE_B = iniReader.ReadInt(section, "BLUE", 0);
	VEHICLE_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	VEHICLE_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	VEHICLE_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	VEHICLE_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
	VEHICLE_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	VEHICLE_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	VEHICLE_SLANT = iniReader.ReadFloat(section, "SLANT", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "SUBTITLES");
	SUBTITLES_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	SUBTITLES_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	SUBTITLES_R = iniReader.ReadInt(section, "RED", 0);
	SUBTITLES_G = iniReader.ReadInt(section, "GREEN", 0);
	SUBTITLES_B = iniReader.ReadInt(section, "BLUE", 0);
	SUBTITLES_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	SUBTITLES_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	SUBTITLES_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	SUBTITLES_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
	SUBTITLES_LINEWIDTH = iniReader.ReadFloat(section, "LINEWIDTH", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "SUBTITLES_VITALSTATS");
	SUBTITLES_VITALSTATS_X = iniReader.ReadFloat(section, "POS_X1", 0.0f);
	SUBTITLES_VITALSTATS_SIZE_X = iniReader.ReadFloat(section, "SIZE_X1", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "HELP_TEXT");
	fTextBoxPosnX = iniReader.ReadFloat(section, "POS_X", 0.0f);
	fTextBoxPosnXWithRadar = iniReader.ReadFloat(section, "POS_X", 0.0f);
	fTextBoxPosnXWithRadarAndPlane = iniReader.ReadFloat(section, "POS_X", 0.0f);
	fTextBoxPosnY = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	fTextBoxFontScaleX = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	fTextBoxFontScaleY = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
	fTextBoxWidth = iniReader.ReadFloat(section, "WIDTH", 0.0f);
	fTextBoxBorderSize = iniReader.ReadFloat(section, "BORDER_SIZE", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "HELP_TEXT_STAT");
	TEXTBOX_STATBAR_R = iniReader.ReadInt(section, "BAR_R", 0);
	TEXTBOX_STATBAR_G = iniReader.ReadInt(section, "BAR_G", 0);
	TEXTBOX_STATBAR_B = iniReader.ReadInt(section, "BAR_B", 0);
	TEXTBOX_STATBAR_ADD_R = iniReader.ReadInt(section, "BAR_ADD_R", 0);
	TEXTBOX_STATBAR_ADD_G = iniReader.ReadInt(section, "BAR_ADD_G", 0);
	TEXTBOX_STATBAR_ADD_B = iniReader.ReadInt(section, "BAR_ADD_B", 0);
	TEXTBOX_STATBAR_SUBS_R = iniReader.ReadInt(section, "BAR_SUBS_R", 0);
	TEXTBOX_STATBAR_SUBS_G = iniReader.ReadInt(section, "BAR_SUBS_G", 0);
	TEXTBOX_STATBAR_SUBS_B = iniReader.ReadInt(section, "BAR_SUBS_B", 0);
	TEXTBOX_STAT_BORDER = iniReader.ReadFloat(section, "BORDER_SIZE", 0.0f);
	TEXTBOX_STATBAR_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	TEXTBOX_STATBAR_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	TEXTBOX_STATBAR_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "RADAR");
	RADAR_A = iniReader.ReadInt(section, "ALPHA", 0);
	RADAR_RANGE = iniReader.ReadFloat(section, "RANGE", 0.0f);
	RADAR_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	RADAR_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	RADAR_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	RADAR_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
	VC_RADAR_OFFSET = iniReader.ReadFloat(section, "VC_OFFSET", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "STYLED1");
	STYLED1_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	STYLED1_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	STYLED1_R = iniReader.ReadInt(section, "RED", 0);
	STYLED1_G = iniReader.ReadInt(section, "GREEN", 0);
	STYLED1_B = iniReader.ReadInt(section, "BLUE", 0);
	STYLED1_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	STYLED1_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	STYLED1_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "STYLED1_2LINES_NOT_ST3");
	STYLED1_Y2 = iniReader.ReadFloat(section, "POS_Y2", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "STYLED1_AND_ST4_OR_ST6");
	STYLED1_Y1 = iniReader.ReadFloat(section, "POS_Y1", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "STYLED2");
	MT_R = iniReader.ReadInt(section, "RED", 0);
	MT_G = iniReader.ReadInt(section, "GREEN", 0);
	MT_B = iniReader.ReadInt(section, "BLUE", 0);
	MT_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	MT_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	MT_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);
	MT_POS_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	MT_POS_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	MT_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	MT_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "STYLED3");
	WB_R = iniReader.ReadInt(section, "RED", 0);
	WB_G = iniReader.ReadInt(section, "GREEN", 0);
	WB_B = iniReader.ReadInt(section, "BLUE", 0);
	WB_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	WB_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	WB_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);
	WB_ALIGN = iniReader.ReadInt(section, "ALIGN", 0);
	WB_POS_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	WB_POS_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	WB_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	WB_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "STYLED4");
	STYLED4_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);
	STYLED4_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	STYLED4_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	STYLED4_ALIGN = iniReader.ReadInt(section, "ALIGN", 0);
	STYLED4_R = iniReader.ReadInt(section, "RED", 0);
	STYLED4_G = iniReader.ReadInt(section, "GREEN", 0);
	STYLED4_B = iniReader.ReadInt(section, "BLUE", 0);
	STYLED4_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	STYLED4_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	STYLED4_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	STYLED4_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "STYLED5");
	STYLED5_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);
	STYLED5_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	STYLED5_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	STYLED5_R = iniReader.ReadInt(section, "RED", 0);
	STYLED5_G = iniReader.ReadInt(section, "GREEN", 0);
	STYLED5_B = iniReader.ReadInt(section, "BLUE", 0);
	STYLED5_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	STYLED5_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	STYLED5_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "STYLED6");
	STYLED6_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);
	STYLED6_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	STYLED6_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	STYLED6_R = iniReader.ReadInt(section, "RED", 0);
	STYLED6_G = iniReader.ReadInt(section, "GREEN", 0);
	STYLED6_B = iniReader.ReadInt(section, "BLUE", 0);
	STYLED6_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	STYLED6_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	STYLED6_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "STYLED7");
	STYLED7_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);
	STYLED7_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	STYLED7_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	STYLED7_R = iniReader.ReadInt(section, "RED", 0);
	STYLED7_G = iniReader.ReadInt(section, "GREEN", 0);
	STYLED7_B = iniReader.ReadInt(section, "BLUE", 0);
	STYLED7_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	STYLED7_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	STYLED7_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "MISSION_TIMERS");
	M_TIMERS_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);
	M_TIMERS_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	M_TIMERS_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	M_TIMERS_R = iniReader.ReadInt(section, "RED", 0);
	M_TIMERS_G = iniReader.ReadInt(section, "GREEN", 0);
	M_TIMERS_B = iniReader.ReadInt(section, "BLUE", 0);
	M_TIMERS_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	M_TIMERS_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
	M_TIMER_X = iniReader.ReadFloat(section, "POS_X2", 0.0f);
	M_TIMER_DESC_X = iniReader.ReadFloat(section, "POS_X1", 0.0f);
	M_TIMER_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "STATUS_TEXT");
	M_STATUSTEXT_X = iniReader.ReadFloat(section, "POS_X2", 0.0f);
	M_STATUSTEXT_DESC_X = iniReader.ReadFloat(section, "POS_X1", 0.0f);
	M_STATUSTEXT_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	M_STATUSTEXT_SPACE = iniReader.ReadFloat(section, "SPACE", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "STATUS_TEXT_BAR");
	M_STATUSBAR_BORDER = iniReader.ReadInt(section, "BORDER", 0);
	M_STATUSBAR_R = iniReader.ReadInt(section, "RED", 0);
	M_STATUSBAR_G = iniReader.ReadInt(section, "GREEN", 0);
	M_STATUSBAR_B = iniReader.ReadInt(section, "BLUE", 0);
	M_STATUSBAR_A = iniReader.ReadInt(section, "ALPHA", 0);
	M_STATUSBAR_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	M_STATUSBAR_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	M_STATUSBAR_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	M_STATUSBAR_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "RADIO_STATION");
	RS_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);
	RS_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	RS_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	RS_RED1 = iniReader.ReadInt(section, "RED1", 0);
	RS_GREEN1 = iniReader.ReadInt(section, "GREEN1", 0);
	RS_BLUE1 = iniReader.ReadInt(section, "BLUE1", 0);
	RS_RED2 = iniReader.ReadInt(section, "RED2", 0);
	RS_GREEN2 = iniReader.ReadInt(section, "GREEN2", 0);
	RS_BLUE2 = iniReader.ReadInt(section, "BLUE2", 0);
	RS_POS_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	RS_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	RS_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "GARAGE_MESSAGES");
	GARAGES_R = iniReader.ReadInt(section, "RED", 0);
	GARAGES_G = iniReader.ReadInt(section, "GREEN", 0);
	GARAGES_B = iniReader.ReadInt(section, "BLUE", 0);
	GARAGES_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);
	GARAGES_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	GARAGES_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	GARAGES_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	GARAGES_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	GARAGES_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
	GARAGES_LINEWIDTH = iniReader.ReadFloat(section, "LINEWIDTH", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "LEGAL_TEXT1");
	LEGALTEXT_ENABLE = iniReader.ReadInt(section, "ENABLE", 0);

	sprintf(section, "%s_%s", prefix[style], "SPLASH");
	SPLASH_STYLE = iniReader.ReadInt(section, "STYLE", 0);
	SPLASH_SCREENS = iniReader.ReadInt(section, "SCREENS", 0);
	LOADSCS_TXD = iniReader.ReadString(section, "PATH", "404");
	SPLASH_LOGO_SIZE = iniReader.ReadInt(section, "LOGO_SIZE", 0);

	sprintf(section, "%s_%s", prefix[style], "LOADING_SCREEN_BAR");
	LOADBAR_R1 = iniReader.ReadInt(section, "RED1", 0);
	LOADBAR_G1 = iniReader.ReadInt(section, "GREEN1", 0);
	LOADBAR_B1 = iniReader.ReadInt(section, "BLUE1", 0);
	LOADBAR_R2 = iniReader.ReadInt(section, "RED2", 0);
	LOADBAR_G2 = iniReader.ReadInt(section, "GREEN2", 0);
	LOADBAR_B2 = iniReader.ReadInt(section, "BLUE2", 0);
	LOADBAR_STYLE = iniReader.ReadInt(section, "STYLE", 0);
	LOADBAR_BORDER = iniReader.ReadInt(section, "BORDER", 0);
	LOADBAR_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	LOADBAR_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	LOADBAR_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	LOADBAR_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
	LOADBAR_SHADOW = iniReader.ReadFloat(section, "SHADOW", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "MENU_TITLE");
	MENUTITLE_R = iniReader.ReadInt(section, "RED", 0);
	MENUTITLE_G = iniReader.ReadInt(section, "GREEN", 0);
	MENUTITLE_B = iniReader.ReadInt(section, "BLUE", 0);
	MENUTITLE_D_R = iniReader.ReadInt(section, "DROP_RED", 0);
	MENUTITLE_D_G = iniReader.ReadInt(section, "DROP_GREEN", 0);
	MENUTITLE_D_B = iniReader.ReadInt(section, "DROP_BLUE", 0);
	MENUTITLE_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);
	MENUTITLE_ALIGN = iniReader.ReadInt(section, "ALIGN", 0);
	MENUTITLE_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	MENUTITLE_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	MENUTITLE_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	MENUTITLE_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	MENUTITLE_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	MENUTITLE_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "MENU_TITLE_STATS");
	MENUTITLE_STATS_X = iniReader.ReadFloat(section, "STATS_X", 0.0f);
	MENUTITLE_STATS_Y = iniReader.ReadFloat(section, "STATS_Y", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "MENU_ASK");
	MENUASK_R = iniReader.ReadInt(section, "RED", 0);
	MENUASK_G = iniReader.ReadInt(section, "GREEN", 0);
	MENUASK_B = iniReader.ReadInt(section, "BLUE", 0);
	MENUASK_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);
	MENUASK_ALIGN = iniReader.ReadInt(section, "ALIGN", 0);
	MENUASK_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	MENUASK_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	MENUASK_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	MENUASK_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	MENUASK_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	MENUASK_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
	MENUASK_LINEWIDTH = iniReader.ReadFloat(section, "LINEWIDTH", 0.0f);
	MENU_YN = iniReader.ReadFloat(section, "POS_YN", 0.0f);
	MENU_YY = iniReader.ReadFloat(section, "POS_YY", 0.0f);
	MENUASK_YN_R = iniReader.ReadInt(section, "YN_RED", 0);
	MENUASK_YN_G = iniReader.ReadInt(section, "YN_GREEN", 0);
	MENUASK_YN_B = iniReader.ReadInt(section, "YN_BLUE", 0);
	MENUASK_YN_SEL_R = iniReader.ReadInt(section, "SEL_YN_RED", 0);
	MENUASK_YN_SEL_G = iniReader.ReadInt(section, "SEL_YN_GREEN", 0);
	MENUASK_YN_SEL_B = iniReader.ReadInt(section, "SEL_YN_BLUE", 0);
	MENUASK_YN_FONTSTYLE = iniReader.ReadInt(section, "YN_FONTSTYLE", 0);


	sprintf(section, "%s_%s", prefix[style], "MENU_OPTIONS");
	MENU_R = iniReader.ReadInt(section, "RED", 0);
	MENU_G = iniReader.ReadInt(section, "GREEN", 0);
	MENU_B = iniReader.ReadInt(section, "BLUE", 0);
	MENU_SEL_R = iniReader.ReadInt(section, "SELECTED_RED", 0);
	MENU_SEL_G = iniReader.ReadInt(section, "SELECTED_GREEN", 0);
	MENU_SEL_B = iniReader.ReadInt(section, "SELECTED_BLUE", 0);
	MENU_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);
	MENU_ALIGN = iniReader.ReadInt(section, "ALIGN", 0);
	MENU_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	MENU_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	MENU_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	MENU_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	MENU_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
	MENU_Y0 = iniReader.ReadFloat(section, "POS_Y0", 0.0f);
	MENU_Y1 = iniReader.ReadFloat(section, "POS_Y1", 0.0f);
	MENU_Y1_OFFSET = iniReader.ReadFloat(section, "OFFSET_Y1", 0.0f);
	MENU_YSPACE = iniReader.ReadFloat(section, "Y_SPACE", 0.0f);
	MENU_YK = iniReader.ReadFloat(section, "POS_YK", 0.0f);
	MENU_RECT_R = iniReader.ReadInt(section, "RECT_R", 0);
	MENU_RECT_G = iniReader.ReadInt(section, "RECT_G", 0);
	MENU_RECT_B = iniReader.ReadInt(section, "RECT_B", 0);
	MENU_RECT_A = iniReader.ReadInt(section, "RECT_A", 0);
	MENU_RECT_X = iniReader.ReadFloat(section, "RECT_X", 0.0f);
	MENU_RECT_Y = iniReader.ReadFloat(section, "RECT_Y", 0.0f);
	MENU_RECT_WIDTH = iniReader.ReadFloat(section, "RECT_WIDTH", 0.0f);
	MENU_RECT_HEIGHT = iniReader.ReadFloat(section, "RECT_HEIGHT", 0.0f);	

	sprintf(section, "%s_%s", prefix[style], "MENU_BORDERS");
	D_W = iniReader.ReadFloat(section, "W", 0.0f);
	D_H = iniReader.ReadFloat(section, "H", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "MENU_OPTIONS_STATS");
	MENU_STATS_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);
	MENU_STATS_X = iniReader.ReadFloat(section, "STATS_X", 0.0f);
	MENU_STATS_Y_OFFSET = iniReader.ReadFloat(section, "POS_Y_OFFSET", 0.0f);
	MENU_STATS_YSPACE = iniReader.ReadFloat(section, "SPACE", 0.0f);
	MENU_STATS_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	MENU_STATS_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
	MENU_STATS_RECT_X = iniReader.ReadFloat(section, "RECT_X", 0.0f);
	MENU_STATS_RECT_WIDTH = iniReader.ReadFloat(section, "RECT_WIDTH", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "STATS_PLAYER_RANK");
	RANK_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	RANK_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	RANK_R = iniReader.ReadInt(section, "RED", 0);
	RANK_G = iniReader.ReadInt(section, "GREEN", 0);
	RANK_B = iniReader.ReadInt(section, "BLUE", 0);
	RANK_ALIGN = iniReader.ReadInt(section, "ALIGN", 0);
	RANK_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);
	RANK_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	RANK_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	RANK_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	RANK_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "MENU_STATS");
	STATS_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	STATS_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	STATS_R = iniReader.ReadInt(section, "RED", 0);
	STATS_G = iniReader.ReadInt(section, "GREEN", 0);
	STATS_B = iniReader.ReadInt(section, "BLUE", 0);
	STATS_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);
	STATS_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	STATS_Y_OFFSET = iniReader.ReadFloat(section, "POS_Y_OFFSET", 0.0f);
	STATS_R_X = iniReader.ReadFloat(section, "RIGHT_X", 0.0f);
	STATS_R_Y = iniReader.ReadFloat(section, "RIGHT_Y", 0.0f);
	STATS_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	STATS_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
	STATS_YSPACE = iniReader.ReadFloat(section, "SPACE", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "MENU_SETTINGS");
	MENU_SETTINGS_SHOW_RIGHT = iniReader.ReadInt(section, "SETTINGS_SHOW_RIGHT", 0);
	MENU_SETTINGS_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);
	MENU_SETTINGS_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	MENU_SETTINGS_Y_OFFSET = iniReader.ReadFloat(section, "POS_Y_OFFSET", 0.0f);
	MENU_SETTINGS_YSPACE = iniReader.ReadFloat(section, "SPACE", 0.0f);
	MENU_SETTINGS_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	MENU_SETTINGS_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "MENU_OPTIONS_RIGHT");
	MENU_R_X = iniReader.ReadFloat(section, "RIGHT_X", 0.0f);
	MENU_R_SIZE_X = iniReader.ReadFloat(section, "R_SIZE_X", 0.0f);
	MENU_R_SIZE_Y = iniReader.ReadFloat(section, "R_SIZE_Y", 0.0f);
	MENU_SLIDERS_X = iniReader.ReadFloat(section, "SLIDERS_X", 0.0f);
	MENU_SLIDERS_Y = iniReader.ReadFloat(section, "SLIDERS_Y", 0.0f);
	MENU_SLIDERS_SIZE_X = iniReader.ReadFloat(section, "SLIDERS_SIZE_X", 0.0f);
	MENU_SLIDERS_SIZE_Y = iniReader.ReadFloat(section, "SLIDERS_SIZE_Y", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "MENU_SAVE_LOAD");
	MENU_SAVELOAD_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);
	MENU_SAVELOAD_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	MENU_SAVELOAD_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	MENU_SAVELOAD_R = iniReader.ReadInt(section, "RED", 0);
	MENU_SAVELOAD_G = iniReader.ReadInt(section, "GREEN", 0);
	MENU_SAVELOAD_B = iniReader.ReadInt(section, "BLUE", 0);
	MENU_SAVELOAD_SEL_R = iniReader.ReadInt(section, "SEL_RED", 0);
	MENU_SAVELOAD_SEL_G = iniReader.ReadInt(section, "SEL_GREEN", 0);
	MENU_SAVELOAD_SEL_B = iniReader.ReadInt(section, "SEL_BLUE", 0);
	MENU_SAVELOAD_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	MENU_SAVELOAD_Y_OFFSET = iniReader.ReadFloat(section, "POS_Y_OFFSET", 0.0f);
	MENU_SAVELOAD_YSPACE = iniReader.ReadFloat(section, "YSPACE_OFFSET", 0.0f);
	MENU_SAVELOAD_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	MENU_SAVELOAD_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "TEST_RECTANGLE");
	MENU_TEST_RECT_X = iniReader.ReadFloat(section, "RECT_X", 0.0f);
	MENU_TEST_RECT_Y = iniReader.ReadFloat(section, "RECT_Y", 0.0f);
	MENU_TEST_RECT_W = iniReader.ReadFloat(section, "RECT_W", 0.0f);
	MENU_TEST_RECT_H = iniReader.ReadFloat(section, "RECT_H", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "NEW_GAME_SCREEN");
	MENU_NEWGAME_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);

	sprintf(section, "%s_%s", prefix[style], "MENU_BACKGROUND");
	BGMENU_TXD = iniReader.ReadString(section, "PATH", "404");
	BG_STYLE = iniReader.ReadInt(section, "STYLE", 0);
	BG_SCREENS = iniReader.ReadInt(section, "SCREENS", 0);
	BG_VC_SCREEN = iniReader.ReadInt(section, "VC_SCREEN", 0);
	BG_SEL_RECTS = iniReader.ReadInt(section, "SELECTION_RECTANGLES", 0);
	BG_V_ALIGN_MIDDLE = iniReader.ReadInt(section, "V_ALIGN_MIDDLE", 0);
	BG_MAP_MASK_ID = iniReader.ReadInt(section, "MAP_MASK_ID", 0);
	DISPLAY_LOGO_ON_MAIN_MENU = iniReader.ReadInt(section, "DISPLAY_LOGO_ON_MAIN_MENU", 0);
	DISPLAY_LOGO_ON_LOADING_SCREEN = iniReader.ReadInt(section, "DISPLAY_LOGO_ON_LOADING_SCREEN", 0);
	DISPLAY_LOGO_ON_FRONT_END = iniReader.ReadInt(section, "DISPLAY_LOGO_ON_FRONT_END", 0);
	LOGO_ID = iniReader.ReadInt(section, "LOGO_ID", 0);
	LOGO_X = iniReader.ReadFloat(section, "LOGO_X", 0.0f);
	LOGO_Y = iniReader.ReadFloat(section, "LOGO_Y", 0.0f);
	LOGO_SIZE_X = iniReader.ReadFloat(section, "LOGO_SIZE_X", 0.0f);
	LOGO_SIZE_Y = iniReader.ReadFloat(section, "LOGO_SIZE_Y", 0.0f);
	LOGO_SHADOW_OFFSET = iniReader.ReadFloat(section, "LOGO_SHADOW_OFFSET", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "TEMP_DATA");
	D_LOGO_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	D_LOGO_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	D_LOGO_W = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	D_LOGO_H = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
	D_LOGO_SHADOW_OFFSET = iniReader.ReadFloat(section, "SHADOW_OFFSET", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "BRIEF_MESSAGES");
	BRIEF_SHADOW = iniReader.ReadInt(section, "SHADOW", 0);
	BRIEF_OUTLINE = iniReader.ReadInt(section, "OUTLINE", 0);
	BRIEF_R = iniReader.ReadInt(section, "RED", 0);
	BRIEF_G = iniReader.ReadInt(section, "GREEN", 0);
	BRIEF_B = iniReader.ReadInt(section, "BLUE", 0);
	BRIEF_X = iniReader.ReadFloat(section, "POS_X", 0.0f);
	BRIEF_Y = iniReader.ReadFloat(section, "POS_Y", 0.0f);
	BRIEF_YSPACE = iniReader.ReadFloat(section, "YSPACE", 0.0f);
	BRIEF_SIZE_X = iniReader.ReadFloat(section, "SIZE_X", 0.0f);
	BRIEF_SIZE_Y = iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

	sprintf(section, "%s_%s", prefix[style], "MARKERS");
	MARKERS_R = iniReader.ReadInt(section, "RED", 0);
	MARKERS_G = iniReader.ReadInt(section, "GREEN", 0);
	MARKERS_B = iniReader.ReadInt(section, "BLUE", 0);
	ENEX_R = iniReader.ReadInt(section, "ENEX_R", 0);
	ENEX_G = iniReader.ReadInt(section, "ENEX_G", 0);
	ENEX_B = iniReader.ReadInt(section, "ENEX_B", 0);
	MARKERS_USE_ARROWS = iniReader.ReadInt(section, "USE_ARROWS", 0);
	MARKERS_ROTATE = iniReader.ReadInt(section, "ROTATE", 0);
	MARKERS_CONE_ALPHA = iniReader.ReadInt(section, "CONE_ALPHA", 255);
	MARKERS_CONE_GLOW = iniReader.ReadInt(section, "CONE_GLOW", 0);
	MARKERS_SIZE_CHANGE = iniReader.ReadInt(section, "SIZE_CHANGE", 0);
}
