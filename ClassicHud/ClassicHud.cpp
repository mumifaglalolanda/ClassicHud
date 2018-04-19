#include "plugin.h"
#include "CHud.h"
#include "CTimer.h"
#include "CFont.h"
#include "CWorld.h"
#include "CTxdStore.h"
#include "CFileMgr.h"
#include "CSprite2d.h"
#include "CClock.h"
#include "CStats.h"
#include "CMessages.h"
#include "CText.h"
#include "CHudColours.h"

#include "CCamera.h"
#include "CCutsceneMgr.h"
#include "CTheScripts.h"
#include "CMenuManager.h"
#include "CAERadioTrackManager.h"
#include "CPad.h"

#include "IniReader.h"

#define g_pHelpMessage ((char *)0xBAA7A0)
#define g_HelpMessageState (*(__int32 *)0xBAA474)
#define g_HelpMessageTimer (*(signed __int32 *)0xBAA47C)
#define g_HelpMessageFadeTimer (*(signed __int32 *)0xBAA478)
#define g_pHelpMessageToPrint ((char *)0xBAA480)
#define g_fTextBoxNumLines (*(float *)0xBAA460)
#define PLAYONESHOT(id, volume) ((void (__thiscall *)(unsigned int, unsigned short, float, float))0x506EA0)(0xB6BC90, id, volume, 1.0)
#define Camera_WidescreenOn (*(unsigned __int8 *)0xB6F065)
#define bTextBoxPermanent2 (*(__int8 *)0xBAA464)
#define m_HelpMessageQuick (*(unsigned __int8 *)0xBAA472)
#define Cutscene_Running (*(unsigned __int8 *)0xB5F851)
#define g_HelpMessageStatId (*(unsigned __int16 *)0xBAA470)
#define g_filenameBuffer ((char *)0xB71670)
#define flt_8D0938 (*(float *)0x8D0938)
#define GetGroupMembersCount(groupId) ((unsigned int (__thiscall *)(unsigned int))0x5F6AA0)(groupId * 0x2D4 + 0xC09928)
#define GetPlayerStat(statid) ((float (__cdecl *)(unsigned short))0x558E40)(statid)
#define dword_BAA468 (*(float *)0xBAA468)
#define g_fScriptTextBoxesWidth (*(float *)0x8D0934)
#define StatMax (*(unsigned __int16 *)0xBAA46C)
#define g_BigMessage0 ((char *)0xBAACC0)
#define g_BigMessage4 ((char *)0xBAAEC0)
#define byte_96C014 ((char *)0x96C014)
#define Menu_WidescreenOn (*(unsigned __int8 *)0xBA6793)
#define ZoneToPrint ((char *)0xBAB1D0)

#define CLASSICHUD_TXD_PATH "MODELS\\CLASSICHUD\\CLASSICHUD.TXD"
#define DEFAULT_WEAPONS_TXD "MODELS\\CLASSICHUD\\WEAPONS.TXD"
#define CLASSICHUD_DAT_PATH ".\\CLASSICHUD.DAT"

short& m_ItemToFlash = *(short*)0xBAB1DC;

using namespace plugin;

static int STYLE;

// Files
static char* HUD_TXD;
static char* FONTS_TXD;
static char* FONTS_DAT;
static char* WEAPONS_TXD;

// Money
static float MONEY_X, MONEY_Y, MONEY_WIDTH, MONEY_HEIGHT;
static int MONEY_R, MONEY_G, MONEY_B, MONEY_A, MONEY_SHADOW, MONEY_OUTLINE, MONEY_FONTSTYLE;

// Clock
static float CLOCK_X, CLOCK_Y, CLOCK_WIDTH, CLOCK_HEIGHT;
static int CLOCK_R, CLOCK_G, CLOCK_B, CLOCK_A, CLOCK_FONTSTYLE;

// Health
static float HEALTHICON_X, HEALTHICON_Y, HEALTHICON_WIDTH, HEALTHICON_HEIGHT,
			HEALTH_X, HEALTH_Y, HEALTH_WIDTH, HEALTH_HEIGHT,
			HEALTHBAR_X, HEALTHBAR_Y, HEALTHBAR_SIZE_X, HEALTHBAR_SIZE_Y;
static int HEALTH_DISPLAY, HEALTH_R, HEALTH_G, HEALTH_B, HEALTH_A, HEALTHBAR_BORDER, HEALTH_TYPE;

// Armour
static float ARMOUR_X,ARMOUR_Y,ARMOUR_SIZE_X,ARMOUR_SIZE_Y,
			ARMOURBAR_X,ARMOURBAR_Y,ARMOURBAR_SIZE_X,ARMOURBAR_SIZE_Y,
			ARMOURICON_X,ARMOURICON_Y,ARMOURICON_SIZE_X,ARMOURICON_SIZE_Y;
static int ARMOUR_R, ARMOUR_G, ARMOUR_B, ARMOUR_A, ARMOURBAR_BORDER, ARMOUR_TYPE;

// Breath
static float BREATH_X, BREATH_Y, BREATH_SIZE_X, BREATH_SIZE_Y,
			BREATHICON_X, BREATHICON_Y, BREATHICON_SIZE_X, BREATHICON_SIZE_Y,
			BREATHBAR_X, BREATHBAR_Y, BREATHBAR_SIZE_X, BREATHBAR_SIZE_Y;
static int BREATH_R, BREATH_G, BREATH_B, BREATH_A, BREATHBAR_BORDER, BREATH_TYPE;

// Ammo
static int AMMO_SHADOW, AMMO_OUTLINE, D_AMMO_R, D_AMMO_G, D_AMMO_B, AMMO_R, AMMO_G, AMMO_B;
static float AMMO_SIZE_X, AMMO_SIZE_Y, AMMO_X, AMMO_Y;

// Weapon
static int WEAPON_A;
static float WEAPON_X, WEAPON_Y, WEAPON_SIZE_X, WEAPON_SIZE_Y;
static int STAT_WEP_ALPHA;
static float stat3_2;

// Wanted Level
static float STAR_X, STAR_Y, STAR_SIZE_X, STAR_SIZE_Y, STAR_SPACE, STAR_X_OFFSET, STAR_Y_OFFSET;
static int STAR_R, STAR_G, STAR_B, STARBACK_R, STARBACK_G, STARBACK_B, SHADOWSTAR_A, 
FLASHSTAR_R, FLASHSTAR_G, FLASHSTAR_B, STAR_A2, STAR_A3, SHADOWSTAR_A2, SHADOWSTAR_A3, STAR_STYLE;

// Area
static int AREA_FONTSTYLE, AREA_R, AREA_G, AREA_B, AREA_SHADOW, AREA_OUTLINE;
static float AREA_SIZE_X, AREA_SIZE_Y, AREA_X, AREA_Y, AREA_SLANT;

// Vehicle
static int VEHICLE_FONTSTYLE, VEHICLE_R, VEHICLE_G, VEHICLE_B, VEHICLE_SHADOW, VEHICLE_OUTLINE;
static float VEHICLE_SIZE_X, VEHICLE_SIZE_Y, VEHICLE_X, VEHICLE_Y, VEHICLE_SLANT;

// Subtitles
static int SUBTITLES_SHADOW, SUBTITLES_OUTLINE, SUBTITLES_R, SUBTITLES_G, SUBTITLES_B;
static float SUBTITLES_X, SUBTITLES_Y, SUBTITLES_SIZE_X, SUBTITLES_SIZE_Y,
SUBTITLES_VITALSTATS_X, SUBTITLES_VITALSTATS_SIZE_X, SUBTITLES_LINEWIDTH;

// Text box (Help Message)
static float fTextBoxPosnX = 50.0f;
static float fTextBoxPosnXWithRadar = 50.0f;
static float fTextBoxPosnXWithRadarAndPlane = 50.0f;
static float fTextBoxPosnY = 50.0;
static float fTextBoxFontScaleX = 0.7f;
static float fTextBoxFontScaleY = 1.9f;
static float fTextBoxWidth = 421.0f;
static float fTextBoxBorderSize = 6.0f;

// Radar
static float RADAR_RANGE, RADAR_X, RADAR_Y, RADAR_SIZE_X, RADAR_SIZE_Y;
static int RADAR_A;
static void __declspec(naked) RadarAlpha() {
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void *>(TRUE));
	_asm {
		mov     ecx, 586420h
		mov     al, [esp + 140h - 12Dh]
		test    al, al
		jmp     ecx
	}
}
void RotateVertices(CVector2D *rect, unsigned int numVerts, float x, float y, float angle) {
	float xold, yold;
	// angle /= 57.2957795;
	float _cos = cosf(angle);
	float _sin = sinf(angle);
	for (unsigned int i = 0; i < numVerts; i++) {
		xold = rect[i].x;
		yold = rect[i].y;
		rect[i].x = x + (xold - x) * _cos + (yold - y) * _sin;
		rect[i].y = y - (xold - x) * _sin + (yold - y) * _cos;
	}
}
float BilinearOffset(float coord) {
	return static_cast<float>(static_cast<int>(coord)) - 0.5f;
}

// Success Failed Message (Styled Text 1)
static int STYLED1_SHADOW, STYLED1_OUTLINE, STYLED1_R, STYLED1_G, STYLED1_B;
static float STYLED1_Y, STYLED1_Y1, STYLED1_Y2, STYLED1_SIZE_X, STYLED1_SIZE_Y;

// Mission Title (Styled Text 2)
static int MT_R, MT_G, MT_B, MT_SHADOW, MT_OUTLINE, MT_FONTSTYLE;
static float MT_POS_X, MT_POS_Y, MT_SIZE_X, MT_SIZE_Y;

// Wasted Busted Message (Styled Text 3)
static int WB_R, WB_G, WB_B, WB_SHADOW, WB_OUTLINE, WB_FONTSTYLE, WB_ALIGN;
static float WB_POS_X, WB_POS_Y, WB_SIZE_X, WB_SIZE_Y;

// Odd Job Messages 1 (Styled Text 4)
static int STYLED4_FONTSTYLE, STYLED4_SHADOW, STYLED4_OUTLINE, STYLED4_R, STYLED4_G, STYLED4_B, STYLED4_ALIGN;
static float STYLED4_X, STYLED4_Y, STYLED4_SIZE_X, STYLED4_SIZE_Y;

// Odd Job Messages 2 (Styled Text 5)
static int STYLED5_FONTSTYLE, STYLED5_SHADOW, STYLED5_OUTLINE, STYLED5_R, STYLED5_G, STYLED5_B;
static float STYLED5_Y, STYLED5_SIZE_X, STYLED5_SIZE_Y;

// Odd Job Messages 3 (Styled Text 6)
static int STYLED6_FONTSTYLE, STYLED6_SHADOW, STYLED6_OUTLINE, STYLED6_R, STYLED6_G, STYLED6_B;
static float STYLED6_Y, STYLED6_SIZE_X, STYLED6_SIZE_Y;

// Odd Job Messages 4 (Styled Text 7)
static int STYLED7_FONTSTYLE, STYLED7_SHADOW, STYLED7_OUTLINE, STYLED7_R, STYLED7_G, STYLED7_B;
static float STYLED7_Y, STYLED7_SIZE_X, STYLED7_SIZE_Y;

// Mission Timers (Timer and Status Text)
static int M_TIMERS_FONTSTYLE, M_TIMERS_SHADOW, M_TIMERS_OUTLINE, M_TIMERS_R, M_TIMERS_G, M_TIMERS_B;
static float M_TIMERS_SIZE_X, M_TIMERS_SIZE_Y, M_TIMER_X, M_TIMER_Y, M_TIMER_DESC_X;
static int M_STATUSBAR_BORDER, M_STATUSBAR_R, M_STATUSBAR_G, M_STATUSBAR_B, M_STATUSBAR_A;
static float M_STATUSBAR_X, M_STATUSBAR_Y, M_STATUSBAR_SIZE_X, M_STATUSBAR_SIZE_Y;
static float M_STATUSTEXT_X, M_STATUSTEXT_Y, M_STATUSTEXT_DESC_X, M_STATUSTEXT_SPACE;

// Radio Station Text
static int RS_FONTSTYLE, RS_SHADOW, RS_OUTLINE, RS_RED1, RS_GREEN1, RS_BLUE1, RS_RED2, RS_GREEN2, RS_BLUE2;
static float RS_POS_Y, RS_SIZE_X, RS_SIZE_Y;

// Garage Messages
static int GARAGES_R, GARAGES_G, GARAGES_B, GARAGES_FONTSTYLE, GARAGES_SHADOW, GARAGES_OUTLINE;
static float GARAGES_Y, GARAGES_SIZE_X, GARAGES_SIZE_Y, GARAGES_LINEWIDTH;

// Custom text (for Hud Editor)
static int CUSTOM_ALIGN, CUSTOM_SHADOW, CUSTOM_OUTLINE, CUSTOM_FONTSTYLE, CUSTOM_R, CUSTOM_G, CUSTOM_B, CUSTOM_A;
static float CUSTOM_X, CUSTOM_Y, CUSTOM_SIZE_X, CUSTOM_SIZE_Y, CUSTOM_LINEWIDTH;

CRGBA AreaCRGBA;

static float HUD_POS_X, HUD_POS_Y;

CSprite2d hudIcons[10];
CSprite2d weaponicons[50];
CSprite2d sprite;

class ClassicHud {
public:

	static void HudTextures(int index) {
		CTxdStore::LoadTxd(index, HUD_TXD);
	}

	static void FontTextures(int index) {
		CTxdStore::LoadTxd(index, FONTS_TXD);
	}

	static void FontData() {
		CFileMgr::OpenFile(FONTS_DAT, "rb");
	}

	static void DrawMoney(void)
	{
		int displayedScore = CWorld::Players[CWorld::PlayerInFocus].m_nDisplayMoney;
		const char* displayScore;
		char str[16];

		if (displayedScore < 0)
		{
			CFont::SetColor(CRGBA(255, 38, 41, 255));
			displayedScore = -displayedScore;
			displayScore = "-$%07d";
		}
		else
		{
			CFont::SetColor(CRGBA(MONEY_R, MONEY_G, MONEY_B, 255));
			displayScore = "$%08d";
		}

		_snprintf(str, sizeof(str), displayScore, displayedScore);

		CFont::SetBackground(0, 0);
		CFont::SetProp(false);
		CFont::SetAlignment(ALIGN_RIGHT);
		CFont::SetFontStyle(MONEY_FONTSTYLE);
		CFont::SetRightJustifyWrap(0.0);
		CFont::SetScale(SCREEN_MULTIPLIER(MONEY_WIDTH), SCREEN_MULTIPLIER(MONEY_HEIGHT));
		CFont::SetDropShadowPosition(MONEY_SHADOW);
		if (MONEY_OUTLINE != 0)
			CFont::SetOutlinePosition(MONEY_OUTLINE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + MONEY_X), SCREEN_COORD(HUD_POS_Y + MONEY_Y), str);
	}

	static void DrawClock(float posX, float posY) {
		char str[16];
		CClock* CClock;

		CFont::SetProp(0);
		CFont::SetBackground(0, 0);
		CFont::SetScale(SCREEN_MULTIPLIER(CLOCK_WIDTH), SCREEN_MULTIPLIER(CLOCK_HEIGHT));
		CFont::SetAlignment(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0);
		CFont::SetFontStyle(CLOCK_FONTSTYLE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(CLOCK_R, CLOCK_G, CLOCK_B, 255));
		CFont::SetDropShadowPosition(MONEY_SHADOW);
		if (MONEY_OUTLINE != 0)
			CFont::SetOutlinePosition(MONEY_OUTLINE);

		_snprintf(str, sizeof(str), "%02d:%02d", CClock->ms_nGameClockHours, CClock->ms_nGameClockMinutes);

		CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + CLOCK_X), SCREEN_COORD(HUD_POS_Y + CLOCK_Y), str);
	}

	static void ClassicHudTextures() {
		int v0 = CTxdStore::AddTxdSlot("classichud");
		CTxdStore::LoadTxd(v0, CLASSICHUD_TXD_PATH);
		CTxdStore::AddRef(v0);
		CTxdStore::PushCurrentTxd();
		CTxdStore::SetCurrentTxd(v0);
		hudIcons[0].SetTexture("health");
		hudIcons[1].SetTexture("armour");
		hudIcons[2].SetTexture("breath");
		hudIcons[3].SetTexture("star");
		hudIcons[4].SetTexture("arrow");
		hudIcons[5].SetTexture("bar_squareOutline");
		hudIcons[6].SetTexture("bar_roundOutline");
		hudIcons[7].SetTexture("star_Outline");
		hudIcons[8].SetTexture("sa_star");
		hudIcons[9].SetTexture("stories_star");
		CTxdStore::PopCurrentTxd();
	}

	static void ClassicHudTextureShutdown() {
		hudIcons[0].Delete();
		hudIcons[1].Delete();
		hudIcons[2].Delete();
		hudIcons[3].Delete();
		hudIcons[4].Delete();
		hudIcons[5].Delete();
		hudIcons[6].Delete();
		hudIcons[7].Delete();
		hudIcons[8].Delete();
		hudIcons[9].Delete();
		int v0 = CTxdStore::FindTxdSlot("classichud");
		CTxdStore::RemoveTxdSlot(v0);
	}

	static void DrawWeaponAmmo(float x, float y, char *string) {
		CFont::SetBackground(0, 0);
		CFont::SetDropShadowPosition(AMMO_SHADOW);
		if (AMMO_OUTLINE != 0)
			CFont::SetOutlinePosition(AMMO_OUTLINE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(AMMO_R, AMMO_G, AMMO_B, 255));
		CFont::SetScale(SCREEN_MULTIPLIER(AMMO_SIZE_X), SCREEN_MULTIPLIER(AMMO_SIZE_Y));
		CFont::PrintString(
			RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + AMMO_X),
			SCREEN_COORD(HUD_POS_Y + AMMO_Y), string);
	}

	static void WeaponTextures() {
		int v1 = CTxdStore::AddTxdSlot("weapons");
		CTxdStore::LoadTxd(v1, WEAPONS_TXD);
		CTxdStore::AddRef(v1);
		CTxdStore::PushCurrentTxd();
		CTxdStore::SetCurrentTxd(v1);
		weaponicons[0].SetTexture("UNARMED");
		weaponicons[1].SetTexture("GUN_DILDO1");
		weaponicons[2].SetTexture("GUN_DILDO2");
		weaponicons[3].SetTexture("GUN_VIBE1");
		weaponicons[4].SetTexture("GUN_VIBE2");
		weaponicons[5].SetTexture("FLOWERA");
		weaponicons[6].SetTexture("GUN_CANE");
		weaponicons[7].SetTexture("GUN_BOXWEE");
		weaponicons[8].SetTexture("GUN_BOXBIG");
		weaponicons[9].SetTexture("CELLPHONE");
		weaponicons[10].SetTexture("BRASSKNUCKLE");
		weaponicons[11].SetTexture("GOLFCLUB");
		weaponicons[12].SetTexture("NITESTICK");
		weaponicons[13].SetTexture("KNIFECUR");
		weaponicons[14].SetTexture("BAT");
		weaponicons[15].SetTexture("SHOVEL");
		weaponicons[16].SetTexture("POOLCUE");
		weaponicons[17].SetTexture("KATANA");
		weaponicons[18].SetTexture("CHNSAW");
		weaponicons[19].SetTexture("GRENADE");
		weaponicons[20].SetTexture("TEARGAS");
		weaponicons[21].SetTexture("MOLOTOV");
		weaponicons[22].SetTexture("MISSILE");
		weaponicons[23].SetTexture("COLT45");
		weaponicons[24].SetTexture("SILENCED");
		weaponicons[25].SetTexture("DESERT_EAGLE");
		weaponicons[26].SetTexture("CHROMEGUN");
		weaponicons[27].SetTexture("SAWNOFF");
		weaponicons[28].SetTexture("SHOTGSPA");
		weaponicons[29].SetTexture("MICRO_UZI");
		weaponicons[30].SetTexture("MP5LNG");
		weaponicons[31].SetTexture("FLARE");
		weaponicons[32].SetTexture("AK47");
		weaponicons[33].SetTexture("M4");
		weaponicons[34].SetTexture("CUNTGUN");
		weaponicons[35].SetTexture("SNIPER");
		weaponicons[36].SetTexture("ROCKETLA");
		weaponicons[37].SetTexture("HEATSEEK");
		weaponicons[38].SetTexture("FLAME");
		weaponicons[39].SetTexture("MINIGUN");
		weaponicons[40].SetTexture("SATCHEL");
		weaponicons[41].SetTexture("DETONATOR");
		weaponicons[42].SetTexture("SPRAYCAN");
		weaponicons[43].SetTexture("FIRE_EX");
		weaponicons[44].SetTexture("CAMERA");
		weaponicons[45].SetTexture("GOGGLES");
		weaponicons[46].SetTexture("GOGGLES");
		weaponicons[47].SetTexture("JETPACK");
		weaponicons[48].SetTexture("GUN_PARA");
		weaponicons[49].SetTexture("TEC9");
		CTxdStore::PopCurrentTxd();
	}

	static void WeaponTexturesShutdown() {
		weaponicons[0].Delete();
		weaponicons[1].Delete();
		weaponicons[2].Delete();
		weaponicons[3].Delete();
		weaponicons[4].Delete();
		weaponicons[5].Delete();
		weaponicons[6].Delete();
		weaponicons[7].Delete();
		weaponicons[8].Delete();
		weaponicons[9].Delete();
		weaponicons[10].Delete();
		weaponicons[11].Delete();
		weaponicons[12].Delete();
		weaponicons[13].Delete();
		weaponicons[14].Delete();
		weaponicons[15].Delete();
		weaponicons[16].Delete();
		weaponicons[17].Delete();
		weaponicons[18].Delete();
		weaponicons[19].Delete();
		weaponicons[20].Delete();
		weaponicons[21].Delete();
		weaponicons[22].Delete();
		weaponicons[23].Delete();
		weaponicons[24].Delete();
		weaponicons[25].Delete();
		weaponicons[26].Delete();
		weaponicons[27].Delete();
		weaponicons[28].Delete();
		weaponicons[29].Delete();
		weaponicons[30].Delete();
		weaponicons[31].Delete();
		weaponicons[32].Delete();
		weaponicons[33].Delete();
		weaponicons[34].Delete();
		weaponicons[35].Delete();
		weaponicons[36].Delete();
		weaponicons[37].Delete();
		weaponicons[38].Delete();
		weaponicons[39].Delete();
		weaponicons[40].Delete();
		weaponicons[41].Delete();
		weaponicons[42].Delete();
		weaponicons[43].Delete();
		weaponicons[44].Delete();
		weaponicons[45].Delete();
		weaponicons[46].Delete();
		weaponicons[47].Delete();
		weaponicons[48].Delete();
		weaponicons[49].Delete();
		int v1 = CTxdStore::FindTxdSlot("weapons");
		CTxdStore::RemoveTxdSlot(v1);
	}

	static void PrintIcon() {
		RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
		sprite.Draw(
			RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + WEAPON_X),
			SCREEN_COORD(HUD_POS_Y + WEAPON_Y),
			SCREEN_MULTIPLIER(WEAPON_SIZE_X),
			SCREEN_MULTIPLIER(WEAPON_SIZE_Y), CRGBA(255, 255, 255, WEAPON_A));
		RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
	}

	static void DrawWeaponIcon(CPed *player) {
		RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void *)rwFILTERLINEAR);
		int modelId = CWeaponInfo::GetWeaponInfo((eWeaponType)player->m_aWeapons[player->m_nActiveWeaponSlot].m_nType, 1)->m_nModelId1;
		if (modelId <= 0) {
			sprite = weaponicons[0];
			PrintIcon();
			stat3_2 = CStats::GetStatValue(-1);
			STAT_WEP_ALPHA = 50;
		}
		else {
			switch (modelId) {
			case MODEL_GUN_DILDO1:
				sprite = weaponicons[1];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_GUN_DILDO2:
				sprite = weaponicons[2];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_GUN_VIBE1:
				sprite = weaponicons[3];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_GUN_VIBE2:
				sprite = weaponicons[4];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_FLOWERA:
				sprite = weaponicons[5];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_GUN_CANE:
				sprite = weaponicons[6];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_GUN_BOXWEE:
				sprite = weaponicons[7];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_GUN_BOXBIG:
				sprite = weaponicons[8];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_CELLPHONE:
				sprite = weaponicons[9];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_BRASSKNUCKLE:
				sprite = weaponicons[10];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_GOLFCLUB:
				sprite = weaponicons[11];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_NITESTICK:
				sprite = weaponicons[12];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_KNIFECUR:
				sprite = weaponicons[13];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_BAT:
				sprite = weaponicons[14];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_SHOVEL:
				sprite = weaponicons[15];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_POOLCUE:
				sprite = weaponicons[16];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_KATANA:
				sprite = weaponicons[17];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_CHNSAW:
				sprite = weaponicons[18];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_GRENADE:
				sprite = weaponicons[19];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_TEARGAS:
				sprite = weaponicons[20];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_MOLOTOV:
				sprite = weaponicons[21];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_MISSILE:
				sprite = weaponicons[22];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_COLT45:
				sprite = weaponicons[23];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(69)* 0.001f * 100.0f;
				STAT_WEP_ALPHA = 255;
				break;
			case MODEL_SILENCED:
				sprite = weaponicons[24];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(70)* 0.001f * 100.0f;
				STAT_WEP_ALPHA = 255;
				break;
			case MODEL_DESERT_EAGLE:
				sprite = weaponicons[25];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(71)* 0.001f * 100.0f;
				STAT_WEP_ALPHA = 255;
				break;
			case MODEL_CHROMEGUN:
				sprite = weaponicons[26];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(72)* 0.001f * 100.0f;
				STAT_WEP_ALPHA = 255;
				break;
			case MODEL_SAWNOFF:
				sprite = weaponicons[27];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(73)* 0.001f * 100.0f;
				STAT_WEP_ALPHA = 255;
				break;
			case MODEL_SHOTGSPA:
				sprite = weaponicons[28];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(74)* 0.001f * 100.0f;
				STAT_WEP_ALPHA = 255;
				break;
			case MODEL_MICRO_UZI:
				sprite = weaponicons[29];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(75)* 0.001f * 100.0f;
				STAT_WEP_ALPHA = 255;
				break;
			case MODEL_MP5LNG:
				sprite = weaponicons[30];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(76)* 0.001f * 100.0f;
				STAT_WEP_ALPHA = 255;
				break;
			case MODEL_FLARE:
				sprite = weaponicons[31];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 255;
				break;
			case MODEL_AK47:
				sprite = weaponicons[32];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(77)* 0.001f * 100.0f;
				STAT_WEP_ALPHA = 255;
				break;
			case MODEL_M4:
				sprite = weaponicons[33];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(78)* 0.001f * 100.0f;
				STAT_WEP_ALPHA = 255;
				break;
			case MODEL_CUNTGUN:
				sprite = weaponicons[34];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_SNIPER:
				sprite = weaponicons[35];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(79)* 0.001f * 100.0f;
				STAT_WEP_ALPHA = 255;
				break;
			case MODEL_ROCKETLA:
				sprite = weaponicons[36];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_HEATSEEK:
				sprite = weaponicons[37];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_FLAME:
				sprite = weaponicons[38];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_MINIGUN:
				sprite = weaponicons[39];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_SATCHEL:
				sprite = weaponicons[40];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_BOMB:
				sprite = weaponicons[41];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_SPRAYCAN:
				sprite = weaponicons[42];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_FIRE_EX:
				sprite = weaponicons[43];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_CAMERA:
				sprite = weaponicons[44];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_NVGOGGLES:
				sprite = weaponicons[45];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_IRGOGGLES:
				sprite = weaponicons[46];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_JETPACK:
				sprite = weaponicons[47];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_GUN_PARA:
				sprite = weaponicons[48];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			case MODEL_TEC9:
				sprite = weaponicons[49];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(-1);
				STAT_WEP_ALPHA = 50;
				break;
			}
		}
	}

	static void PrintStar(float x, float y, float width, float height, CRGBA color) {
		int id = (STAR_STYLE < 0 || STAR_STYLE > 2) ? 0 : STAR_STYLE;
		int star[3] = { 8, 3, 9 };
		RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
		hudIcons[star[id]].Draw((x), (y), (width), (height), CRGBA(color));
		RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
	}

	static void DrawWanted() {
		int nWantedLevel = FindPlayerWanted(-1)->m_nWantedLevel;
		int nTimeOfWLChange = FindPlayerWanted(-1)->m_nLastTimeWantedLevelChanged;
		int nWantedLevelBeforeParole = FindPlayerWanted(-1)->m_nWantedLevelBeforeParole;

		float fCurrentPos = RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + STAR_X);
		float off_x1, off_x2, off_y1, off_y2, off_w1, off_w2, off_h1, off_h2;
		if (STAR_STYLE == 0) {
			off_x1 = -SCREEN_COORD(STAR_X_OFFSET), off_x2 = -off_x1;
			off_y1 = -SCREEN_COORD(STAR_Y_OFFSET), off_y2 = -off_y1;
			off_w1 = -off_x1*2.0f, off_w2 = off_x1*2.0f;
			off_h1 = -off_y1*2.0f, off_h2 = off_y1*2.0f;
		}
		else {
			off_x1 = 2.0f, off_x2 = 0.0f;
			off_y1 = SCREEN_COORD(2.0f), off_y2 = 0.0f;
			off_w1 = 0.0f, off_w2 = 0.0f;
			off_h1 = 0.0f, off_h2 = 0.0f;
		}

		if (nWantedLevel) { // Active Stars
			for (int i = 0; i < 6; ++i, fCurrentPos -= SCREEN_MULTIPLIER(STAR_SPACE)) {
				if (nWantedLevel > i && (CTimer::m_snTimeInMilliseconds > nTimeOfWLChange + 2000 || ShowFlashingItem(150, 150))) {
					PrintStar(
						fCurrentPos + off_x1, SCREEN_COORD(HUD_POS_Y + STAR_Y) + off_y1,
						SCREEN_MULTIPLIER(STAR_SIZE_X) + off_w1,
						SCREEN_MULTIPLIER(STAR_SIZE_Y) + off_h1,
						CRGBA(0, 0, 0, SHADOWSTAR_A)); // Shadow
					PrintStar(
						fCurrentPos + off_x2, SCREEN_COORD(HUD_POS_Y + STAR_Y) + off_y2,
						SCREEN_MULTIPLIER(STAR_SIZE_X) + off_w2,
						SCREEN_MULTIPLIER(STAR_SIZE_Y) + off_h2,
						CRGBA(STAR_R, STAR_G, STAR_B, 255));
				}
				else { // Inactive Stars during wanted level > 0
					PrintStar(
						fCurrentPos + 2.0f, SCREEN_COORD(HUD_POS_Y + STAR_Y + 2.0f),
						SCREEN_MULTIPLIER(STAR_SIZE_X),
						SCREEN_MULTIPLIER(STAR_SIZE_Y),
						CRGBA(0, 0, 0, SHADOWSTAR_A2)); // Shadow
					PrintStar(fCurrentPos, SCREEN_COORD(HUD_POS_Y + STAR_Y),
						SCREEN_MULTIPLIER(STAR_SIZE_X), SCREEN_MULTIPLIER(STAR_SIZE_Y),
						CRGBA(STARBACK_R, STARBACK_G, STARBACK_B, STAR_A2));
				}
			}
		}
		else if (nWantedLevelBeforeParole) { // Flashing stars during suspended wanted level
			for (int i = 0; i < 6; ++i, fCurrentPos -= SCREEN_MULTIPLIER(STAR_SPACE)) {
				if (nWantedLevelBeforeParole > i && CTimer::m_FrameCounter & 4) {
					PrintStar(
						fCurrentPos + off_x1, SCREEN_COORD(HUD_POS_Y + STAR_Y) + off_y1,
						SCREEN_MULTIPLIER(STAR_SIZE_X) + off_w1,
						SCREEN_MULTIPLIER(STAR_SIZE_Y) + off_h1,
						CRGBA(0, 0, 0, SHADOWSTAR_A)); // Shadow
					PrintStar(
						fCurrentPos + off_x2, SCREEN_COORD(HUD_POS_Y + STAR_Y) + off_y2,
						SCREEN_MULTIPLIER(STAR_SIZE_X) + off_w2,
						SCREEN_MULTIPLIER(STAR_SIZE_Y) + off_h2,
						CRGBA(FLASHSTAR_R, FLASHSTAR_G, FLASHSTAR_B, 255));
				}
				else {
					PrintStar(
						fCurrentPos + 2.0f, SCREEN_COORD(HUD_POS_Y + STAR_Y + 2.0f),
						SCREEN_MULTIPLIER(STAR_SIZE_X),
						SCREEN_MULTIPLIER(STAR_SIZE_Y),
						CRGBA(0, 0, 0, SHADOWSTAR_A2)); // Shadow
					PrintStar(fCurrentPos, SCREEN_COORD(HUD_POS_Y + STAR_Y),
						SCREEN_MULTIPLIER(STAR_SIZE_X), SCREEN_MULTIPLIER(STAR_SIZE_Y),
						CRGBA(STARBACK_R, STARBACK_G, STARBACK_B, STAR_A2));
				}
			}
		}
		else { // Inactive stars
			float fCurrentPos = RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + STAR_X);
			for (int i = 0; i < 6; ++i, fCurrentPos -= SCREEN_MULTIPLIER(STAR_SPACE)) {
				PrintStar(fCurrentPos + 2.0f, SCREEN_COORD(HUD_POS_Y + STAR_Y + 2.0f),
					SCREEN_MULTIPLIER(STAR_SIZE_X),
					SCREEN_MULTIPLIER(STAR_SIZE_Y),
					CRGBA(0, 0, 0, SHADOWSTAR_A3)); // Shadow
				PrintStar(fCurrentPos, SCREEN_COORD(HUD_POS_Y + STAR_Y),
					SCREEN_MULTIPLIER(STAR_SIZE_X),
					SCREEN_MULTIPLIER(STAR_SIZE_Y),
					CRGBA(STARBACK_R, STARBACK_G, STARBACK_B, STAR_A3));
			}
		}
	}

	static void AreaName(float x, float y, char* str) {
		CFont::SetFontStyle(AREA_FONTSTYLE);
		CFont::SetBackground(0, 0);
		CFont::SetScale(SCREEN_MULTIPLIER(AREA_SIZE_X), SCREEN_MULTIPLIER(AREA_SIZE_Y));
		CFont::SetAlignment(ALIGN_RIGHT);
		CFont::SetDropShadowPosition(AREA_SHADOW);
		if (AREA_OUTLINE != 0)
			CFont::SetOutlinePosition(AREA_OUTLINE);
		CFont::SetSlantRefPoint(RsGlobal.maximumWidth - SCREEN_COORD(AREA_X), SCREEN_COORD_BOTTOM(AREA_Y));
		CFont::SetSlant(AREA_SLANT);
		CFont::PrintString(
			RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + AREA_X),
			SCREEN_COORD_BOTTOM(HUD_POS_Y + AREA_Y), str);
	}

	static void AreaNameColor(CRGBA value)
	{
		CFont::SetColor(CRGBA(AREA_R, AREA_G, AREA_B, value.alpha));
		AreaCRGBA = value;
	}

	static void VehicleName(float x, float y, char* str) {
		CFont::SetFontStyle(VEHICLE_FONTSTYLE);
		CFont::SetBackground(0, 0);
		CFont::SetScale(SCREEN_MULTIPLIER(VEHICLE_SIZE_X), SCREEN_MULTIPLIER(VEHICLE_SIZE_Y));
		CFont::SetAlignment(ALIGN_RIGHT);
		CFont::SetDropShadowPosition(VEHICLE_SHADOW);
		if (VEHICLE_OUTLINE != 0)
			CFont::SetOutlinePosition(VEHICLE_OUTLINE);
		CFont::SetSlantRefPoint(RsGlobal.maximumWidth - SCREEN_COORD(VEHICLE_X), SCREEN_COORD_BOTTOM(VEHICLE_Y));
		CFont::SetSlant(VEHICLE_SLANT);
		CFont::PrintString(
			RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + VEHICLE_X),
			SCREEN_COORD_BOTTOM(HUD_POS_Y + VEHICLE_Y), str);
	}

	static void VehicleNameColor(CRGBA value)
	{
		CFont::SetColor(CRGBA(VEHICLE_R, VEHICLE_G, VEHICLE_B, value.alpha));
		AreaCRGBA = value;
	}

	static void PrintHealthString(float posX, float posY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine,
		BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor) {
		int Percentage = 0;
		
		CFont::SetFontStyle(FONT_PRICEDOWN);
		CFont::SetBackground(0, 0);
		CFont::SetScale(SCREEN_MULTIPLIER(HEALTH_WIDTH), SCREEN_MULTIPLIER(HEALTH_HEIGHT));
		CFont::SetColor(CRGBA(HEALTH_R, HEALTH_G, HEALTH_B, HEALTH_A));
		CFont::SetDropColor(CRGBA(0, 0, 0, HEALTH_A));
		CFont::SetDropShadowPosition(1);
		//CFont::SetOutlinePosition(HUD_GLOBAL_OUTLINE);
		char str[10];
		CPed *playa = FindPlayerPed(0);

		if (Percentage == 1)
			sprintf(str, "%03d", (int)fPercentage);
		else
			sprintf(str, "%03d", (int)playa->m_fHealth);

		if (m_ItemToFlash != 3 || ShowFlashingItem(300, 300)) {
			if (CWorld::Players[0].m_nLastTimeArmourLost == CWorld::Players[0].m_nLastTimeEnergyLost || CWorld::Players[0].m_nLastTimeEnergyLost + 1000 < CTimer::m_snTimeInMilliseconds || ShowFlashingItem(150, 150))
			{
				CFont::PrintString(
					RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HEALTH_X),
					SCREEN_COORD(HUD_POS_Y + HEALTH_Y),
					str);

				RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
				hudIcons[0].Draw(
					RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HEALTHICON_X + -2.0f),
					SCREEN_COORD(HUD_POS_Y + HEALTHICON_Y + 2.0f),
					SCREEN_MULTIPLIER(HEALTHICON_WIDTH), SCREEN_MULTIPLIER(HEALTHICON_HEIGHT),
					CRGBA(0, 0, 0, HEALTH_A));
				hudIcons[0].Draw(
					RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HEALTHICON_X),
					SCREEN_COORD(HUD_POS_Y + HEALTHICON_Y),
					SCREEN_MULTIPLIER(HEALTHICON_WIDTH), SCREEN_MULTIPLIER(HEALTHICON_HEIGHT),
					CRGBA(HEALTH_R, HEALTH_G, HEALTH_B, HEALTH_A));
				RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
			}
		}
	}

	static void PrintArmourString(float posX, float posY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine,
		BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor) {
		int Percentage = 0;
		CFont::SetFontStyle(FONT_PRICEDOWN);
		CFont::SetBackground(0, 0);
		CFont::SetScale(SCREEN_MULTIPLIER(ARMOUR_SIZE_X), SCREEN_MULTIPLIER(ARMOUR_SIZE_Y));
		CFont::SetColor(CRGBA(ARMOUR_R, ARMOUR_G, ARMOUR_B, ARMOUR_A));
		CFont::SetDropColor(CRGBA(0, 0, 0, ARMOUR_A));
		CFont::SetDropShadowPosition(1);
		//CFont::SetOutlinePosition(HUD_GLOBAL_OUTLINE);
		char str[10];
		CPed *playa = FindPlayerPed(0);
		if (Percentage == 1)
			sprintf(str, "%03d", (int)fPercentage);
		else
			sprintf(str, "%03d", (int)playa->m_fArmour);

		if (m_ItemToFlash != 4 || ShowFlashingItem(300, 300)) {
			if (CWorld::Players[0].m_nLastTimeArmourLost == 0 || CWorld::Players[0].m_nLastTimeArmourLost + 1000 < CTimer::m_snTimeInMilliseconds || ShowFlashingItem(150, 150))
			{
				CFont::PrintString(
					RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + ARMOUR_X),
					SCREEN_COORD(HUD_POS_Y + ARMOUR_Y), str);

				RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
				hudIcons[1].Draw(
					RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + ARMOURICON_X + -2.0f),
					SCREEN_COORD(HUD_POS_Y + ARMOURICON_Y + 2.0f),
					SCREEN_MULTIPLIER(ARMOURICON_SIZE_X),
					SCREEN_MULTIPLIER(ARMOURICON_SIZE_Y),
					CRGBA(0, 0, 0, ARMOUR_A));
				hudIcons[1].Draw(
					RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + ARMOURICON_X),
					SCREEN_COORD(HUD_POS_Y + ARMOURICON_Y),
					SCREEN_MULTIPLIER(ARMOURICON_SIZE_X),
					SCREEN_MULTIPLIER(ARMOURICON_SIZE_Y),
					CRGBA(ARMOUR_R, ARMOUR_G, ARMOUR_B, ARMOUR_A));
				RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
			}
		}
	}

	static void PrintBreathString(float posX, float posY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine,
		BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor)
	{
		CFont::SetFontStyle(FONT_PRICEDOWN);
		CFont::SetBackground(0, 0);
		CFont::SetScale(SCREEN_MULTIPLIER(BREATH_SIZE_X), SCREEN_MULTIPLIER(BREATH_SIZE_Y));
		CFont::SetColor(CRGBA(BREATH_R, BREATH_G, BREATH_B, BREATH_A));
		CFont::SetDropColor(CRGBA(0, 0, 0, BREATH_A));
		CFont::SetDropShadowPosition(1);
		char str[10];
		sprintf(str, "%03d", (int)fPercentage);

		CFont::PrintString(
			RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + BREATH_X),
			SCREEN_COORD(HUD_POS_Y + BREATH_Y), str);

		RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
		hudIcons[2].Draw(
			RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + BREATHICON_X + -2.0f),
			SCREEN_COORD(HUD_POS_Y + BREATHICON_Y + 2.0f), SCREEN_MULTIPLIER(BREATHICON_SIZE_X),
			SCREEN_MULTIPLIER(BREATHICON_SIZE_Y), CRGBA(0, 0, 0, 255));
		hudIcons[2].Draw(
			RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + BREATHICON_X),
			SCREEN_COORD(HUD_POS_Y + BREATHICON_Y), SCREEN_MULTIPLIER(BREATHICON_SIZE_X),
			SCREEN_MULTIPLIER(BREATHICON_SIZE_Y),
			CRGBA(BREATH_R, BREATH_G, BREATH_B, 255));
		RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
	}

	static void PrintHealthBar(float posX, float posY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine,
		BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor)
	{
		int SquareBar = 1;
		if (m_ItemToFlash != 3 || ShowFlashingItem(300, 300)) {
			if (CWorld::Players[0].m_nLastTimeArmourLost == CWorld::Players[0].m_nLastTimeEnergyLost || CWorld::Players[0].m_nLastTimeEnergyLost + 1000 < CTimer::m_snTimeInMilliseconds || ShowFlashingItem(150, 150))
			{
				RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
				switch (HEALTH_TYPE) {
				case 0: default: // Default Bar
					CSprite2d::DrawBarChart(
						RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HEALTHBAR_X),
						SCREEN_COORD(HUD_POS_Y + HEALTHBAR_Y),
						(unsigned short)SCREEN_MULTIPLIER(HEALTHBAR_SIZE_X),
						(unsigned char)SCREEN_MULTIPLIER(HEALTHBAR_SIZE_Y),
						(unsigned char)fPercentage, 0, 0, HEALTHBAR_BORDER, CRGBA(HEALTH_R, HEALTH_G, HEALTH_B, HEALTH_A), CRGBA(0, 0, 0, HEALTH_A));
					break;
				case 1: // Square Bar
					CSprite2d::DrawBarChart(
						RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HEALTHBAR_X),
						SCREEN_COORD(HUD_POS_Y + HEALTHBAR_Y),
						(unsigned short)SCREEN_MULTIPLIER(HEALTHBAR_SIZE_X),
						(unsigned char)SCREEN_MULTIPLIER(HEALTHBAR_SIZE_Y),
						(unsigned char)fPercentage, 0, 0, HEALTHBAR_BORDER, CRGBA(HEALTH_R, HEALTH_G, HEALTH_B, HEALTH_A), CRGBA(0, 0, 0, HEALTH_A));
					hudIcons[5].Draw(
						RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HEALTHBAR_X),
						SCREEN_COORD(HUD_POS_Y + HEALTHBAR_Y),
						SCREEN_MULTIPLIER(HEALTHBAR_SIZE_X),
						SCREEN_MULTIPLIER(HEALTHBAR_SIZE_Y),
						CRGBA(0, 0, 0, 255));
					break;
				case 2: // Rounded Bar
					CSprite2d::DrawBarChart(
						RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HEALTHBAR_X + -5.0f),
						SCREEN_COORD(HUD_POS_Y + HEALTHBAR_Y + 6.0f),
						(unsigned short)SCREEN_MULTIPLIER(HEALTHBAR_SIZE_X + -9.0f),
						(unsigned char)SCREEN_MULTIPLIER(HEALTHBAR_SIZE_Y + -11.0f),
						(unsigned char)fPercentage, 0, 0, 0, CRGBA(HEALTH_R, HEALTH_G, HEALTH_B, HEALTH_A),
						CRGBA(0, 0, 0, HEALTH_A));
					hudIcons[6].Draw(
						RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HEALTHBAR_X),
						SCREEN_COORD(HUD_POS_Y + HEALTHBAR_Y),
						SCREEN_MULTIPLIER(HEALTHBAR_SIZE_X),
						SCREEN_MULTIPLIER(HEALTHBAR_SIZE_Y),
						CRGBA(0, 0, 0, 255));
					break;
				}
				RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
			}
		}
	}

	static void PrintArmourBar(float posX, float posY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine,
		BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor)
	{
		int SquareBar = 1;
		if (m_ItemToFlash != 4 || ShowFlashingItem(300, 300)) {
			if (CWorld::Players[0].m_nLastTimeArmourLost == 0 || CWorld::Players[0].m_nLastTimeArmourLost + 1000 < CTimer::m_snTimeInMilliseconds || ShowFlashingItem(150, 150))
			{
				RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
				switch (ARMOUR_TYPE) {
				default: case 0: // Default Bar
					CSprite2d::DrawBarChart(
						RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + ARMOURBAR_X),
						SCREEN_COORD(HUD_POS_Y + ARMOURBAR_Y),
						(unsigned short)SCREEN_MULTIPLIER(ARMOURBAR_SIZE_X),
						(unsigned char)SCREEN_MULTIPLIER(ARMOURBAR_SIZE_Y),
						(unsigned char)fPercentage, 0, 0, ARMOURBAR_BORDER, CRGBA(ARMOUR_R, ARMOUR_G, ARMOUR_B, ARMOUR_A),
						CRGBA(0, 0, 0, ARMOUR_A));
					break;
				case 1: // Square Bar
					CSprite2d::DrawBarChart(
						RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + ARMOURBAR_X),
						SCREEN_COORD(HUD_POS_Y + ARMOURBAR_Y),
						(unsigned short)SCREEN_MULTIPLIER(ARMOURBAR_SIZE_X),
						(unsigned char)SCREEN_MULTIPLIER(ARMOURBAR_SIZE_Y),
						(unsigned char)fPercentage, 0, 0, ARMOURBAR_BORDER, CRGBA(ARMOUR_R, ARMOUR_G, ARMOUR_B, ARMOUR_A),
						CRGBA(0, 0, 0, ARMOUR_A));
					hudIcons[5].Draw(
					RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + ARMOURBAR_X),
					SCREEN_COORD(HUD_POS_Y + ARMOURBAR_Y),
					SCREEN_MULTIPLIER(ARMOURBAR_SIZE_X),
					SCREEN_MULTIPLIER(ARMOURBAR_SIZE_Y),
					CRGBA(0, 0, 0, 255));
					break;
				case 2: // Rounded Bar
					CSprite2d::DrawBarChart(
						RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + ARMOURBAR_X + -5.0f),
						SCREEN_COORD(HUD_POS_Y + ARMOURBAR_Y + 6.0f),
						(unsigned short)SCREEN_MULTIPLIER(ARMOURBAR_SIZE_X + -9.0f),
						(unsigned char)SCREEN_MULTIPLIER(ARMOURBAR_SIZE_Y + -11.0f),
						(unsigned char)fPercentage, 0, 0, 0, CRGBA(ARMOUR_R, ARMOUR_G, ARMOUR_B, ARMOUR_A),
						CRGBA(0, 0, 0, ARMOUR_A));
					hudIcons[6].Draw(
						RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + ARMOURBAR_X),
						SCREEN_COORD(HUD_POS_Y + ARMOURBAR_Y),
						SCREEN_MULTIPLIER(ARMOURBAR_SIZE_X),
						SCREEN_MULTIPLIER(ARMOURBAR_SIZE_Y),
						CRGBA(0, 0, 0, 255));
					break;
				}
				RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
			}
		}
	}

	static void PrintBreathBar(float posX, float posY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine,
		BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor)
	{
		CPed *playa = FindPlayerPed(0);
		RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
		switch (BREATH_TYPE) {
		default: case 0: // Default Bar
			CSprite2d::DrawBarChart(
				RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + BREATHBAR_X),
				SCREEN_COORD(HUD_POS_Y + BREATHBAR_Y),
				(unsigned short)SCREEN_MULTIPLIER(BREATHBAR_SIZE_X),
				(unsigned char)SCREEN_MULTIPLIER(BREATHBAR_SIZE_Y),
				(unsigned char)fPercentage, 0, 0, BREATHBAR_BORDER, CRGBA(BREATH_R, BREATH_G, BREATH_B, BREATH_A),
				CRGBA(0, 0, 0, BREATH_A));
			break;
		case 1: // Square Bar
			CSprite2d::DrawBarChart(
				RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + BREATHBAR_X),
				SCREEN_COORD(HUD_POS_Y + BREATHBAR_Y),
				(unsigned short)SCREEN_MULTIPLIER(BREATHBAR_SIZE_X),
				(unsigned char)SCREEN_MULTIPLIER(BREATHBAR_SIZE_Y),
				(unsigned char)fPercentage, 0, 0, BREATHBAR_BORDER, CRGBA(BREATH_R, BREATH_G, BREATH_B, BREATH_A),
				CRGBA(0, 0, 0, BREATH_A));
			hudIcons[5].Draw(
				RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + BREATHBAR_X),
				SCREEN_COORD(HUD_POS_Y + BREATHBAR_Y),
				SCREEN_MULTIPLIER(BREATHBAR_SIZE_X),
				SCREEN_MULTIPLIER(BREATHBAR_SIZE_Y), CRGBA(0, 0, 0, 255));
			break;
		case 2: // Rounded Bar
			CSprite2d::DrawBarChart(
				RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + BREATHBAR_X + -5.0f),
				SCREEN_COORD(HUD_POS_Y + BREATHBAR_Y + 6.0f),
				(unsigned short)SCREEN_MULTIPLIER(BREATHBAR_SIZE_X + -9.0f),
				(unsigned char)SCREEN_MULTIPLIER(BREATHBAR_SIZE_Y + -11.0f),
				(unsigned char)fPercentage, 0, 0, 0, CRGBA(BREATH_R, BREATH_G, BREATH_B, BREATH_A),
				CRGBA(0, 0, 0, BREATH_A));
			hudIcons[6].Draw(
				RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + BREATHBAR_X),
				SCREEN_COORD(HUD_POS_Y + BREATHBAR_Y),
				SCREEN_MULTIPLIER(BREATHBAR_SIZE_X), SCREEN_MULTIPLIER(BREATHBAR_SIZE_Y), CRGBA(0, 0, 0, 255));
			break;
		}
		RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
	}

	static inline bool ShowFlashingItem(signed int nOnDuration, signed int nOffDuration)
	{
		return CTimer::m_snTimeInMillisecondsPauseMode % (nOnDuration + nOffDuration) < nOnDuration;
	}

	static void DrawHealth(float posX, float posY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine,
		BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor)
	{
		switch (HEALTH_TYPE)
		{
		default: case 0: case 1: case 2:
			PrintHealthBar(posX, posY, wWidth, wHeight, fPercentage, drawBlueLine,
				drawPercentage, drawBorder, dwColor, dwForeColor);
			break;
		case 3:
			PrintHealthString(posX, posY, wWidth, wHeight, fPercentage, drawBlueLine,
				drawPercentage, drawBorder, dwColor, dwForeColor);
			break;
		}
	}
	
	static void DrawArmour(float posX, float posY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine,
		BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor)
	{
		switch (ARMOUR_TYPE) {
		default: case 0: case 1: case 2:
			PrintArmourBar(posX, posY, wWidth, wHeight, fPercentage, drawBlueLine,
				drawPercentage, drawBorder, dwColor, dwForeColor);
			break;
		case 3:
			PrintArmourString(posX, posY, wWidth, wHeight, fPercentage, drawBlueLine,
				drawPercentage, drawBorder, dwColor, dwForeColor);
			break;
		}
	}

	static void DrawBreath(float posX, float posY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine,
		BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor)
	{
		switch (BREATH_TYPE) {
		default: case 0: case 1: case 2:
			PrintBreathBar(posX, posY, wWidth, wHeight, fPercentage, drawBlueLine,
				drawPercentage, drawBorder, dwColor, dwForeColor);
			break;
		case 3:
			PrintBreathString(posX, posY, wWidth, wHeight, fPercentage, drawBlueLine,
				drawPercentage, drawBorder, dwColor, dwForeColor);
			break;
		}
	}

	static void DrawSubtitlesUnused(float x, float y, char *str)
	{
		CFont::SetBackground(0, 0);
		CFont::SetColor(CRGBA(255, 255, 255, 255));
		CFont::SetFontStyle(FONT_SUBTITLES);
		CFont::SetDropShadowPosition(SUBTITLES_SHADOW);
		if (SUBTITLES_OUTLINE != 0)
			CFont::SetOutlinePosition(SUBTITLES_OUTLINE);
		CFont::SetScale(SCREEN_MULTIPLIER(SUBTITLES_SIZE_X), SCREEN_MULTIPLIER(SUBTITLES_SIZE_Y));
		CFont::PrintString(SCREEN_COORD_CENTER_LEFT(SUBTITLES_X), SCREEN_COORD_BOTTOM(SUBTITLES_Y), str);
	}

	static void DrawSubtitles(float x, float y, char *str)
	{
		float subtitles_x[5] = { 20.0f, 151.0f, 22.0f, 0.0f, 0.0f },
			subtitles_y[5] = { 105.0f, 62.0f, 105.0f, 0.0f, 0.0f },
			subtitles_size_x[5] = { 0.58f, 0.472f, 0.621f, 0.0f, 0.0f },
			subtitles_size_y[5] = { 1.22f, 1.08f, 1.22f, 0.0f, 0.0f },
			subtitles_linewidth[5] = { 20.0f, -45.0f, 20.0f, 0.0f, 0.0f },
			subtitles_vitalstats_x = 0.0f,
			subtitles_vitalstats_y[5] = { 105.0f, 62.0f, 105.0f, 0.0f, 0.0f },
			subtitles_vitalstats_size_x[5] = { 0.58f, 0.472f, 0.621f, 0.0f, 0.0f },
			subtitles_vitalstats_size_y[5] = { 1.22f, 1.08f, 1.22f, 0.0f, 0.0f },
			subtitles_vitalstats_linewidth[5] = { 20.0f, -45.0f, 20.0f, 0.0f, 0.0f },
			subtitles_widescreen_x = 0.0f,
			subtitles_widescreen_y = 0.0f,
			subtitles_widescreen_size_x = 0.0f,
			subtitles_widescreen_size_y = 0.0f,
			subtitles_widescreen_linewidth = 0.0f;
		int shadow[5] = { 2, 1, 0, 0, 0 },
			outline[5] = { 0, 0, 0, 0, 0 };
		int subt_r = 225, subt_g = 225, subt_b = 225;

		CFont::SetAlignment(ALIGN_CENTER);
		CFont::SetFontStyle(FONT_SUBTITLES);
		CFont::SetColor(CRGBA(SUBTITLES_R, SUBTITLES_G, SUBTITLES_B, 255));
		CFont::SetDropShadowPosition(SUBTITLES_SHADOW);
		if (SUBTITLES_OUTLINE != 0)
			CFont::SetOutlinePosition(SUBTITLES_OUTLINE);
		if (!TheCamera.m_bWideScreenOn)
		{
			CFont::SetScaleForCurrentlanguage(
				SCREEN_MULTIPLIER(SUBTITLES_SIZE_X),
				SCREEN_MULTIPLIER(SUBTITLES_SIZE_Y));
			if (CHud::bDrawingVitalStats)
			{
				CFont::SetScaleForCurrentlanguage(
					SCREEN_MULTIPLIER(SUBTITLES_SIZE_X * SUBTITLES_VITALSTATS_SIZE_X),
					SCREEN_MULTIPLIER(SUBTITLES_SIZE_Y));
				CFont::SetCentreSize((RsGlobal.maximumWidth
					- 0.0015625f * RsGlobal.maximumWidth * SUBTITLES_LINEWIDTH
					- 8.0f * (0.0015625f * RsGlobal.maximumWidth)
					- (0.0015625f * RsGlobal.maximumWidth * 140.0f
						+ 8.0f * (0.0015625f * RsGlobal.maximumWidth))) * SUBTITLES_VITALSTATS_SIZE_X);
				y = SCREEN_COORD_BOTTOM(SUBTITLES_Y);
				x = SCREEN_COORD_CENTER_LEFT(SUBTITLES_X + SUBTITLES_VITALSTATS_X);
			}
			else
			{
				CFont::SetCentreSize(RsGlobal.maximumWidth
					- 0.0015625f * RsGlobal.maximumWidth * SUBTITLES_LINEWIDTH
					- 8.0f * (0.0015625f * RsGlobal.maximumWidth)
					- (0.0015625f * RsGlobal.maximumWidth * 140.0f
						+ 8.0f * (0.0015625f * RsGlobal.maximumWidth)));
				y = SCREEN_COORD_BOTTOM(SUBTITLES_Y);
				x = SCREEN_COORD_CENTER_LEFT(SUBTITLES_X);
			}
		}
		CFont::PrintString(x, y, CHud::m_Message);
	}

	static float GetHelpBoxXShift() {
		// if radar drawn
		if ((*(unsigned __int8 *)0xBA676C) != 2)
		{
			if (FindPlayerVehicle(-1, 0) && FindPlayerVehicle(-1, 0)->m_nModelIndex != 0x21B &&
				((*(unsigned __int32 *)((unsigned int)FindPlayerVehicle(-1, 0) + 0x594)) == 4 ||
				(*(unsigned __int32 *)((unsigned int)FindPlayerVehicle(-1, 0) + 0x594)) == 3))
				return fTextBoxPosnXWithRadarAndPlane;
			else if (FindPlayerPed(-1) && FindPlayerPed(-1)->m_aWeapons[FindPlayerPed(-1)->m_nActiveWeaponSlot].m_nType == WEAPON_PARACHUTE)
				return fTextBoxPosnXWithRadarAndPlane;
			else return fTextBoxPosnXWithRadar;
		}
		else return fTextBoxPosnX;
	}

	static void DrawHelpText() {
		char baseY;
		float alpha;
		float posX;
		float progress;
		if (g_pHelpMessage[0])
		{
			if (!CMessages::StringCompare(g_pHelpMessage, CHud::m_pLastHelpMessage, 400))
			{
				switch (g_HelpMessageState)
				{
				case 0: // STATE_FREE_TO_USE
					g_HelpMessageState = 2; // STATE_FADE_OUT
					g_HelpMessageTimer = 0;
					g_HelpMessageFadeTimer = 0;
					CMessages::StringCopy(g_pHelpMessageToPrint, g_pHelpMessage, 400);
					CFont::SetAlignment(ALIGN_LEFT);
					CFont::SetJustify(false);
					CFont::SetWrapx(SCREEN_MULTIPLIER(28 + 350 - 16));
					CFont::SetFontStyle(FONT_SUBTITLES);
					CFont::SetBackground(true, true);
					CFont::SetDropShadowPosition(0);
					g_fTextBoxNumLines = (float)CFont::GetNumberLinesNoPrint(SCREEN_COORD(34.0f), SCREEN_COORD(28.0f), g_pHelpMessageToPrint) + 3.0f;
					CFont::SetWrapx((float)RsGlobal.maximumWidth);
					PLAYONESHOT(32, 0.0);
					break;
				case 1:
				case 2:
				case 3:
				case 4:
					g_HelpMessageState = 4; // STATE_REPLACE
					g_HelpMessageTimer = 5;
					break;
				default:
					break;
				}
				CMessages::StringCopy(CHud::m_pLastHelpMessage, g_pHelpMessage, 400);
			}
			alpha = 170.0;
			if (g_HelpMessageState)
			{
				switch (g_HelpMessageState)
				{
				case 2: // FADE_OUT
					if (!Camera_WidescreenOn)
					{
						g_HelpMessageFadeTimer += 2 * (int)(CTimer::ms_fTimeStep * 0.02f * 1000.0f);
						if (g_HelpMessageFadeTimer > 0)
						{
							g_HelpMessageFadeTimer = 0;
							g_HelpMessageState = 1; // STATE_IDLE
						}
						alpha = g_HelpMessageFadeTimer * 0.001f * 200.0f;
					}
					break;
				case 3: // FADE_IN
					g_HelpMessageFadeTimer -= 2 * (int)(CTimer::ms_fTimeStep * 0.02f * 1000.0f);
					if (g_HelpMessageFadeTimer < 0 || Camera_WidescreenOn)
					{
						g_HelpMessageFadeTimer = 0;
						g_HelpMessageState = 0; // STATE_FREE_TO_USE
					}
					alpha = g_HelpMessageFadeTimer * 0.001f * 200.0f;
					break;
				case 4: // REPLACE
					g_HelpMessageFadeTimer -= 2 * (int)(CTimer::ms_fTimeStep * 0.02f * 1000.0f);
					if (g_HelpMessageFadeTimer < 0)
					{
						g_HelpMessageFadeTimer = 0;
						g_HelpMessageState = 2; // FADE_OUT
						CMessages::StringCopy(g_pHelpMessageToPrint, CHud::m_pLastHelpMessage, 400);
					}
					alpha = g_HelpMessageFadeTimer * 0.001f * 200.0f;
					break;
				case 1: // IDLE
					alpha = 200.0;
					g_HelpMessageFadeTimer = 600;
					if (!bTextBoxPermanent2)
					{
						if (g_HelpMessageTimer > g_fTextBoxNumLines * 1000.0 || m_HelpMessageQuick && g_HelpMessageTimer > 3000)
						{
							g_HelpMessageState = 3; // FADE_IN
							g_HelpMessageFadeTimer = 600;
						}
					}
					break;
				default:
					break;
				}
				if (!Cutscene_Running)
				{
					g_HelpMessageTimer += (int)(CTimer::ms_fTimeStep * 0.02f * 1000.0f);
					CFont::SetAlphaFade(alpha);
					CFont::SetProp(true);
					CFont::SetScale(SCREEN_MULTIPLIER(fTextBoxFontScaleX), SCREEN_MULTIPLIER(fTextBoxFontScaleY));
					if (g_HelpMessageStatId)
					{
						if (Camera_WidescreenOn)
						{
							CFont::SetAlphaFade(255.0);
							return;
						}
						if (g_HelpMessageStatId >= 10)
						{
							if (g_HelpMessageStatId >= 100)
								sprintf(g_filenameBuffer, "STAT%d", g_HelpMessageStatId);
							else
								sprintf(g_filenameBuffer, "STAT0%d", g_HelpMessageStatId);
						}
						else
							sprintf(g_filenameBuffer, "STAT00%d", g_HelpMessageStatId);
						CFont::SetAlignment(ALIGN_LEFT);
						CFont::SetJustify(false);
						CFont::SetWrapx((float)RsGlobal.maximumWidth);
						CFont::SetFontStyle(FONT_SUBTITLES);
						CFont::SetBackground(true, true);
						CFont::SetDropShadowPosition(0);
						CFont::SetBackgroundColor(CRGBA(0, 0, 0, (unsigned char)alpha));
						CFont::SetColor(CRGBA(255, 255, 255, 255));
						posX = CFont::GetStringWidth((char *)TheText.Get(g_filenameBuffer), 1, 0) + SCREEN_MULTIPLIER(GetHelpBoxXShift() + fTextBoxBorderSize) + 4.0f;
						CFont::SetWrapx(SCREEN_MULTIPLIER(164.0f) + posX + 4.0f);
						CFont::PrintString(
							SCREEN_COORD(GetHelpBoxXShift() + fTextBoxBorderSize) + 4.0f,
							SCREEN_COORD((150.0f - flt_8D0938) * 0.6f) + SCREEN_COORD(fTextBoxPosnY),
							(char *)TheText.Get(g_filenameBuffer));
						if (g_HelpMessageStatId == 336)
							progress = (float)GetGroupMembersCount(FindPlayerPed(-1)->m_pPlayerData->m_nPlayerGroup);
						else
							progress = GetPlayerStat(g_HelpMessageStatId);
						if (g_pHelpMessageToPrint[0] == '+')
						{
							CSprite2d::DrawBarChart(
								posX + SCREEN_COORD(15.0),
								SCREEN_COORD((157.0f - flt_8D0938) * 0.6f) + SCREEN_COORD(fTextBoxPosnY + 4.0f),
								(unsigned short)SCREEN_MULTIPLIER(130.0f),
								(unsigned char)SCREEN_MULTIPLIER(20.0f),
								fmax(1.0f / StatMax * progress * 100.0f, 2.0f),
								(unsigned char)fmax((1.0f / StatMax) * dword_BAA468 * 100.0f, 3.0f),
								0, 0, CRGBA(255, 255, 255, 255), CRGBA(255, 255, 255, 255));
							CFont::SetColor(CRGBA(255, 255, 255, 255));
							CFont::PrintString(
								posX + SCREEN_COORD(155.0),
								SCREEN_COORD((153.0f - flt_8D0938) * 0.6f) + SCREEN_COORD(fTextBoxPosnY),
								g_pHelpMessageToPrint);
						}
						else
						{
							CSprite2d::DrawBarChart(
								posX + SCREEN_COORD(15.0),
								SCREEN_COORD((157.0f - flt_8D0938) * 0.6f) + SCREEN_COORD(fTextBoxPosnY + 4.0f),
								(unsigned short)SCREEN_MULTIPLIER(130.0f),
								(unsigned char)SCREEN_MULTIPLIER(20.0f),
								fmax(1.0f / StatMax * progress * 100.0f, 2.0f),
								(unsigned char)fmax((1.0f / StatMax) * dword_BAA468 * 100.0f, 3.0f),
								0, 0, CRGBA(255, 255, 255, 255), CRGBA(255, 255, 255, 255));
							CFont::SetColor(CRGBA(255, 255, 255, 255));
							CFont::PrintString(
								posX + SCREEN_COORD(155.0),
								SCREEN_COORD((153.0f - flt_8D0938) * 0.6f) + SCREEN_COORD(fTextBoxPosnY),
								g_pHelpMessageToPrint);
						}
					}
					else
					{
						if (g_BigMessage0[0] || g_BigMessage4[0] || byte_96C014[0])
						{
							CFont::SetAlphaFade(255.0);
							return;
						}
						CFont::SetAlignment(ALIGN_LEFT);
						CFont::SetJustify(false);
						if (g_fScriptTextBoxesWidth == 200.0)
						{
							CFont::SetWrapx(SCREEN_MULTIPLIER(GetHelpBoxXShift() + fTextBoxWidth - fTextBoxBorderSize) - 4.0f);
						}
						else
							CFont::SetWrapx(SCREEN_MULTIPLIER(GetHelpBoxXShift() + (g_fScriptTextBoxesWidth - fTextBoxBorderSize)) - 4.0f);
						CFont::SetFontStyle(FONT_SUBTITLES);
						CFont::SetBackground(true, true);
						CFont::SetDropShadowPosition(0);
						CFont::SetBackgroundColor(CRGBA(0, 0, 0, (unsigned char)alpha));
						CFont::SetColor(CRGBA(255, 255, 255, 255));
						baseY = 0;
						if (Camera_WidescreenOn && !Menu_WidescreenOn)
							baseY = 56;
						CFont::PrintString(
							SCREEN_COORD(GetHelpBoxXShift() + fTextBoxBorderSize) + 4.0f,
							SCREEN_COORD(((float)baseY + 150.0f - flt_8D0938) * 0.6f) + SCREEN_COORD(fTextBoxPosnY),
							g_pHelpMessageToPrint);
					}
					CFont::SetWrapx((float)RsGlobal.maximumWidth);
					CFont::SetAlphaFade(255.0);
					return;
				}
			}
		}
		else
			g_HelpMessageState = 0;
	}

	static CRGBA * __fastcall SetRadarDisc(CRGBA *rgba, int, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		rgba->red = 255;
		rgba->green = 255;
		rgba->blue = 255;
		rgba->alpha = 255;
		return rgba;
	}

	static void SetRadarPosAndSize()
	{
		static float radar[5][4] = {
			{ 40.0f, 104.0f, 94.0f, 76.0f }, // SA
			{ 27.0f, 121.0f, 90.0f, 72.0f }, // III
			{ 22.0f, 114.0f, 90.0f, 72.0f }, // VC
			{ 40.0f, 104.0f, 94.0f, 76.0f }, // LCS
			{ 0.0f, 0.0f, 0.0f, 0.0f }, // VCS
		};
		static float pos_x, pos_y, size_x, size_y;
		//pos_x = radar[style][0]; pos_y = radar[style][1];
		//size_x = radar[style][2]; size_y = radar[style][3];
		pos_x = RADAR_X, pos_y = RADAR_Y, size_x = RADAR_SIZE_X, size_y = RADAR_SIZE_Y;

		// Radar Pos X
		patch::Set<const void*>(0x5834D4, &pos_x);
		patch::Set<const void*>(0x58A469, &pos_x);
		patch::Set<const void*>(0x58A5E2, &pos_x);
		patch::Set<const void*>(0x58A6E6, &pos_x);
		patch::Set<const void*>(0x58A79B, &pos_x);
		patch::Set<const void*>(0x58A836, &pos_x);
		patch::Set<const void*>(0x58A8E9, &pos_x);
		patch::Set<const void*>(0x58A98A, &pos_x);
		// Radar Pos Y
		patch::Set<const void*>(0x583500, &pos_y);
		patch::Set<const void*>(0x58A1A7, &pos_y);
		patch::Set<const void*>(0x58A2D6, &pos_y);
		patch::Set<const void*>(0x58A499, &pos_y);
		patch::Set<const void*>(0x58A60E, &pos_y);
		patch::Set<const void*>(0x58A71E, &pos_y);
		patch::Set<const void*>(0x58A7C7, &pos_y);
		patch::Set<const void*>(0x58A868, &pos_y);
		patch::Set<const void*>(0x58A913, &pos_y);
		patch::Set<const void*>(0x58A9C7, &pos_y);
		// Radar Size X
		patch::Set<const void*>(0x5834C2, &size_x);
		patch::Set<const void*>(0x58A449, &size_x);
		patch::Set<const void*>(0x58A7E9, &size_x);
		patch::Set<const void*>(0x58A840, &size_x);
		patch::Set<const void*>(0x58A943, &size_x);
		patch::Set<const void*>(0x58A99D, &size_x);
		// Radar Size Y
		patch::Set<const void*>(0x5834F6, &size_y);
		patch::Set<const void*>(0x58A47D, &size_y);
		patch::Set<const void*>(0x58A632, &size_y);
		patch::Set<const void*>(0x58A6AB, &size_y);
		patch::Set<const void*>(0x58A70E, &size_y);
		patch::Set<const void*>(0x58A801, &size_y);
		patch::Set<const void*>(0x58A8AB, &size_y);
		patch::Set<const void*>(0x58A921, &size_y);
		patch::Set<const void*>(0x58A9D5, &size_y);
	}

	static void DrawRadarRectangle(CSprite2d *sprite, CRect const& rect, CRGBA const& color) {
		if (sprite) {
			sprite->Draw(CRect(rect.left, SCREEN_HEIGHT - rect.top, rect.right, SCREEN_HEIGHT - rect.bottom), color);
		}
		else {
			CSprite2d::DrawRect(CRect(rect.left, SCREEN_HEIGHT - rect.top, rect.right, SCREEN_HEIGHT - rect.bottom), color);
		}
	}

	static void __fastcall MyDrawRadarCircle(CSprite2d *sprite, int, CRect const &rect, CRGBA const &color)
	{
		CPed *player = FindPlayerPed(-1);
		CPad *pad = CPad::GetPad(0);
		DrawRadarRectangle(sprite, CRect(SCREEN_COORD(RADAR_X - RADAR_SIZE_X), SCREEN_COORD(RADAR_Y - RADAR_SIZE_Y),
			SCREEN_COORD(RADAR_X), SCREEN_COORD(RADAR_Y)), CRGBA(255, 255, 255, 255));
		DrawRadarRectangle(sprite, CRect(SCREEN_COORD(RADAR_X + RADAR_SIZE_X), SCREEN_COORD(RADAR_Y - RADAR_SIZE_Y),
			SCREEN_COORD(RADAR_X), SCREEN_COORD(RADAR_Y)), CRGBA(255, 255, 255, 255));
		DrawRadarRectangle(sprite, CRect(SCREEN_COORD(RADAR_X - RADAR_SIZE_X), SCREEN_COORD(RADAR_Y + RADAR_SIZE_Y),
			SCREEN_COORD(RADAR_X), SCREEN_COORD(RADAR_Y)), CRGBA(255, 255, 255, 255));
		DrawRadarRectangle(sprite, CRect(SCREEN_COORD(RADAR_X + RADAR_SIZE_X), SCREEN_COORD(RADAR_Y + RADAR_SIZE_Y),
			SCREEN_COORD(RADAR_X), SCREEN_COORD(RADAR_Y)), CRGBA(255, 255, 255, 255));
	}

	static void MyTransformRadarPointToScreenSpace(CVector2D *out, CVector2D *in) {
		CPed *player = FindPlayerPed(-1);
		CPad *pad = CPad::GetPad(0);
		__asm push edx

		if (FrontEndMenuManager.drawRadarOrMap) {
			out->x = FrontEndMenuManager.m_fMapZoom * in->x + FrontEndMenuManager.m_fMapBaseX;
			out->y = FrontEndMenuManager.m_fMapBaseY - FrontEndMenuManager.m_fMapZoom * in->y;
		}
		else {
			if (!pad->GetDisplayVitalStats(player) || FindPlayerVehicle(-1, 0))
				out->x = SCREEN_COORD(RADAR_X) + in->x * SCREEN_COORD(RADAR_SIZE_X)*0.98f;
			else
				out->x = SCREEN_COORD(RADAR_X + 270.0f) + in->x * SCREEN_COORD(RADAR_SIZE_X)*0.98f;
			out->y = SCREEN_COORD_MAX_Y - SCREEN_COORD(RADAR_Y) - in->y * SCREEN_COORD(RADAR_SIZE_Y)*0.98f;
		}

		__asm pop edx
	}

	/*static void __fastcall MyDrawRadarPlane(CSprite2d *sprite, int, float x1, float y1, float x2, float y2, float x3,
		float y3, float x4, float y4, CRGBA const &color)
	{
		CVector2D posn[4];
		posn[0].x = SCREEN_COORD(RADAR_X) - SCREEN_COORD(RADAR_SIZE_X);
		posn[0].y = SCREEN_COORD(RADAR_Y) + SCREEN_COORD(RADAR_SIZE_Y);
		posn[1].x = SCREEN_COORD(RADAR_X) + SCREEN_COORD(RADAR_SIZE_X);
		posn[1].y = SCREEN_COORD(RADAR_Y) + SCREEN_COORD(RADAR_SIZE_Y);
		posn[2].x = SCREEN_COORD(RADAR_X) - SCREEN_COORD(RADAR_SIZE_X);
		posn[2].y = SCREEN_COORD(RADAR_Y) - SCREEN_COORD(RADAR_SIZE_Y);
		posn[3].x = SCREEN_COORD(RADAR_X) + SCREEN_COORD(RADAR_SIZE_X);
		posn[3].y = SCREEN_COORD(RADAR_Y) - SCREEN_COORD(RADAR_SIZE_Y);
		CMatrix *planeMat = FindPlayerVehicle(-1, false)->m_matrix;
		float angle = atan2f(planeMat->at.z, -planeMat->right.z) - 1.57079632679f;
		RotateVertices(posn, 4, SCREEN_COORD(RADAR_X), SCREEN_COORD(RADAR_Y), -angle);
		sprite->Draw(posn[2].x, SCREEN_COORD_MAX_Y - posn[2].y - SCREEN_COORD(1.0f),
			posn[3].x, SCREEN_COORD_MAX_Y - posn[3].y - SCREEN_COORD(1.0f),
			posn[0].x, SCREEN_COORD_MAX_Y - posn[0].y - SCREEN_COORD(1.0f),
			posn[1].x, SCREEN_COORD_MAX_Y - posn[1].y - SCREEN_COORD(1.0f),
			CRGBA(255, 255, 255, 255));
	}

	static void MyDrawPlaneHeightBorder(CRect const& rect, CRGBA const& color) {
		float settings_vecPlaneRadarPosn_x = 0.0f, settings_vecPlaneRadarPosn_y = 67.0f;
		float settings_vecPlaneRadarSize_x = 14.0f, settings_vecPlaneRadarSize_y = 145.0f;
		float settings_vecPlaneRadarLightPartPosn_x = 225.0f, settings_vecPlaneRadarLightPartPosn_y = 70.0f;
		float settings_vecPlaneRadarLightPartSize_x = 8.0f, settings_vecPlaneRadarLightPartSize_y = 139.0f;
		CPed *player = FindPlayerPed();
		CPad *pad = CPad::GetPad(0);
		if (!pad->GetDisplayVitalStats(player) || FindPlayerVehicle(-1, 0)) {
			DrawRadarRectangle(NULL, CRect(BilinearOffset(SCREEN_COORD(settings_vecPlaneRadarPosn_x)), BilinearOffset(SCREEN_COORD(settings_vecPlaneRadarPosn_y)),
				BilinearOffset(SCREEN_COORD(settings_vecPlaneRadarPosn_x) + SCREEN_COORD(settings_vecPlaneRadarSize_x)), BilinearOffset(SCREEN_COORD(settings_vecPlaneRadarPosn_y)
					+ SCREEN_COORD(settings_vecPlaneRadarSize_y))), CRGBA(0, 0, 0, 255));
			DrawRadarRectangle(NULL, CRect(BilinearOffset(SCREEN_COORD(settings_vecPlaneRadarLightPartPosn_x)),
				BilinearOffset(SCREEN_COORD(settings_vecPlaneRadarLightPartPosn_y)),
				BilinearOffset(SCREEN_COORD(settings_vecPlaneRadarLightPartPosn_x) + SCREEN_COORD(settings_vecPlaneRadarLightPartSize_x)),
				BilinearOffset(SCREEN_COORD(settings_vecPlaneRadarLightPartPosn_y) + SCREEN_COORD(settings_vecPlaneRadarLightPartSize_y))), CRGBA(120, 120, 120, 255));
		}
		else {
			DrawRadarRectangle(NULL, CRect(BilinearOffset(SCREEN_COORD(settings_vecPlaneRadarPosn_x + 270.0f)), BilinearOffset(SCREEN_COORD(settings_vecPlaneRadarPosn_y)),
				BilinearOffset(SCREEN_COORD(settings_vecPlaneRadarPosn_x + 270.0f) + SCREEN_COORD(settings_vecPlaneRadarSize_x)), BilinearOffset(SCREEN_COORD(settings_vecPlaneRadarPosn_y)
					+ SCREEN_COORD(settings_vecPlaneRadarSize_y))), CRGBA(0, 0, 0, 255));
			DrawRadarRectangle(NULL, CRect(BilinearOffset(SCREEN_COORD(settings_vecPlaneRadarLightPartPosn_x + 270.0f)),
				BilinearOffset(SCREEN_COORD(settings_vecPlaneRadarLightPartPosn_y)),
				BilinearOffset(SCREEN_COORD(settings_vecPlaneRadarLightPartPosn_x + 270.0f) + SCREEN_COORD(settings_vecPlaneRadarLightPartSize_x)),
				BilinearOffset(SCREEN_COORD(settings_vecPlaneRadarLightPartPosn_y) + SCREEN_COORD(settings_vecPlaneRadarLightPartSize_y))), CRGBA(120, 120, 120, 255));
		}
	}

	void MyDrawPlaneHeight(CRect const& rect, CRGBA const& color) {
		float playerHeight = 0.0f;
		float blackLineBaseY = 0.0f;
		CPed *playa = FindPlayerPed();
		CPad *pad = CPad::GetPad(0);
		CVehicle *playaVeh = FindPlayerVehicle(-1, false);
		if (playaVeh)
			playerHeight = playaVeh->GetPosition().z;
		else if (playa)
			playerHeight = playa->GetPosition().z;
		if (playerHeight < 0.0f)
			playerHeight = 0.0f;
		else
			playerHeight /= 900.0f;
		if (playerHeight > 1.0f)
			playerHeight = 1.0f;
		if (playerHeight > 0.0f) {
			if (!pad->GetDisplayVitalStats(playa) || FindPlayerVehicle(-1, 0)) {
				CSprite2d::DrawRect(CRect(BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.x)),
					BilinearOffset(SCREEN_COORD_MAX_Y + 1.0f - SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y) -
						SCREEN_COORD(settings.vecPlaneRadarLightPartSize.y) * playerHeight),
					BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.x) + SCREEN_COORD(settings.vecPlaneRadarLightPartSize.x)),
					BilinearOffset(SCREEN_COORD_MAX_Y + 1.0f - SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y))), CRGBA(255, 255, 255, 255));
			}
			else {
				CSprite2d::DrawRect(CRect(BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.x + 270.0f)),
					BilinearOffset(SCREEN_COORD_MAX_Y + 1.0f - SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y) -
						SCREEN_COORD(settings.vecPlaneRadarLightPartSize.y) * playerHeight),
					BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.x + 270.0f) + SCREEN_COORD(settings.vecPlaneRadarLightPartSize.x)),
					BilinearOffset(SCREEN_COORD_MAX_Y + 1.0f - SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y))), CRGBA(255, 255, 255, 255));
			}
		}
		if (!pad->GetDisplayVitalStats(playa) || FindPlayerVehicle(-1, 0)) {
			blackLineBaseY = playerHeight * (settings.vecPlaneRadarLightPartSize.y) > 4.0f ?
				SCREEN_COORD_MAX_Y + 1.0f - SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y) - SCREEN_COORD(settings.vecPlaneRadarLightPartSize.y) * playerHeight :
				SCREEN_COORD_MAX_Y + 1.0f - SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y) - SCREEN_COORD(4.0f);
			CSprite2d::DrawRect(CRect(BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.x) - SCREEN_COORD(3.0f)),
				BilinearOffset(blackLineBaseY), BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.x) + SCREEN_COORD(settings.vecPlaneRadarSize.x) + SCREEN_COORD(3.0f)),
				BilinearOffset(blackLineBaseY + SCREEN_COORD(4.0f))), CRGBA(0, 0, 0, 255));
		}
		else {
			blackLineBaseY = playerHeight * (settings.vecPlaneRadarLightPartSize.y) > 4.0f ?
				SCREEN_COORD_MAX_Y + 1.0f - SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y) - SCREEN_COORD(settings.vecPlaneRadarLightPartSize.y) * playerHeight :
				SCREEN_COORD_MAX_Y + 1.0f - SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y) - SCREEN_COORD(4.0f);
			CSprite2d::DrawRect(CRect(BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.x + 270.0f) - SCREEN_COORD(3.0f)),
				BilinearOffset(blackLineBaseY), BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.x + 270.0f) + SCREEN_COORD(settings.vecPlaneRadarSize.x) + SCREEN_COORD(3.0f)),
				BilinearOffset(blackLineBaseY + SCREEN_COORD(4.0f))), CRGBA(0, 0, 0, 255));
		}
	}*/

	static void DrawSuccessFailedMessage(float x, float y, char* str)
	{
		float pos_y = SCREEN_COORD(STYLED1_Y),
			pos_x = (float)(RsGlobal.maximumWidth / 2);

		if (CHud::m_BigMessage[3][0] || CHud::m_BigMessage[5][0]) // Styled 4 or 6
			pos_y += STYLED1_Y1;
		else if (!CHud::m_BigMessage[2][0] && CFont::GetNumberLinesNoPrint(pos_x, pos_y, str) > 1) // Styled 3
			pos_y += STYLED1_Y2;
		CFont::SetScale(SCREEN_MULTIPLIER(STYLED1_SIZE_X), SCREEN_MULTIPLIER(STYLED1_SIZE_Y));
		CFont::SetCentreSize(RsGlobal.maximumWidth * 0.0015625f * 590.0f);
		CFont::SetFontStyle(FONT_PRICEDOWN);
		CFont::SetDropShadowPosition(STYLED1_SHADOW);
		if (STYLED1_OUTLINE != 0)
			CFont::SetOutlinePosition(STYLED1_OUTLINE);
		CFont::SetColor(CRGBA(STYLED1_R, STYLED1_G, STYLED1_B, (unsigned char)BigMessageAlpha[0]));
		CFont::PrintString(pos_x, pos_y, str);
	}

	static void DrawMissionTitle(float x, float y, char* str)
	{
		CFont::SetAlignment(ALIGN_RIGHT);
		CFont::SetFontStyle(MT_FONTSTYLE);
		CFont::SetScale(SCREEN_MULTIPLIER(MT_SIZE_X), SCREEN_MULTIPLIER(MT_SIZE_Y));
		CFont::SetDropShadowPosition(MT_SHADOW);
		if (MT_OUTLINE != 0)
			CFont::SetOutlinePosition(MT_OUTLINE);
		CFont::SetColor(CRGBA(MT_R, MT_G, MT_B, (unsigned char)BigMessageAlpha[1]));
		CFont::PrintStringFromBottom(RsGlobal.maximumWidth - SCREEN_COORD(MT_POS_X), SCREEN_COORD(MT_POS_Y), str);
	}

	static void DrawBustedWastedMessage(float x, float y, char *str) {
		CFont::SetAlignment((eFontAlignment)WB_ALIGN);
		CFont::SetDropShadowPosition(WB_SHADOW);
		if (WB_OUTLINE != 0)
			CFont::SetOutlinePosition(WB_OUTLINE);
		CFont::SetFontStyle(WB_FONTSTYLE);
		CFont::SetScale(SCREEN_MULTIPLIER(WB_SIZE_X), SCREEN_MULTIPLIER(WB_SIZE_Y));
		CFont::SetColor(CRGBA(WB_R, WB_G, WB_B, (unsigned char)BigMessageAlpha[2]));
		CFont::PrintStringFromBottom(RsGlobal.maximumWidth - SCREEN_COORD(WB_POS_X), SCREEN_COORD(WB_POS_Y), str);
	}

	void static DrawOddJobMessage4(float x, float y, char* str)
	{
		CFont::SetScaleForCurrentlanguage(SCREEN_MULTIPLIER(STYLED4_SIZE_X), SCREEN_MULTIPLIER(STYLED4_SIZE_Y));
		CFont::SetAlignment((eFontAlignment)STYLED4_ALIGN);
		CFont::SetCentreSize(RsGlobal.maximumWidth * 0.0015625f * 500.0f);
		CFont::SetFontStyle(STYLED4_FONTSTYLE);
		CFont::SetDropShadowPosition(STYLED4_SHADOW);
		if (STYLED4_OUTLINE != 0)
			CFont::SetOutlinePosition(STYLED4_OUTLINE);
		CFont::SetColor(CRGBA(STYLED4_R, STYLED4_G, STYLED4_B, 255));
		CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(STYLED4_X), SCREEN_COORD(STYLED4_Y), str);
	}

	void static DrawOddJobMessage5(float x, float y, char* str)
	{
		CFont::SetScaleForCurrentlanguage(SCREEN_MULTIPLIER(STYLED5_SIZE_X), SCREEN_MULTIPLIER(STYLED5_SIZE_Y));
		CFont::SetAlignment(ALIGN_CENTER);
		CFont::SetCentreSize(RsGlobal.maximumWidth * 0.0015625f * 600.0f);
		CFont::SetFontStyle(STYLED5_FONTSTYLE);
		CFont::SetDropShadowPosition(STYLED5_SHADOW);
		if (STYLED5_OUTLINE != 0)
			CFont::SetOutlinePosition(STYLED5_OUTLINE);
		CFont::SetColor(CRGBA(STYLED5_R, STYLED5_G, STYLED5_B, 255));
		CFont::PrintStringFromBottom((float)(RsGlobal.maximumWidth / 2), SCREEN_COORD(STYLED5_Y), str);
	}

	void static DrawOddJobMessage6(float x, float y, char* str)
	{
		CFont::SetScaleForCurrentlanguage(SCREEN_MULTIPLIER(STYLED6_SIZE_X), SCREEN_MULTIPLIER(STYLED6_SIZE_Y));
		CFont::SetAlignment(ALIGN_CENTER);
		CFont::SetCentreSize(RsGlobal.maximumWidth * 0.0015625f * 500.0f);
		CFont::SetFontStyle(STYLED6_FONTSTYLE);
		CFont::SetDropShadowPosition(STYLED6_SHADOW);
		if (STYLED6_OUTLINE != 0)
			CFont::SetOutlinePosition(STYLED6_OUTLINE);
		CFont::SetColor(CRGBA(STYLED6_R, STYLED6_G, STYLED6_B, 255));
		CFont::PrintString((float)(RsGlobal.maximumWidth / 2), SCREEN_COORD(STYLED6_Y), str);
	}

	void static DrawOddJobMessage7(float x, float y, char* str)
	{
		CFont::SetScaleForCurrentlanguage(SCREEN_MULTIPLIER(STYLED7_SIZE_X), SCREEN_MULTIPLIER(STYLED7_SIZE_Y));
		CFont::SetAlignment(ALIGN_CENTER);
		CFont::SetCentreSize(RsGlobal.maximumWidth * 0.0015625f * 500.0f);
		CFont::SetFontStyle(STYLED7_FONTSTYLE);
		CFont::SetDropShadowPosition(STYLED7_SHADOW);
		if (STYLED7_OUTLINE != 0)
			CFont::SetOutlinePosition(STYLED7_OUTLINE);
		CFont::SetColor(CRGBA(STYLED7_R, STYLED7_G, STYLED7_B, 255));
		CFont::PrintString((float)(RsGlobal.maximumWidth / 2), SCREEN_COORD(STYLED7_Y), str);
	}

	static void DrawMissionTimers_Timer(float x, float y, char* str)
	{
		CFont::SetScale(SCREEN_MULTIPLIER(M_TIMERS_SIZE_X), SCREEN_MULTIPLIER(M_TIMERS_SIZE_Y));
		CFont::SetAlignment(ALIGN_RIGHT);
		CFont::SetFontStyle(M_TIMERS_FONTSTYLE); // default FONT_MENU: 2
		CFont::SetDropShadowPosition(M_TIMERS_SHADOW);
		if (M_TIMERS_OUTLINE != 0)
			CFont::SetOutlinePosition(M_TIMERS_OUTLINE);
		CFont::SetColor(CRGBA(M_TIMERS_R, M_TIMERS_G, M_TIMERS_B, 255));
		CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(M_TIMER_X), SCREEN_COORD(M_TIMER_Y), str);
	}

	static void DrawMissionTimers_TimerText(float x, float y, char* str)
	{
		CFont::SetScale(SCREEN_MULTIPLIER(M_TIMERS_SIZE_X), SCREEN_MULTIPLIER(M_TIMERS_SIZE_Y));
		CFont::SetAlignment(ALIGN_RIGHT);
		CFont::SetFontStyle(M_TIMERS_FONTSTYLE); // default FONT_MENU: 2
		CFont::SetDropShadowPosition(M_TIMERS_SHADOW);
		if (M_TIMERS_OUTLINE != 0)
			CFont::SetOutlinePosition(M_TIMERS_OUTLINE);
		CFont::SetColor(CRGBA(M_TIMERS_R, M_TIMERS_G, M_TIMERS_B, 255));
		CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(M_TIMER_DESC_X), SCREEN_COORD(M_TIMER_Y), str);
	}

	static void DrawMissionTimers_StatusTextChartBar(float x, float y, WORD width, WORD height, float fPercentage,
		BYTE drawBlueLine, BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor)
	{
		float OFFSET_Y = y * M_STATUSTEXT_SPACE;
		CSprite2d::DrawBarChart(
			RsGlobal.maximumWidth - SCREEN_COORD(M_STATUSBAR_X),
			SCREEN_COORD(M_STATUSBAR_Y + OFFSET_Y),
			(unsigned short)SCREEN_MULTIPLIER(M_STATUSBAR_SIZE_X),
			(unsigned char)SCREEN_MULTIPLIER(M_STATUSBAR_SIZE_Y),
			(unsigned char)fPercentage, 0, 0, M_STATUSBAR_BORDER,
			CRGBA(M_STATUSBAR_R, M_STATUSBAR_G, M_STATUSBAR_B, M_STATUSBAR_A),
			CRGBA(0, 0, 0, M_STATUSBAR_A));
	}

	static void DrawMissionTimers_StatusTextValue(float x, float y, char* str)
	{
		float OFFSET_Y = y * M_STATUSTEXT_SPACE;
		CFont::SetScale(SCREEN_MULTIPLIER(M_TIMERS_SIZE_X), SCREEN_MULTIPLIER(M_TIMERS_SIZE_Y));
		CFont::SetAlignment(ALIGN_RIGHT);
		CFont::SetFontStyle(M_TIMERS_FONTSTYLE); // default FONT_MENU: 2
		CFont::SetDropShadowPosition(M_TIMERS_SHADOW);
		if (M_TIMERS_OUTLINE != 0)
			CFont::SetOutlinePosition(M_TIMERS_OUTLINE);
		CFont::SetColor(CRGBA(M_STATUSBAR_R, M_STATUSBAR_G, M_STATUSBAR_B, 255));
		CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(M_STATUSTEXT_X), SCREEN_COORD(M_STATUSTEXT_Y + OFFSET_Y), str);
	}

	static void DrawMissionTimers_StatusTextDescription(float x, float y, char* str)
	{
		float OFFSET_Y = y * M_STATUSTEXT_SPACE;
		CFont::SetScale(SCREEN_MULTIPLIER(M_TIMERS_SIZE_X), SCREEN_MULTIPLIER(M_TIMERS_SIZE_Y));
		CFont::SetAlignment(ALIGN_RIGHT);
		CFont::SetFontStyle(M_TIMERS_FONTSTYLE); // default FONT_MENU: 2
		CFont::SetDropShadowPosition(M_TIMERS_SHADOW);
		if (M_TIMERS_OUTLINE != 0)
			CFont::SetOutlinePosition(M_TIMERS_OUTLINE);
		CFont::SetColor(CRGBA(M_STATUSBAR_R, M_STATUSBAR_G, M_STATUSBAR_B, 255));
		CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(M_STATUSTEXT_DESC_X), SCREEN_COORD(M_STATUSTEXT_Y + OFFSET_Y), str);
	}

	static void DisplayRadioStationName(float x, float y, char* str)
	{
		auto &trackMgr = *reinterpret_cast<CAERadioTrackManager *>(0x8CB6F8);

		CFont::SetCentreSize(RsGlobal.maximumWidth * 0.0015625f * 590.0f);
		CFont::SetFontStyle(RS_FONTSTYLE); // default FONT_MENU: 2
		CFont::SetScale(SCREEN_MULTIPLIER(RS_SIZE_X), SCREEN_MULTIPLIER(RS_SIZE_Y));
		CFont::SetDropShadowPosition(RS_SHADOW);
		if (RS_OUTLINE != 0)
			CFont::SetOutlinePosition(RS_OUTLINE);
		if (trackMgr.m_nStationsListed || trackMgr.m_nStationsListDown)
			CFont::SetColor(CRGBA(RS_RED1, RS_GREEN1, RS_BLUE1, 255));
		else
			CFont::SetColor(CRGBA(RS_RED2, RS_GREEN2, RS_BLUE2, 255));
		CFont::PrintString((float)(RsGlobal.maximumWidth / 2), SCREEN_COORD(RS_POS_Y), str);
	}

	static void CGarages__PrintMessages(float x, float y, char* str)
	{
		CFont::SetScale(SCREEN_MULTIPLIER(GARAGES_SIZE_X), SCREEN_MULTIPLIER(GARAGES_SIZE_Y));
		CFont::SetCentreSize(RsGlobal.maximumWidth * (1.0f - 0.0015625f * GARAGES_LINEWIDTH));	// defaulf: 230.0f
		CFont::SetFontStyle(GARAGES_FONTSTYLE); // default FONT_MENU: 2
		CFont::SetColor(CRGBA(GARAGES_R, GARAGES_G, GARAGES_B, 255));
		CFont::SetDropShadowPosition(GARAGES_SHADOW);
		if (GARAGES_OUTLINE != 0)
			CFont::SetOutlinePosition(GARAGES_OUTLINE);
		CFont::PrintString((float)(RsGlobal.maximumWidth / 2), SCREEN_COORD(GARAGES_Y), str);
	}

	ClassicHud() {

		static int keyPressTime = 0;
		static int show_splash = 0;

		CIniReader iniReader(CLASSICHUD_DAT_PATH);
		WEAPONS_TXD = iniReader.ReadString("SA_FILES", "WEAPONS_TXD", DEFAULT_WEAPONS_TXD);
		Events::initRwEvent += ClassicHud::ClassicHudTextures;
		Events::initRwEvent += ClassicHud::WeaponTextures;
		Events::shutdownRwEvent += ClassicHud::ClassicHudTextureShutdown;
		Events::shutdownRwEvent += ClassicHud::WeaponTexturesShutdown;

		static char str[100];

		Events::gameProcessEvent += [] {

			if (FindPlayerPed() && KeyPressed(VK_F9) && CTimer::m_snTimeInMilliseconds - keyPressTime > 500) {
				keyPressTime = CTimer::m_snTimeInMilliseconds;

				STYLE = (STYLE >= 3) ? 0 : STYLE + 1;
				ClassicHud::Init(STYLE);

				ClassicHud::WeaponTexturesShutdown();
				ClassicHud::WeaponTextures();

				patch::RedirectCall(0x5BA8AF, ClassicHud::HudTextures);
				patch::RedirectCall(0x5BA6A4, ClassicHud::FontTextures);
				patch::RedirectCall(0x7187DB, ClassicHud::FontData);
				
				CFont::Initialise();

				//CHud::Shutdown();
				//CHud::Initialise();

				patch::RedirectCall(0x58F607, ClassicHud::DrawMoney);
				patch::RedirectCall(0x58EC21, ClassicHud::DrawClock);

				patch::RedirectCall(0x589395, ClassicHud::DrawHealth);
				patch::RedirectCall(0x58917A, ClassicHud::DrawArmour);
				patch::RedirectCall(0x589252, ClassicHud::DrawBreath);

				patch::RedirectJump(0x58D7D0, ClassicHud::DrawWeaponIcon);
				patch::RedirectCall(0x58962A, ClassicHud::DrawWeaponAmmo);
				patch::RedirectCall(0x58FBDB, ClassicHud::DrawWanted);

				patch::RedirectCall(0x58AE5D, ClassicHud::AreaName);
				patch::RedirectCall(0x58AE02, ClassicHud::AreaNameColor);
				patch::RedirectCall(0x58B156, ClassicHud::VehicleName);
				patch::RedirectCall(0x58B0ED, ClassicHud::VehicleNameColor);

				patch::RedirectCall(0x58C68A, ClassicHud::DrawSubtitles);
				patch::RedirectCall(0x58FCFA, ClassicHud::DrawHelpText);

				/*// Set Radar Range
				patch::SetFloat(0x586C9B, RADAR_RANGE);
				patch::Set<const void*>(0x586C66, &RADAR_RANGE);

				// Transparent Radar
				patch::Set<DWORD>(0x5864BD, RADAR_A);
				plugin::patch::RedirectJump(0x58641A, RadarAlpha);

				//Radar Disc RGBA
				patch::RedirectCall(0x58A8BD, &SetRadarDisc);
				patch::RedirectCall(0x58A813, &SetRadarDisc);
				patch::RedirectCall(0x58A967, &SetRadarDisc);
				patch::RedirectCall(0x58AA15, &SetRadarDisc);

				SetRadarPosAndSize();*/
				plugin::patch::RedirectCall(0x58AA25, ClassicHud::MyDrawRadarCircle);
				//plugin::patch::RedirectCall(0x58A551, ClassicHud::MyDrawRadarPlane);
				//plugin::patch::RedirectCall(0x58A649, ClassicHud::MyDrawPlaneHeightBorder);
				//plugin::patch::RedirectCall(0x58A77A, ClassicHud::MyDrawPlaneHeight);
				plugin::patch::RedirectJump(0x583480, ClassicHud::MyTransformRadarPointToScreenSpace);
				plugin::patch::Nop(0x58A818, 16);
				plugin::patch::Nop(0x58A8C2, 16);
				plugin::patch::Nop(0x58A96C, 16);

				// Styled Texts
				patch::RedirectCall(0x58CA30, ClassicHud::DrawSuccessFailedMessage);
				patch::RedirectCall(0x58D470, ClassicHud::DrawMissionTitle);
				patch::RedirectCall(0x58CC6A, ClassicHud::DrawBustedWastedMessage);
				patch::RedirectCall(0x58CFA0, ClassicHud::DrawOddJobMessage4);
				patch::RedirectCall(0x58CD99, ClassicHud::DrawOddJobMessage5);
				patch::RedirectCall(0x58D21A, ClassicHud::DrawOddJobMessage6);
				patch::RedirectCall(0x58CE9E, ClassicHud::DrawOddJobMessage7);

				// Mission Timers (Timer and Status Text)
				patch::RedirectCall(0x58B420, ClassicHud::DrawMissionTimers_TimerText);
				patch::RedirectCall(0x58B3D2, ClassicHud::DrawMissionTimers_Timer);
				patch::RedirectCall(0x58B657, ClassicHud::DrawMissionTimers_StatusTextChartBar);
				patch::RedirectCall(0x58B58D, ClassicHud::DrawMissionTimers_StatusTextValue);
				patch::RedirectCall(0x58B6A6, ClassicHud::DrawMissionTimers_StatusTextDescription);

				patch::RedirectCall(0x4E9FF1, ClassicHud::DisplayRadioStationName);

				// Garage Messages
				patch::RedirectCall(0x4478D6, ClassicHud::CGarages__PrintMessages);
				patch::RedirectCall(0x447940, ClassicHud::CGarages__PrintMessages);
				patch::RedirectCall(0x44797E, ClassicHud::CGarages__PrintMessages);
			}
		};
	}

	static void Init(int style)
	{
		char section[20];
		char* prefix[5] = { "SA", "III", "VC", "LCS", "VCS" };
		CIniReader iniReader(CLASSICHUD_DAT_PATH);

		sprintf(section, "CUSTOM_TEXT");
		CUSTOM_ALIGN		= iniReader.ReadInt(section, "ALIGN", 0);
		CUSTOM_SHADOW		= iniReader.ReadInt(section, "SHADOW", 0);
		CUSTOM_OUTLINE		= iniReader.ReadInt(section, "OUTLINE", 0);
		CUSTOM_FONTSTYLE	= iniReader.ReadInt(section, "FONTSTYLE", 0);
		CUSTOM_R			= iniReader.ReadInt(section, "RED", 0);
		CUSTOM_G			= iniReader.ReadInt(section, "GREEN", 0);
		CUSTOM_B			= iniReader.ReadInt(section, "BLUE", 0);
		CUSTOM_A			= iniReader.ReadInt(section, "ALPHA", 0);
		CUSTOM_X			= iniReader.ReadFloat(section, "POS_X", 0.0f);
		CUSTOM_Y			= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		CUSTOM_SIZE_X		= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		CUSTOM_SIZE_Y		= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
		CUSTOM_LINEWIDTH	= iniReader.ReadFloat(section, "LINEWIDTH", 0.0f);

		sprintf(section, "%s_%s", prefix[style], "FILES");
		HUD_TXD		= iniReader.ReadString(section, "HUD_TXD", "404");
		FONTS_TXD	= iniReader.ReadString(section, "FONTS_TXD", "404");
		FONTS_DAT	= iniReader.ReadString(section, "FONTS_DAT", "404");
		WEAPONS_TXD	= iniReader.ReadString(section, "WEAPONS_TXD", "404");

		sprintf(section, "%s_%s", prefix[style], "MONEY");
		MONEY_X			= iniReader.ReadFloat(section, "POS_X", 0.0f);
		MONEY_Y			= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		MONEY_WIDTH		= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		MONEY_HEIGHT	= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
		MONEY_R			= iniReader.ReadInt(section, "RED", 47);
		MONEY_G			= iniReader.ReadInt(section, "GREEN", 90);
		MONEY_B			= iniReader.ReadInt(section, "BLUE", 38);
		MONEY_A			= iniReader.ReadInt(section, "ALPHA", 255);
		MONEY_OUTLINE	= iniReader.ReadInt(section, "OUTLINE", 2);
		MONEY_SHADOW	= iniReader.ReadInt(section, "SHADOW", 0);
		MONEY_FONTSTYLE	= iniReader.ReadInt(section, "FONTSTYLE", 0);
		
		sprintf(section, "%s_%s", prefix[style], "CLOCK");
		CLOCK_X			= iniReader.ReadFloat(section, "POS_X", 0.0f);
		CLOCK_Y			= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		CLOCK_WIDTH		= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		CLOCK_HEIGHT	= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
		CLOCK_R			= iniReader.ReadInt(section, "RED", 194);
		CLOCK_G			= iniReader.ReadInt(section, "GREEN", 165);
		CLOCK_B			= iniReader.ReadInt(section, "BLUE", 120);
		CLOCK_A			= iniReader.ReadInt(section, "ALPHA", 255);
		CLOCK_FONTSTYLE = iniReader.ReadInt(section, "FONTSTYLE", 0);

		sprintf(section, "%s_%s", prefix[style], "HEALTH");
		HEALTH_DISPLAY		= iniReader.ReadInt(section, "DISPLAY", 0);
		HEALTH_R			= iniReader.ReadInt(section, "RED", 180);
		HEALTH_G			= iniReader.ReadInt(section, "GREEN", 100);
		HEALTH_B			= iniReader.ReadInt(section, "BLUE", 50);
		HEALTH_A			= iniReader.ReadInt(section, "ALPHA", 255);
		HEALTHICON_X		= iniReader.ReadFloat(section, "ICON_X", 0.0f);
		HEALTHICON_Y		= iniReader.ReadFloat(section, "ICON_Y", 0.0f);
		HEALTHICON_WIDTH	= iniReader.ReadFloat(section, "ICON_SIZE_X", 0.0f);
		HEALTHICON_HEIGHT	= iniReader.ReadFloat(section, "ICON_SIZE_Y", 0.0f);
		HEALTH_X			= iniReader.ReadFloat(section, "POS_X", 0.0f);
		HEALTH_Y			= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		HEALTH_WIDTH		= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		HEALTH_HEIGHT		= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
		HEALTHBAR_X			= iniReader.ReadFloat(section, "BAR_X", 0.0f);
		HEALTHBAR_Y			= iniReader.ReadFloat(section, "BAR_Y", 0.0f);
		HEALTHBAR_SIZE_X	= iniReader.ReadFloat(section, "BAR_SIZE_X", 0.0f);
		HEALTHBAR_SIZE_Y	= iniReader.ReadFloat(section, "BAR_SIZE_Y", 0.0f);
		HEALTHBAR_BORDER	= iniReader.ReadInt(section, "BORDER", 0);
		HEALTH_TYPE			= iniReader.ReadInt(section, "TYPE", 0);

		sprintf(section, "%s_%s", prefix[style], "ARMOUR");
		ARMOUR_R			= iniReader.ReadInt(section, "RED", 121);
		ARMOUR_G			= iniReader.ReadInt(section, "GREEN", 136);
		ARMOUR_B			= iniReader.ReadInt(section, "BLUE", 93);
		ARMOUR_A			= iniReader.ReadInt(section, "ALPHA", 255);
		ARMOUR_X			= iniReader.ReadFloat(section, "POS_X", 0.0f);
		ARMOUR_Y			= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		ARMOUR_SIZE_X		= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		ARMOUR_SIZE_Y		= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
		ARMOURBAR_X			= iniReader.ReadFloat(section, "BAR_X", 0.0f);
		ARMOURBAR_Y			= iniReader.ReadFloat(section, "BAR_Y", 0.0f);
		ARMOURBAR_SIZE_X	= iniReader.ReadFloat(section, "BAR_SIZE_X", 0.0f);
		ARMOURBAR_SIZE_Y	= iniReader.ReadFloat(section, "BAR_SIZE_Y", 0.0f);
		ARMOURICON_X		= iniReader.ReadFloat(section, "ICON_X", 0.0f);
		ARMOURICON_Y		= iniReader.ReadFloat(section, "ICON_Y", 0.0f);
		ARMOURICON_SIZE_X	= iniReader.ReadFloat(section, "ICON_SIZE_X", 0.0f);
		ARMOURICON_SIZE_Y	= iniReader.ReadFloat(section, "ICON_SIZE_Y", 0.0f);
		ARMOURBAR_BORDER	= iniReader.ReadInt(section, "BORDER", 0);
		ARMOUR_TYPE			= iniReader.ReadInt(section, "TYPE", 0);

		sprintf(section, "%s_%s", prefix[style], "BREATH");
		BREATH_X			= iniReader.ReadFloat(section, "POS_X", 0.0f);
		BREATH_Y			= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		BREATH_SIZE_X		= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		BREATH_SIZE_Y		= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
		BREATHICON_X		= iniReader.ReadFloat(section, "ICON_X", 0.0f);
		BREATHICON_Y		= iniReader.ReadFloat(section, "ICON_Y", 0.0f);
		BREATHICON_SIZE_X	= iniReader.ReadFloat(section, "ICON_SIZE_X", 0.0f);
		BREATHICON_SIZE_Y	= iniReader.ReadFloat(section, "ICON_SIZE_Y", 0.0f);
		BREATHBAR_X			= iniReader.ReadFloat(section, "BAR_X", 0.0f);
		BREATHBAR_Y			= iniReader.ReadFloat(section, "BAR_Y", 0.0f);
		BREATHBAR_SIZE_X	= iniReader.ReadFloat(section, "BAR_SIZE_X", 0.0f);
		BREATHBAR_SIZE_Y	= iniReader.ReadFloat(section, "BAR_SIZE_Y", 0.0f);
		BREATH_R			= iniReader.ReadInt(section, "RED", 121);
		BREATH_G			= iniReader.ReadInt(section, "GREEN", 136);
		BREATH_B			= iniReader.ReadInt(section, "BLUE", 93);
		BREATH_A			= iniReader.ReadInt(section, "ALPHA", 255);
		BREATHBAR_BORDER	= iniReader.ReadInt(section, "BORDER", 0);
		BREATH_TYPE			= iniReader.ReadInt(section, "TYPE", 0);

		sprintf(section, "%s_%s", prefix[style], "AMMO");
		AMMO_SHADOW		= iniReader.ReadInt(section, "SHADOW", 0);
		AMMO_OUTLINE	= iniReader.ReadInt(section, "OUTLINE", 0);
		D_AMMO_R		= iniReader.ReadInt(section, "D_RED", 0);
		D_AMMO_G		= iniReader.ReadInt(section, "D_GREEN", 0);
		D_AMMO_B		= iniReader.ReadInt(section, "D_BLUE", 0);
		AMMO_R			= iniReader.ReadInt(section, "RED", 0);
		AMMO_G			= iniReader.ReadInt(section, "GREEN", 0);
		AMMO_B			= iniReader.ReadInt(section, "BLUE", 0);
		AMMO_SIZE_X		= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		AMMO_SIZE_Y		= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
		AMMO_X			= iniReader.ReadFloat(section, "POS_X", 0.0f);
		AMMO_Y			= iniReader.ReadFloat(section, "POS_Y", 0.0f);

		sprintf(section, "%s_%s", prefix[style], "WEAPON");
		WEAPON_A		= iniReader.ReadInt(section, "ALPHA", 0);
		WEAPON_X		= iniReader.ReadFloat(section, "POS_X", 0.0f);
		WEAPON_Y		= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		WEAPON_SIZE_X	= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		WEAPON_SIZE_Y	= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

		sprintf(section, "%s_%s", prefix[style], "WANTED");
		STAR_X			= iniReader.ReadFloat(section, "POS_X", 0.0f);
		STAR_Y			= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		STAR_SIZE_X		= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		STAR_SIZE_Y		= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
		STAR_SPACE		= iniReader.ReadFloat(section, "SPACE", 0.0f);
		STAR_R			= iniReader.ReadInt(section, "ACTIVE_R", 0);
		STAR_G			= iniReader.ReadInt(section, "ACTIVE_G", 0);
		STAR_B			= iniReader.ReadInt(section, "ACTIVE_B", 0);
		STARBACK_R		= iniReader.ReadInt(section, "INACTIVE_R", 0);
		STARBACK_G		= iniReader.ReadInt(section, "INACTIVE_G", 0);
		STARBACK_B		= iniReader.ReadInt(section, "INACTIVE_B", 0);
		FLASHSTAR_R		= iniReader.ReadInt(section, "FLASH_R", 0);
		FLASHSTAR_G		= iniReader.ReadInt(section, "FLASH_G", 0);
		FLASHSTAR_B		= iniReader.ReadInt(section, "FLASH_B", 0);
		SHADOWSTAR_A	= iniReader.ReadInt(section, "SHADOW_A1", 0);
		SHADOWSTAR_A2	= iniReader.ReadInt(section, "SHADOW_A2", 0);
		SHADOWSTAR_A3	= iniReader.ReadInt(section, "SHADOW_A3", 0);
		STAR_A2			= iniReader.ReadInt(section, "STAR_A2", 0);
		STAR_A3			= iniReader.ReadInt(section, "STAR_A3", 0);
		STAR_STYLE		= iniReader.ReadInt(section, "STYLE", 0);
		STAR_X_OFFSET	= iniReader.ReadFloat(section, "OFFSET_X", 0.0f);
		STAR_Y_OFFSET	= iniReader.ReadFloat(section, "OFFSET_Y", 0.0f);

		sprintf(section, "%s_%s", prefix[style], "AREA");
		AREA_FONTSTYLE	= iniReader.ReadInt(section, "FONTSTYLE", 0);
		AREA_R			= iniReader.ReadInt(section, "RED", 0);
		AREA_G			= iniReader.ReadInt(section, "GREEN", 0);
		AREA_B			= iniReader.ReadInt(section, "BLUE", 0);
		AREA_SHADOW		= iniReader.ReadInt(section, "SHADOW", 0);
		AREA_OUTLINE	= iniReader.ReadInt(section, "OUTLINE", 0);
		AREA_SIZE_X		= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		AREA_SIZE_Y		= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
		AREA_X			= iniReader.ReadFloat(section, "POS_X", 0.0f);
		AREA_Y			= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		AREA_SLANT		= iniReader.ReadFloat(section, "SLANT", 0.0f);

		sprintf(section, "%s_%s", prefix[style], "VEHICLE");
		VEHICLE_FONTSTYLE	= iniReader.ReadInt(section, "FONTSTYLE", 0);
		VEHICLE_R			= iniReader.ReadInt(section, "RED", 0);
		VEHICLE_G			= iniReader.ReadInt(section, "GREEN", 0);
		VEHICLE_B			= iniReader.ReadInt(section, "BLUE", 0);
		VEHICLE_SHADOW		= iniReader.ReadInt(section, "SHADOW", 0);
		VEHICLE_OUTLINE		= iniReader.ReadInt(section, "OUTLINE", 0);
		VEHICLE_SIZE_X		= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		VEHICLE_SIZE_Y		= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
		VEHICLE_X			= iniReader.ReadFloat(section, "POS_X", 0.0f);
		VEHICLE_Y			= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		VEHICLE_SLANT		= iniReader.ReadFloat(section, "SLANT", 0.0f);

		sprintf(section, "%s_%s", prefix[style], "SUBTITLES");
		SUBTITLES_SHADOW			= iniReader.ReadInt(section, "SHADOW", 0);
		SUBTITLES_OUTLINE			= iniReader.ReadInt(section, "OUTLINE", 0);
		SUBTITLES_R					= iniReader.ReadInt(section, "RED", 0);
		SUBTITLES_G					= iniReader.ReadInt(section, "GREEN", 0);
		SUBTITLES_B					= iniReader.ReadInt(section, "BLUE", 0);
		SUBTITLES_X					= iniReader.ReadFloat(section, "POS_X", 0.0f);
		SUBTITLES_Y					= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		SUBTITLES_SIZE_X			= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		SUBTITLES_SIZE_Y			= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
		SUBTITLES_LINEWIDTH			= iniReader.ReadFloat(section, "LINEWIDTH", 0.0f);

		sprintf(section, "%s_%s", prefix[style], "SUBTITLES_VITALSTATS");
		SUBTITLES_VITALSTATS_X = iniReader.ReadFloat(section, "POS_X1", 0.0f);
		SUBTITLES_VITALSTATS_SIZE_X = iniReader.ReadFloat(section, "SIZE_X1", 0.0f);

		sprintf(section, "%s_%s", prefix[style], "HELP_TEXT");
		fTextBoxPosnX					= iniReader.ReadFloat(section, "POS_X", 0.0f);
		fTextBoxPosnXWithRadar			= iniReader.ReadFloat(section, "POS_X", 0.0f);
		fTextBoxPosnXWithRadarAndPlane	= iniReader.ReadFloat(section, "POS_X", 0.0f);
		fTextBoxPosnY					= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		fTextBoxFontScaleX				= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		fTextBoxFontScaleY				= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
		fTextBoxWidth					= iniReader.ReadFloat(section, "WIDTH", 0.0f);
		fTextBoxBorderSize				= iniReader.ReadFloat(section, "BORDER_SIZE", 0.0f);

		sprintf(section, "%s_%s", prefix[style], "RADAR");
		RADAR_A			= iniReader.ReadInt(section, "ALPHA", 0);
		RADAR_RANGE		= iniReader.ReadFloat(section, "RANGE", 0.0f);
		RADAR_X			= iniReader.ReadFloat(section, "POS_X", 0.0f);
		RADAR_Y			= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		RADAR_SIZE_X	= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		RADAR_SIZE_Y	= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

		sprintf(section, "%s_%s", prefix[style], "STYLED1");
		STYLED1_SHADOW	= iniReader.ReadInt(section, "SHADOW", 0);
		STYLED1_OUTLINE	= iniReader.ReadInt(section, "OUTLINE", 0);
		STYLED1_R		= iniReader.ReadInt(section, "RED", 0);
		STYLED1_G		= iniReader.ReadInt(section, "GREEN", 0);
		STYLED1_B		= iniReader.ReadInt(section, "BLUE", 0);
		STYLED1_Y		= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		STYLED1_Y1		= iniReader.ReadFloat(section, "POS_Y1", 0.0f);
		STYLED1_Y2		= iniReader.ReadFloat(section, "POS_Y2", 0.0f);
		STYLED1_SIZE_X	= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		STYLED1_SIZE_Y	= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

		sprintf(section, "%s_%s", prefix[style], "STYLED2");
		MT_R			= iniReader.ReadInt(section, "RED", 0);
		MT_G			= iniReader.ReadInt(section, "GREEN", 0);
		MT_B			= iniReader.ReadInt(section, "BLUE", 0);
		MT_SHADOW		= iniReader.ReadInt(section, "SHADOW", 0);
		MT_OUTLINE		= iniReader.ReadInt(section, "OUTLINE", 0);
		MT_FONTSTYLE	= iniReader.ReadInt(section, "FONTSTYLE", 0);
		MT_POS_X		= iniReader.ReadFloat(section, "POS_X", 0.0f);
		MT_POS_Y		= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		MT_SIZE_X		= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		MT_SIZE_Y		= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

		sprintf(section, "%s_%s", prefix[style], "STYLED3");
		WB_R			= iniReader.ReadInt(section, "RED", 0);
		WB_G			= iniReader.ReadInt(section, "GREEN", 0);
		WB_B			= iniReader.ReadInt(section, "BLUE", 0);
		WB_SHADOW		= iniReader.ReadInt(section, "SHADOW", 0);
		WB_OUTLINE		= iniReader.ReadInt(section, "OUTLINE", 0);
		WB_FONTSTYLE	= iniReader.ReadInt(section, "FONTSTYLE", 0);
		WB_ALIGN		= iniReader.ReadInt(section, "ALIGN", 0);
		WB_POS_X		= iniReader.ReadFloat(section, "POS_X", 0.0f);
		WB_POS_Y		= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		WB_SIZE_X		= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		WB_SIZE_Y		= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

		sprintf(section, "%s_%s", prefix[style], "STYLED4");
		STYLED4_FONTSTYLE	= iniReader.ReadInt(section, "FONTSTYLE", 0);
		STYLED4_SHADOW		= iniReader.ReadInt(section, "SHADOW", 0);
		STYLED4_OUTLINE		= iniReader.ReadInt(section, "OUTLINE", 0);
		STYLED4_ALIGN		= iniReader.ReadInt(section, "ALIGN", 0);
		STYLED4_R			= iniReader.ReadInt(section, "RED", 0);
		STYLED4_G			= iniReader.ReadInt(section, "GREEN", 0);
		STYLED4_B			= iniReader.ReadInt(section, "BLUE", 0);
		STYLED4_X			= iniReader.ReadFloat(section, "POS_X", 0.0f);
		STYLED4_Y			= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		STYLED4_SIZE_X		= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		STYLED4_SIZE_Y		= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

		sprintf(section, "%s_%s", prefix[style], "STYLED5");
		STYLED5_FONTSTYLE	= iniReader.ReadInt(section, "FONTSTYLE", 0);
		STYLED5_SHADOW		= iniReader.ReadInt(section, "SHADOW", 0);
		STYLED5_OUTLINE		= iniReader.ReadInt(section, "OUTLINE", 0);
		STYLED5_R			= iniReader.ReadInt(section, "RED", 0);
		STYLED5_G			= iniReader.ReadInt(section, "GREEN", 0);
		STYLED5_B			= iniReader.ReadInt(section, "BLUE", 0);
		STYLED5_Y			= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		STYLED5_SIZE_X		= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		STYLED5_SIZE_Y		= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

		sprintf(section, "%s_%s", prefix[style], "STYLED6");
		STYLED6_FONTSTYLE	= iniReader.ReadInt(section, "FONTSTYLE", 0);
		STYLED6_SHADOW		= iniReader.ReadInt(section, "SHADOW", 0);
		STYLED6_OUTLINE		= iniReader.ReadInt(section, "OUTLINE", 0);
		STYLED6_R			= iniReader.ReadInt(section, "RED", 0);
		STYLED6_G			= iniReader.ReadInt(section, "GREEN", 0);
		STYLED6_B			= iniReader.ReadInt(section, "BLUE", 0);
		STYLED6_Y			= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		STYLED6_SIZE_X		= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		STYLED6_SIZE_Y		= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

		sprintf(section, "%s_%s", prefix[style], "STYLED7");
		STYLED7_FONTSTYLE	= iniReader.ReadInt(section, "FONTSTYLE", 0);
		STYLED7_SHADOW		= iniReader.ReadInt(section, "SHADOW", 0);
		STYLED7_OUTLINE		= iniReader.ReadInt(section, "OUTLINE", 0);
		STYLED7_R			= iniReader.ReadInt(section, "RED", 0);
		STYLED7_G			= iniReader.ReadInt(section, "GREEN", 0);
		STYLED7_B			= iniReader.ReadInt(section, "BLUE", 0);
		STYLED7_Y			= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		STYLED7_SIZE_X		= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		STYLED7_SIZE_Y		= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

		sprintf(section, "%s_%s", prefix[style], "MISSION_TIMERS");
		M_TIMERS_FONTSTYLE	= iniReader.ReadInt(section, "FONTSTYLE", 0);
		M_TIMERS_SHADOW		= iniReader.ReadInt(section, "SHADOW", 0);
		M_TIMERS_OUTLINE	= iniReader.ReadInt(section, "OUTLINE", 0);
		M_TIMERS_R			= iniReader.ReadInt(section, "RED", 0);
		M_TIMERS_G			= iniReader.ReadInt(section, "GREEN", 0);
		M_TIMERS_B			= iniReader.ReadInt(section, "BLUE", 0);
		M_TIMERS_SIZE_X		= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		M_TIMERS_SIZE_Y		= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
		M_TIMER_X			= iniReader.ReadFloat(section, "POS_X2", 0.0f);
		M_TIMER_DESC_X		= iniReader.ReadFloat(section, "POS_X1", 0.0f);
		M_TIMER_Y			= iniReader.ReadFloat(section, "POS_Y", 0.0f);

		sprintf(section, "%s_%s", prefix[style], "STATUS_TEXT");
		M_STATUSTEXT_X		= iniReader.ReadFloat(section, "POS_X2", 0.0f);
		M_STATUSTEXT_DESC_X = iniReader.ReadFloat(section, "POS_X1", 0.0f);
		M_STATUSTEXT_Y		= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		M_STATUSTEXT_SPACE	= iniReader.ReadFloat(section, "SPACE", 0.0f);

		sprintf(section, "%s_%s", prefix[style], "STATUS_TEXT_BAR");
		M_STATUSBAR_BORDER	= iniReader.ReadInt(section, "BORDER", 0);
		M_STATUSBAR_R		= iniReader.ReadInt(section, "RED", 0);
		M_STATUSBAR_G		= iniReader.ReadInt(section, "GREEN", 0);
		M_STATUSBAR_B		= iniReader.ReadInt(section, "BLUE", 0);
		M_STATUSBAR_A		= iniReader.ReadInt(section, "ALPHA", 0);
		M_STATUSBAR_X		= iniReader.ReadFloat(section, "POS_X", 0.0f);
		M_STATUSBAR_Y		= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		M_STATUSBAR_SIZE_X	= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		M_STATUSBAR_SIZE_Y	= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

		sprintf(section, "%s_%s", prefix[style], "RADIO_STATION");
		RS_FONTSTYLE	= iniReader.ReadInt(section, "FONTSTYLE", 0);
		RS_SHADOW		= iniReader.ReadInt(section, "SHADOW", 0);
		RS_OUTLINE		= iniReader.ReadInt(section, "OUTLINE", 0);
		RS_RED1			= iniReader.ReadInt(section, "RED1", 0);
		RS_GREEN1		= iniReader.ReadInt(section, "GREEN1", 0);
		RS_BLUE1		= iniReader.ReadInt(section, "BLUE1", 0);
		RS_RED2			= iniReader.ReadInt(section, "RED2", 0);
		RS_GREEN2		= iniReader.ReadInt(section, "GREEN2", 0);
		RS_BLUE2		= iniReader.ReadInt(section, "BLUE2", 0);
		RS_POS_Y		= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		RS_SIZE_X		= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		RS_SIZE_Y		= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);

		sprintf(section, "%s_%s", prefix[style], "GARAGE_MESSAGES");
		GARAGES_R			= iniReader.ReadInt(section, "RED", 0);
		GARAGES_G			= iniReader.ReadInt(section, "GREEN", 0);
		GARAGES_B			= iniReader.ReadInt(section, "BLUE", 0);
		GARAGES_FONTSTYLE	= iniReader.ReadInt(section, "FONTSTYLE", 0);
		GARAGES_SHADOW		= iniReader.ReadInt(section, "SHADOW", 0);
		GARAGES_OUTLINE		= iniReader.ReadInt(section, "OUTLINE", 0);
		GARAGES_Y			= iniReader.ReadFloat(section, "POS_Y", 0.0f);
		GARAGES_SIZE_X		= iniReader.ReadFloat(section, "SIZE_X", 0.0f);
		GARAGES_SIZE_Y		= iniReader.ReadFloat(section, "SIZE_Y", 0.0f);
		GARAGES_LINEWIDTH	= iniReader.ReadFloat(section, "LINEWIDTH", 0.0f);
	}

} classicHud;




class HudEditor
{
public:
	static void DrawTestString(float x, float y, float w, float h, eFontAlignment align,
		float linewidth, int shadow, int outline, int fontstyle, CRGBA color, char* str)
	{
		CFont::SetBackground(0, 0);
		CFont::SetScale(SCREEN_MULTIPLIER(w), SCREEN_MULTIPLIER(h));
		CFont::SetProp(1);
		CFont::SetJustify(0);
		CFont::SetAlignment(align);
		CFont::SetCentreSize(linewidth);
		CFont::SetFontStyle(fontstyle);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetDropShadowPosition(shadow);
		if (outline != 0) CFont::SetOutlinePosition(outline);
		CFont::SetSlantRefPoint(x, y);
		CFont::SetSlant(0.0);
		CFont::SetColor(color);
		CFont::PrintString(x, y, str);
	}

	static char* ModifyValueInIni(char* section, char* key, float incr)
	{
		std::string prefix[5] = { "SA_", "III_", "VC_", "LCS_", "VCS_" };
		std::string s_section = prefix[STYLE] + (std::string)section;

		if ((std::string)section == "CUSTOM_TEXT") s_section = section;

		CIniReader iniReader(CLASSICHUD_DAT_PATH);
		float value = iniReader.ReadFloat((char*)s_section.c_str(), key, 0.0f);

		if (KeyPressed(VK_ADD)) {
			value += incr;
		}
		if (KeyPressed(VK_SUBTRACT)) {
			value -= incr;
		}
		iniReader.WriteFloat((char*)s_section.c_str(), key, value);

		ClassicHud::Init(STYLE);
		if ((std::string)section == "MONEY")
			patch::RedirectCall(0x58F607, ClassicHud::DrawMoney);
		else if ((std::string)section == "CLOCK")
			patch::RedirectCall(0x58EC21, ClassicHud::DrawClock);
		else if ((std::string)section == "HEALTH")
			patch::RedirectCall(0x589395, ClassicHud::DrawHealth);
		else if ((std::string)section == "ARMOUR")
			patch::RedirectCall(0x58917A, ClassicHud::DrawArmour);
		else if ((std::string)section == "BREATH")
			patch::RedirectCall(0x589252, ClassicHud::DrawBreath);
		else if ((std::string)section == "WEAPON")
			patch::RedirectJump(0x58D7D0, ClassicHud::DrawWeaponIcon);
		else if ((std::string)section == "AMMO")
			patch::RedirectCall(0x58962A, ClassicHud::DrawWeaponAmmo);
		else if ((std::string)section == "WANTED")
			patch::RedirectCall(0x58FBDB, ClassicHud::DrawWanted);
		else if ((std::string)section == "HELP_TEXT")
			patch::RedirectCall(0x58FCFA, ClassicHud::DrawHelpText);
		else if ((std::string)section == "RADAR")
			ClassicHud::SetRadarPosAndSize();

		char str[200];
		_snprintf(str, sizeof(str), "section: %s ~n~key: %s ~n~value: %f ~n~incr: %f",
			section, key, value, incr);
		return str;
	}

	HudEditor()
	{
		static bool enable = false;
		static int keyPressTime = 0;
		static int menu_level = 0, chosen = 0;
		static bool showStyled1 = false;
		static char custom_text[200];
		static std::string section;

		Events::drawingEvent += []
		{
			if (section == "AREA")
				AreaName();
			else if (section == "VEHICLE")
				VehicleName();
			else if (section == "RADIO_STATION")
				DisplayRadioStationName();
			else if (section == "SUBTITLES")
				DrawSubtitles();
			else if (section == "SUBTITLES_VITALSTATS")
				DrawSubtitles_Vitalstats();
			else if (section == "STYLED1")
				DrawSuccessFailedMessage();
			else if (section == "STYLED2")
				DrawMissionTitle();
			else if (section == "STYLED3")
				DrawBustedWastedMessage();
			else if (section == "STYLED4")
				DrawOddJobMessage4();
			else if (section == "STYLED5")
				DrawOddJobMessage5();
			else if (section == "STYLED6")
				DrawOddJobMessage6();
			else if (section == "STYLED7")
				DrawOddJobMessage7();
			else if (section == "GARAGE_MESSAGES")
				CGarages__PrintMessages();
			else if (section == "MISSION_TIMERS"
				|| section == "STATUS_TEXT"
				|| section == "STATUS_TEXT_BAR")
				DrawMissionTimers();

			if (enable)
				DrawTestString(SCREEN_COORD(CUSTOM_X), SCREEN_COORD(CUSTOM_Y),
					CUSTOM_SIZE_X, CUSTOM_SIZE_Y, (eFontAlignment)CUSTOM_ALIGN,
					CUSTOM_LINEWIDTH, CUSTOM_SHADOW, CUSTOM_OUTLINE, CUSTOM_FONTSTYLE,
					CRGBA(CUSTOM_R, CUSTOM_G, CUSTOM_B, CUSTOM_A), custom_text);
			if (enable && menu_level == 1)
				DrawTestString(SCREEN_COORD(CUSTOM_X), SCREEN_COORD(CUSTOM_Y - 30.0f),
					CUSTOM_SIZE_X, CUSTOM_SIZE_Y, (eFontAlignment)CUSTOM_ALIGN,
					CUSTOM_LINEWIDTH, CUSTOM_SHADOW, CUSTOM_OUTLINE, CUSTOM_FONTSTYLE,
					CRGBA(255, 255, 255, CUSTOM_A), "Edit mode");
		};

		Events::gameProcessEvent += [] {

			if (FindPlayerPed() && KeyPressed(VK_F10) && CTimer::m_snTimeInMilliseconds - keyPressTime > 500) {
				keyPressTime = CTimer::m_snTimeInMilliseconds;
				enable = !enable;
				menu_level = 0; chosen = 0; menu_level = 0;
				sprintf(custom_text, "Money Pos X");
				CHud::SetHelpMessage(enable?"Hud Editor enabled!":"Hud Editor disabled!", true, false, false);
			}
			
			if (enable && FindPlayerPed() && KeyPressed(VK_MULTIPLY) && CTimer::m_snTimeInMilliseconds - keyPressTime > 500) {
				keyPressTime = CTimer::m_snTimeInMilliseconds;
				menu_level = (menu_level == 0) ? 1 : 0;
			}

			if (enable && FindPlayerPed() && (KeyPressed(VK_ADD) || KeyPressed(VK_SUBTRACT)) && CTimer::m_snTimeInMilliseconds - keyPressTime > 50) {
				keyPressTime = CTimer::m_snTimeInMilliseconds;

				std::string labels[] =			{ "Money Pos X",	"Money Pos Y",	"Money Size X",	"Money Size Y",	"Clock Pos X",	"Clock Pos Y",	"Clock Size X",	"Clock Size Y",	"Health Pos X",	"Health Pos Y",	"Health Size X",	"Health Size Y",	"Health Bar Pos X",	"Health Bar Pos Y",	"Health Bar Size X",	"Health Bar Size Y",	"Health Icon Pos X",	"Health Icon Pos Y",	"Health Icon Size X",	"Health Icon Size Y",	"Armour Pos X",	"Armour Pos Y",	"Armour Size X",	"Armour Size Y",	"Armour Icon Pos X",	"Armour Icon Pos Y",	"Armour Icon Size X",	"Armour Icon Size Y",	"Armour Bar Pos X",	"Armour Bar Pos Y",	"Armour Bar Size X",	"Armour Bar Size Y",	"Breath Pos X",	"Breath Pos Y",	"Breath Size X",	"Breath Size Y",	"Breath Bar Pos X",	"Breath Bar Pos Y",	"Breath Bar Size X",	"Breath Bar Size Y",	"Breath Icon Pos X",	"Breath Icon Pos Y",	"Breath Icon Size X",	"Breath Icon Size Y",	"Weapon Pos X",	"Weapon Pos Y",	"Weapon Size X",	"Weapon Size Y",	"Ammo Pos X",	"Ammo Pos Y",	"Ammo Size X",	"Ammo Size Y",	"Wanted Stars Pos X",	"Wanted Stars Pos Y",	"Wanted Stars Size X",	"Wanted Stars Size Y",	"Wanted Stars Space Between",	"Area Pos X",	"Area Pos Y",	"Area Size X",	"Area Size Y",	"Area Slant",	"Vehicle Pos X",	"Vehicle Pos Y",	"Vehicle Size X",	"Vehicle Size Y",	"Vehicle Slant",	"Subtitles Pos X",	"Subtitles Pos Y",	"Subtitles Size X",	"Subtitles Size Y",	"Subtitles Linewidth",	"Subtitles Vitalstats Offset X",	"Subtitles Vitalstats Size X Factor",	"Help Text Pos X",	"Help Text Pos Y",	"Help Text Size X",	"Help Text Size Y",	"Help Text Width",	"Help Text Border Size",	"Radar Pos X",	"Radar Pos Y",	"Radar Size X",	"Radar Size Y",	"Styled Text 1 Pos Y",	"Styled Text 1 Offset Y1",	"Styled Text 1 Offset Y2",	"Styled Text 1 Size X",	"Styled Text 1 Size Y",	"Mission Title Pos X",	"Mission Title Pos Y",	"Mission Title Size X",	"Mission Title Size Y",	"Wasted Busted Pos X",	"Wasted Busted Pos Y",	"Wasted Busted Size X",	"Wasted Busted Size Y",	"Styled Text 4 Pos X",	"Styled Text 4 Pos Y",	"Styled Text 4 Size X",	"Styled Text 4 Size Y",	"Styled Text 5 Pos Y",	"Styled Text 5 Size X",	"Styled Text 5 Size Y",	"Styled Text 6 Pos Y",	"Styled Text 6 Size X",	"Styled Text 6 Size Y",	"Styled Text 7 Pos Y",	"Styled Text 7 Size X",	"Styled Text 7 Size Y",	"Mission Timers (Timer Desc) Pos X",	"Mission Timers (Timer) Pos X",	"Mission Timers Pos Y",	"Mission Timers Size X",	"Mission Timers Size Y",	"Status Text (Desc) Pos X",	"Status Text (Value) Pos X",	"Status Text Pos Y",	"Status Text Space Between",	"Status Text Bar Pos X",	"Status Text Bar Pos Y",	"Status Text Bar Size X",	"Status Text Bar Size Y",	"Radio Station Pos Y",	"Radio Station Size X",	"Radio Station Size Y",	"Garage Pos Y",		"Garage Size X",	"Garage Size Y",	"Garage Linewidth",	"Custom Text Pos X",	"Custom Text Pos Y",	"Custom Text Size X",	"Custom Text Size Y",	"Custom Text Linewidth" };
				std::string sections[] =		{ "MONEY",			"MONEY",		"MONEY",		"MONEY",		"CLOCK",		"CLOCK",		"CLOCK",		"CLOCK",		"HEALTH",		"HEALTH",		"HEALTH",			"HEALTH",			"HEALTH",			"HEALTH",			"HEALTH",				"HEALTH",				"HEALTH",				"HEALTH",				"HEALTH",				"HEALTH",				"ARMOUR",		"ARMOUR",		"ARMOUR",			"ARMOUR",			"ARMOUR",				"ARMOUR",				"ARMOUR",				"ARMOUR",				"ARMOUR",			"ARMOUR",			"ARMOUR",				"ARMOUR",				"BREATH",		"BREATH",		"BREATH",			"BREATH",			"BREATH",			"BREATH",			"BREATH",				"BREATH",				"BREATH",				"BREATH",				"BREATH",				"BREATH",				"WEAPON",		"WEAPON",		"WEAPON",			"WEAPON",			"AMMO",			"AMMO",			"AMMO",			"AMMO",			"WANTED",				"WANTED",				"WANTED",				"WANTED",				"WANTED",						"AREA",			"AREA",			"AREA",			"AREA",			"AREA",			"VEHICLE",			"VEHICLE",			"VEHICLE",			"VEHICLE",			"VEHICLE",			"SUBTITLES",		"SUBTITLES",		"SUBTITLES",		"SUBTITLES",		"SUBTITLES",			"SUBTITLES_VITALSTATS",				"SUBTITLES_VITALSTATS",					"HELP_TEXT",		"HELP_TEXT",		"HELP_TEXT",		"HELP_TEXT",		"HELP_TEXT",		"HELP_TEXT",				"RADAR",		"RADAR",		"RADAR",		"RADAR",		"STYLED1",				"STYLED1",					"STYLED1",					"STYLED1",				"STYLED1",				"STYLED2",				"STYLED2",				"STYLED2",				"STYLED2",				"STYLED3",				"STYLED3",				"STYLED3",				"STYLED3",				"STYLED4",				"STYLED4",				"STYLED4",				"STYLED4",				"STYLED5",				"STYLED5",				"STYLED5",				"STYLED6",				"STYLED6",				"STYLED6",				"STYLED7",				"STYLED7",				"STYLED7",				"MISSION_TIMERS",						"MISSION_TIMERS",				"MISSION_TIMERS",		"MISSION_TIMERS",			"MISSION_TIMERS",			"STATUS_TEXT",				"STATUS_TEXT",					"STATUS_TEXT",			"STATUS_TEXT",					"STATUS_TEXT_BAR",			"STATUS_TEXT_BAR",			"STATUS_TEXT_BAR",			"STATUS_TEXT_BAR",			"RADIO_STATION",		"RADIO_STATION",		"RADIO_STATION",		"GARAGE_MESSAGES",	"GARAGE_MESSAGES",	"GARAGE_MESSAGES",	"GARAGE_MESSAGES",	"CUSTOM_TEXT",			"CUSTOM_TEXT",			"CUSTOM_TEXT",			"CUSTOM_TEXT",			"CUSTOM_TEXT" };
				std::string keys[] =			{ "POS_X",			"POS_Y",		"SIZE_X",		"SIZE_Y",		"POS_X",		"POS_Y",		"SIZE_X",		"SIZE_Y",		"POS_X",		"POS_Y",		"SIZE_X",			"SIZE_Y",			"BAR_X",			"BAR_Y",			"BAR_SIZE_X",			"BAR_SIZE_Y",			"ICON_X",				"ICON_Y",				"ICON_SIZE_X",			"ICON_SIZE_Y",			"POS_X",		"POS_Y",		"SIZE_X",			"SIZE_Y",			"ICON_X",				"ICON_Y",				"ICON_SIZE_X",			"ICON_SIZE_Y",			"BAR_X",			"BAR_Y",			"BAR_SIZE_X",			"BAR_SIZE_Y",			"POS_X",		"POS_Y",		"SIZE_X",			"SIZE_Y",			"BAR_X",			"BAR_Y",			"BAR_SIZE_X",			"BAR_SIZE_Y",			"ICON_X",				"ICON_Y",				"ICON_SIZE_X",			"ICON_SIZE_Y",			"POS_X",		"POS_Y",		"SIZE_X",			"SIZE_Y",			"POS_X",		"POS_Y",		"SIZE_X",		"SIZE_Y",		"POS_X",				"POS_Y",				"SIZE_X",				"SIZE_Y",				"SPACE",						"POS_X",		"POS_Y",		"SIZE_X",		"SIZE_Y",		"SLANT",		"POS_X",			"POS_Y",			"SIZE_X",			"SIZE_Y",			"SLANT",			"POS_X",			"POS_Y",			"SIZE_X",			"SIZE_Y",			"LINEWIDTH",			"POS_X1",							"SIZE_X1",								"POS_X",			"POS_Y",			"SIZE_X",			"SIZE_Y",			"WIDTH",			"BORDER_SIZE",				"POS_X",		"POS_Y",		"SIZE_X",		"SIZE_Y",		"POS_Y",				"POS_Y1",					"POS_Y2",					"SIZE_X",				"SIZE_Y",				"POS_X",				"POS_Y",				"SIZE_X",				"SIZE_Y",				"POS_X",				"POS_Y",				"SIZE_X",				"SIZE_Y",				"POS_X",				"POS_Y",				"SIZE_X",				"SIZE_Y",				"POS_Y",				"SIZE_X",				"SIZE_Y",				"POS_Y",				"SIZE_X",				"SIZE_Y",				"POS_Y",				"SIZE_X",				"SIZE_Y",				"POS_X1",								"POS_X2",						"POS_Y",				"SIZE_X",					"SIZE_Y",					"POS_X1",					"POS_X2",						"POS_Y",				"SPACE",						"POS_X",					"POS_Y",					"SIZE_X",					"SIZE_Y",					"POS_Y",				"SIZE_X",				"SIZE_Y",				"POS_Y",			"SIZE_X",			"SIZE_Y",			"LINEWIDTH",		"POS_X",				"POS_Y",				"SIZE_X",				"SIZE_Y",				"LINEWIDTH" };
				float		incr[] =			{ 1.0f,				1.0f,			0.01f,			0.01f,			1.0f,			1.0f,			0.01f,			0.01f,			1.0f,			1.0f,			0.01f,				0.01f,				2.0f,				2.0f,				1.0f,					1.0f,					2.0f,					2.0f,					0.5f,					0.5f,					1.0f,			1.0f,			0.01f,				0.01f,				2.0f,					2.0f,					0.5f,					0.5f,					2.0f,				2.0f,				1.0f,					1.0f,					1.0f,			1.0f,			0.01f,				0.01f,				2.0f,				2.0f,				1.0f,					1.0f,					2.0f,					2.0f,					0.5f,					0.5f,					1.0f,			1.0f,			1.0f,				1.0f,				1.0f,			1.0f,			0.01f,			0.01f,			2.0f,					2.0f,					1.0f,					1.0f,					1.0f,							1.0f,			1.0f,			0.01f,			0.01f,			0.01f,			1.0f,				1.0f,				0.01f,				0.01f,				0.01f,				1.0f,				1.0f,				0.01f,				0.01f,				5.0f,					1.0f,								0.01f,									1.0f,				1.0f,				0.01f,				0.01f,				1.0f,				1.0f,						1.0f,			1.0f,			1.0f,			1.0f,			2.0f,					1.0f,						1.0f,						0.01f,					0.01f,					2.0f,					2.0f,					0.01f,					0.01f,					2.0f,					2.0f,					0.01f,					0.01f,					2.0f,					2.0f,					0.01f,					0.01f,					2.0f,					0.01f,					0.01f,					2.0f,					0.01f,					0.01f,					2.0f,					0.01f,					0.01f,					2.0f,									2.0f,							2.0f,					0.01f,						0.02f,						1.0f,						1.0f,							1.0f,					0.01f,							1.0f,						1.0f,						0.5f,						1.0f,						1.0f,					0.01f,					0.02f,					1.0f,				0.01f,				0.02f,				10.0f,				1.0f,					1.0f,					0.01f,					0.02f,					10.0f };
				int			size = sizeof(incr) / sizeof(incr[0]);

				switch (menu_level)
				{
				default:
				case 0:
					if (KeyPressed(VK_ADD))
						chosen = (chosen == size - 1) ? 0 : chosen + 1;
					if (KeyPressed(VK_SUBTRACT))
						chosen = (chosen == 0) ? size - 1 : chosen - 1;
					sprintf(custom_text, labels[chosen].c_str());
						break;
				case 1:
					HudEditor::ModifyValueInIni((char*)sections[chosen].c_str(),
						(char*)keys[chosen].c_str(), incr[chosen]);
					break;
				}

				section = sections[chosen];
			}

		};
	}

	static void AreaName()
	{
		CText text;
		CFont::SetBackground(0, 0);
		CFont::SetScale(SCREEN_MULTIPLIER(AREA_SIZE_X), SCREEN_MULTIPLIER(AREA_SIZE_Y));
		CFont::SetProp(1);
		CFont::SetJustify(0);
		CFont::SetAlignment(ALIGN_RIGHT);
		CFont::SetCentreSize(RsGlobal.maximumWidth * 0.0015625f * 590.0f);
		CFont::SetFontStyle(AREA_FONTSTYLE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetSlantRefPoint(RsGlobal.maximumWidth - SCREEN_COORD(AREA_X), SCREEN_COORD_BOTTOM(AREA_Y));
		CFont::SetSlant(AREA_SLANT);
		CFont::SetDropShadowPosition(AREA_SHADOW);
		if (AREA_OUTLINE != 0)
			CFont::SetOutlinePosition(AREA_OUTLINE);
		CFont::SetColor(CRGBA(AREA_R, AREA_G, AREA_B, 255));
		CFont::PrintString(
			RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + AREA_X),
			SCREEN_COORD_BOTTOM(HUD_POS_Y + AREA_Y), text.Get("GAN")); // Ganton
	}

	static void VehicleName()
	{
		CText text;
		CFont::SetBackground(0, 0);
		CFont::SetScale(SCREEN_MULTIPLIER(VEHICLE_SIZE_X), SCREEN_MULTIPLIER(VEHICLE_SIZE_Y));
		CFont::SetProp(1);
		CFont::SetJustify(0);
		CFont::SetAlignment(ALIGN_RIGHT);
		CFont::SetCentreSize(RsGlobal.maximumWidth * 0.0015625f * 590.0f);
		CFont::SetFontStyle(VEHICLE_FONTSTYLE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetSlantRefPoint(RsGlobal.maximumWidth - SCREEN_COORD(VEHICLE_X), SCREEN_COORD_BOTTOM(VEHICLE_Y));
		CFont::SetSlant(VEHICLE_SLANT);
		CFont::SetDropShadowPosition(VEHICLE_SHADOW);
		if (VEHICLE_OUTLINE != 0)
			CFont::SetOutlinePosition(VEHICLE_OUTLINE);
		CFont::SetColor(CRGBA(VEHICLE_R, VEHICLE_G, VEHICLE_B, 255));
		CFont::PrintString(
			RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + VEHICLE_X),
			SCREEN_COORD_BOTTOM(HUD_POS_Y + VEHICLE_Y), text.Get("BMX")); // BMX
	}

	static void DrawSubtitles()
	{
		CText text;
		CFont::SetBackground(0, 0);
		CFont::SetScaleForCurrentlanguage(
			SCREEN_MULTIPLIER(SUBTITLES_SIZE_X),
			SCREEN_MULTIPLIER(SUBTITLES_SIZE_Y));
		CFont::SetProp(1);
		CFont::SetJustify(0);
		CFont::SetAlignment(ALIGN_CENTER);
		CFont::SetCentreSize(RsGlobal.maximumWidth
			- 0.0015625f * RsGlobal.maximumWidth * SUBTITLES_LINEWIDTH
			- 8.0f * (0.0015625f * RsGlobal.maximumWidth)
			- (0.0015625f * RsGlobal.maximumWidth * 140.0f
				+ 8.0f * (0.0015625f * RsGlobal.maximumWidth)));
		CFont::SetFontStyle(FONT_SUBTITLES);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetDropShadowPosition(SUBTITLES_SHADOW);
		if (SUBTITLES_OUTLINE != 0)
			CFont::SetOutlinePosition(SUBTITLES_OUTLINE);
		CFont::SetColor(CRGBA(SUBTITLES_R, SUBTITLES_G, SUBTITLES_B, 255));
		CFont::SetSlantRefPoint(0.0, 0.0);
		CFont::SetSlant(0.0);
		CFont::PrintString(
			SCREEN_COORD_CENTER_LEFT(SUBTITLES_X),
			SCREEN_COORD_BOTTOM(SUBTITLES_Y),
			text.Get("IE23")); // ~s~This vehicle is not required for export.
	}

	static void DrawSubtitles_Vitalstats()
	{
		CText text;
		CFont::SetBackground(0, 0);
		CFont::SetScaleForCurrentlanguage(
			SCREEN_MULTIPLIER(SUBTITLES_SIZE_X * SUBTITLES_VITALSTATS_SIZE_X),
			SCREEN_MULTIPLIER(SUBTITLES_SIZE_Y));
		CFont::SetProp(1);
		CFont::SetJustify(0);
		CFont::SetAlignment(ALIGN_CENTER);
		CFont::SetCentreSize((RsGlobal.maximumWidth
			- 0.0015625f * RsGlobal.maximumWidth * SUBTITLES_LINEWIDTH
			- 8.0f * (0.0015625f * RsGlobal.maximumWidth)
			- (0.0015625f * RsGlobal.maximumWidth * 140.0f
				+ 8.0f * (0.0015625f * RsGlobal.maximumWidth))) * SUBTITLES_VITALSTATS_SIZE_X);
		CFont::SetFontStyle(FONT_SUBTITLES);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetDropShadowPosition(SUBTITLES_SHADOW);
		if (SUBTITLES_OUTLINE != 0)
			CFont::SetOutlinePosition(SUBTITLES_OUTLINE);
		CFont::SetColor(CRGBA(SUBTITLES_R, SUBTITLES_G, SUBTITLES_B, 255));
		CFont::SetSlantRefPoint(0.0, 0.0);
		CFont::SetSlant(0.0);
		CFont::PrintString(
			SCREEN_COORD_CENTER_LEFT(SUBTITLES_X + SUBTITLES_VITALSTATS_X),
			SCREEN_COORD_BOTTOM(SUBTITLES_Y),
			text.Get("IE23")); // ~s~This vehicle is not required for export.
	}

	static void DrawSuccessFailedMessage()
	{
		CText text;
		float pos_y = SCREEN_COORD(STYLED1_Y) + STYLED1_Y2,
			pos_x = (float)(RsGlobal.maximumWidth / 2);
		DrawTestString(pos_x, pos_y, STYLED1_SIZE_X, STYLED1_SIZE_Y, ALIGN_CENTER,
			RsGlobal.maximumWidth * 0.0015625f * 590.0f,
			STYLED1_SHADOW, STYLED1_OUTLINE, FONT_PRICEDOWN,
			CRGBA(STYLED1_R, STYLED1_G, STYLED1_B, 255), text.Get("M_FAIL")); // MISSION FAILED!
	}

	static void DrawMissionTitle()
	{
		CText text;
		DrawTestString(RsGlobal.maximumWidth - SCREEN_COORD(MT_POS_X),
			SCREEN_COORD(MT_POS_Y), MT_SIZE_X, MT_SIZE_Y, ALIGN_RIGHT,
			RsGlobal.maximumWidth * 0.0015625f * 590.0f,
			MT_SHADOW, MT_OUTLINE, MT_FONTSTYLE,
			CRGBA(MT_R, MT_G, MT_B, 255), text.Get("BEEFY")); // Beefy Baron
	}

	static void DrawBustedWastedMessage()
	{
		CText text;
		DrawTestString(RsGlobal.maximumWidth - SCREEN_COORD(WB_POS_X),
			SCREEN_COORD(WB_POS_Y), WB_SIZE_X, WB_SIZE_Y, (eFontAlignment)WB_ALIGN,
			RsGlobal.maximumWidth * 0.0015625f * 590.0f,
			WB_SHADOW, WB_OUTLINE, WB_FONTSTYLE,
			CRGBA(WB_R, WB_G, WB_B, 255), text.Get("DEAD")); // Wasted
	}

	static void DrawOddJobMessage4()
	{
		CText text;
		CFont::SetBackground(0, 0);
		CFont::SetScaleForCurrentlanguage(SCREEN_MULTIPLIER(STYLED4_SIZE_X), SCREEN_MULTIPLIER(STYLED4_SIZE_Y));
		CFont::SetProp(1);
		CFont::SetJustify(0);
		CFont::SetAlignment((eFontAlignment)STYLED4_ALIGN);
		CFont::SetCentreSize(RsGlobal.maximumWidth * 0.0015625f * 500.0f);
		CFont::SetFontStyle(STYLED4_FONTSTYLE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetDropShadowPosition(STYLED4_SHADOW);
		if (STYLED4_OUTLINE != 0)
			CFont::SetOutlinePosition(STYLED4_OUTLINE);
		CFont::SetColor(CRGBA(STYLED4_R, STYLED4_G, STYLED4_B, 255));
		CFont::SetSlantRefPoint(0.0, 0.0);
		CFont::SetSlant(0.0);
		CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(STYLED4_X),
			SCREEN_COORD(STYLED4_Y), text.Get("TAXI_M")); // ~y~'TAXI DRIVER'
	}

	void static DrawOddJobMessage5()
	{
		CText text;
		CFont::SetBackground(0, 0);
		CFont::SetScaleForCurrentlanguage(SCREEN_MULTIPLIER(STYLED5_SIZE_X), SCREEN_MULTIPLIER(STYLED5_SIZE_Y));
		CFont::SetProp(1);
		CFont::SetJustify(0);
		CFont::SetAlignment(ALIGN_CENTER);
		CFont::SetCentreSize(RsGlobal.maximumWidth * 0.0015625f * 600.0f);
		CFont::SetFontStyle(STYLED5_FONTSTYLE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetDropShadowPosition(STYLED5_SHADOW);
		if (STYLED5_OUTLINE != 0)
			CFont::SetOutlinePosition(STYLED5_OUTLINE);
		CFont::SetColor(CRGBA(STYLED5_R, STYLED5_G, STYLED5_B, 255));
		CFont::SetSlantRefPoint(0.0, 0.0);
		CFont::SetSlant(0.0);
		CFont::PrintStringFromBottom((float)(RsGlobal.maximumWidth / 2),
			SCREEN_COORD(STYLED5_Y), text.Get("RACES_7")); // GO!
	}

	void static DrawOddJobMessage6()
	{
		CText text;
		CFont::SetBackground(0, 0);
		CFont::SetScaleForCurrentlanguage(SCREEN_MULTIPLIER(STYLED6_SIZE_X), SCREEN_MULTIPLIER(STYLED6_SIZE_Y));
		CFont::SetProp(1);
		CFont::SetJustify(0);
		CFont::SetAlignment(ALIGN_CENTER);
		CFont::SetCentreSize(RsGlobal.maximumWidth * 0.0015625f * 500.0f);
		CFont::SetFontStyle(STYLED6_FONTSTYLE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetDropShadowPosition(STYLED6_SHADOW);
		if (STYLED6_OUTLINE != 0)
			CFont::SetOutlinePosition(STYLED6_OUTLINE);
		CFont::SetColor(CRGBA(STYLED6_R, STYLED6_G, STYLED6_B, 255));
		CFont::SetSlantRefPoint(0.0, 0.0);
		CFont::SetSlant(0.0);
		CFont::PrintString((float)(RsGlobal.maximumWidth / 2),
			SCREEN_COORD(STYLED6_Y), text.Get("ASS_ACQ")); // Asset acquired!
	}

	void static DrawOddJobMessage7()
	{
		CText text;
		CFont::SetBackground(0, 0);
		CFont::SetScaleForCurrentlanguage(SCREEN_MULTIPLIER(STYLED7_SIZE_X), SCREEN_MULTIPLIER(STYLED7_SIZE_Y));
		CFont::SetProp(1);
		CFont::SetJustify(0);
		CFont::SetAlignment(ALIGN_CENTER);
		CFont::SetCentreSize(RsGlobal.maximumWidth * 0.0015625f * 500.0f);
		CFont::SetFontStyle(STYLED7_FONTSTYLE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetDropShadowPosition(STYLED7_SHADOW);
		if (STYLED7_OUTLINE != 0)
			CFont::SetOutlinePosition(STYLED7_OUTLINE);
		CFont::SetColor(CRGBA(STYLED7_R, STYLED7_G, STYLED7_B, 255));
		CFont::SetSlantRefPoint(0.0, 0.0);
		CFont::SetSlant(0.0);
		CFont::PrintString((float)(RsGlobal.maximumWidth / 2),
			SCREEN_COORD(STYLED7_Y), text.Get("DNC_002")); // ~y~PERFECT
	}

	static void DisplayRadioStationName()
	{
		CText text;
		CRGBA color1 = CRGBA(RS_RED1, RS_GREEN1, RS_BLUE1, 255);
		CRGBA color2 = CRGBA(RS_RED2, RS_GREEN2, RS_BLUE2, 255);
		DrawTestString((float)(RsGlobal.maximumWidth / 2), SCREEN_COORD(RS_POS_Y),
			RS_SIZE_X, RS_SIZE_Y, ALIGN_CENTER, RsGlobal.maximumWidth * 0.0015625f * 590.0f,
			RS_SHADOW, RS_OUTLINE, RS_FONTSTYLE, color1, text.Get("FEA_R0")); // Playback FM
	}

	static void CGarages__PrintMessages()
	{
		CText text;
		DrawTestString((float)(RsGlobal.maximumWidth / 2), SCREEN_COORD(GARAGES_Y),
			GARAGES_SIZE_X, GARAGES_SIZE_Y, ALIGN_CENTER,
			RsGlobal.maximumWidth * (1.0f - 0.0015625f * GARAGES_LINEWIDTH),
			GARAGES_SHADOW, GARAGES_OUTLINE, GARAGES_FONTSTYLE,
			CRGBA(GARAGES_R, GARAGES_G, GARAGES_B, 255), text.Get("GA_15")); // Hope you like the new color
	}

	static void DrawMissionTimers()
	{
		// Timer Counter
		CFont::SetScale(SCREEN_MULTIPLIER(M_TIMERS_SIZE_X), SCREEN_MULTIPLIER(M_TIMERS_SIZE_Y));
		CFont::SetAlignment(ALIGN_RIGHT);
		CFont::SetCentreSize(RsGlobal.maximumWidth * 0.0015625f * 590.0f);
		CFont::SetFontStyle(M_TIMERS_FONTSTYLE);
		CFont::SetDropShadowPosition(M_TIMERS_SHADOW);
		if (M_TIMERS_OUTLINE != 0)
			CFont::SetOutlinePosition(M_TIMERS_OUTLINE);
		CFont::SetColor(CRGBA(M_TIMERS_R, M_TIMERS_G, M_TIMERS_B, 255));
		CFont::SetSlantRefPoint(0.0, 0.0);
		CFont::SetSlant(0.0);
		CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(M_TIMER_X), SCREEN_COORD(M_TIMER_Y), "00:00");

		// Timer Desc
		CText text;
		CFont::SetScale(SCREEN_MULTIPLIER(M_TIMERS_SIZE_X), SCREEN_MULTIPLIER(M_TIMERS_SIZE_Y));
		CFont::SetAlignment(ALIGN_RIGHT);
		CFont::SetCentreSize(RsGlobal.maximumWidth * 0.0015625f * 590.0f);
		CFont::SetFontStyle(M_TIMERS_FONTSTYLE);
		CFont::SetDropShadowPosition(M_TIMERS_SHADOW);
		if (M_TIMERS_OUTLINE != 0)
			CFont::SetOutlinePosition(M_TIMERS_OUTLINE);
		CFont::SetColor(CRGBA(M_TIMERS_R, M_TIMERS_G, M_TIMERS_B, 255));
		CFont::SetSlantRefPoint(0.0, 0.0);
		CFont::SetSlant(0.0);
		CFont::PrintString(
			RsGlobal.maximumWidth - SCREEN_COORD(M_TIMER_DESC_X),
			SCREEN_COORD(M_TIMER_Y), text.Get("BB_19")); // Time

		std::string gxt[3] = { "ZER2_43", "BB_18", "TX_ADDS" };
		bool type[3] = { false, true, true };

		for (int i = 0; i < 3; i++) {
			float OFFSET_Y = (329.0f + (float)i*105.0f) * M_STATUSTEXT_SPACE;

			// Status Text Desc
			CFont::SetScale(SCREEN_MULTIPLIER(M_TIMERS_SIZE_X), SCREEN_MULTIPLIER(M_TIMERS_SIZE_Y));
			CFont::SetAlignment(ALIGN_RIGHT);
			CFont::SetCentreSize(RsGlobal.maximumWidth * 0.0015625f * 590.0f);
			CFont::SetFontStyle(M_TIMERS_FONTSTYLE);
			CFont::SetDropShadowPosition(M_TIMERS_SHADOW);
			if (M_TIMERS_OUTLINE != 0)
				CFont::SetOutlinePosition(M_TIMERS_OUTLINE);
			CFont::SetColor(CRGBA(M_STATUSBAR_R, M_STATUSBAR_G, M_STATUSBAR_B, 255));
			CFont::SetSlantRefPoint(0.0, 0.0);
			CFont::SetSlant(0.0);
			CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(M_STATUSTEXT_DESC_X),
				SCREEN_COORD(M_STATUSTEXT_Y + OFFSET_Y), text.Get((char*)gxt[i].c_str()));

			if (!type[i]) {
				// Status Text Value
				CFont::SetScale(SCREEN_MULTIPLIER(M_TIMERS_SIZE_X), SCREEN_MULTIPLIER(M_TIMERS_SIZE_Y));
				CFont::SetAlignment(ALIGN_RIGHT);
				CFont::SetCentreSize(RsGlobal.maximumWidth * 0.0015625f * 590.0f);
				CFont::SetFontStyle(M_TIMERS_FONTSTYLE);
				CFont::SetDropShadowPosition(M_TIMERS_SHADOW);
				if (M_TIMERS_OUTLINE != 0)
					CFont::SetOutlinePosition(M_TIMERS_OUTLINE);
				CFont::SetColor(CRGBA(M_STATUSBAR_R, M_STATUSBAR_G, M_STATUSBAR_B, 255));
				CFont::SetSlantRefPoint(0.0, 0.0);
				CFont::SetSlant(0.0);
				CFont::PrintString(
					RsGlobal.maximumWidth - SCREEN_COORD(M_STATUSTEXT_X),
					SCREEN_COORD(M_STATUSTEXT_Y + OFFSET_Y), "50");
			}
			else {
				// Status Text Progressbar
				CSprite2d::DrawBarChart(
					RsGlobal.maximumWidth - SCREEN_COORD(M_STATUSBAR_X),
					SCREEN_COORD(M_STATUSBAR_Y + OFFSET_Y + 14.0f*M_STATUSTEXT_SPACE),
					(unsigned short)SCREEN_MULTIPLIER(M_STATUSBAR_SIZE_X),
					(unsigned char)SCREEN_MULTIPLIER(M_STATUSBAR_SIZE_Y),
					(unsigned char)50.0, 0, 0, M_STATUSBAR_BORDER,
					CRGBA(M_STATUSBAR_R, M_STATUSBAR_G, M_STATUSBAR_B, M_STATUSBAR_A),
					CRGBA(0, 0, 0, M_STATUSBAR_A));
			}
		}
	}
}editHud;