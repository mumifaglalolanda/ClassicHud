#include "3DMarkers.h"

#include "CTimer.h"
#include "CModelInfo.h"
#include "CStreaming.h"
#include "CTxdStore.h"
#include "CScene.h"
#include "CCamera.h"
#include "CCoronas.h"

#include "Settings.h"

using namespace plugin;

RpClump *__cdecl CFileLoader__LoadAtomicFile2Return(char *filename)
{
	RpClump *v1; // edi
	RwStream *v2; // esi

	v1 = 0;
	v2 = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, filename);
	if (RwStreamFindChunk(v2, 16, 0, 0))
		v1 = RpClumpStreamRead(v2);
	RwStreamClose(v2, 0);
	return v1;
}

// Static variables
float		C3dMarkers::m_PosZMult;
const float	C3dMarkers::m_MovingMultiplier = 0.25f;

RpClump	**C3dMarkers::m_pRpClumpArray = (RpClump**)0xC7C6DC;
C3dMarker *C3dMarkers::m_aMarkerArray = (C3dMarker*)0xC7DD58;
int &C3dMarkers::NumActiveMarkers = *(int*)0xC7C6D8;
float &C3dMarkers::m_angleDiamond = *(float*)0xC7C700;
bool &C3dMarkers::IgnoreRenderLimit = *(bool*)0xC7C704;

void C3dMarkers::InitTextures()
{
	int slot = CTxdStore::AddTxdSlot("markers");
	CTxdStore::LoadTxd(slot, "models\\ClassicHud\\ingame.txd");
	CTxdStore::AddRef(slot);
	CTxdStore::PushCurrentTxd();
	CTxdStore::SetCurrentTxd(slot);
	CTxdStore::PopCurrentTxd();
}

void C3dMarkers::ShutDownTextures()
{
	int slot = CTxdStore::FindTxdSlot("markers");
	CTxdStore::RemoveTxdSlot(slot);
}

void
C3dMarkers::Init(void)
{
	int i;
	C3dMarker *marker;

	marker = m_aMarkerArray;
	for (i = 0; i < 32; i++) {
		marker->m_lastPosition = CVector(0, 0, 0);
		marker->m_pAtomic = NULL;
		marker->m_nType = 0x101;
		marker->m_bIsUsed = false;
		marker->m_bIsUsed2 = false;
		marker->m_nIdentifier = 0;
		marker->m_color.red = 255;
		marker->m_color.green = 255;
		marker->m_color.blue = 255;
		marker->m_color.alpha = 255;
		marker->m_nPulsePeriod = 1024;
		marker->m_nRotateRate = 5;
		marker->m_nStartTime = 0;
		marker->m_fPulseFraction = 0.25;
		marker->m_fStdSize = 1.0;
		marker->m_fSize = 1.0;
		marker->m_fBrightness = 1.0;
		marker->m_fCameraRange = 0.0;
		marker->m_normal = CVector(0, 0, 1);
		marker->m_LastMapReadX = 30000;
		marker->m_roofHeight = 65535.0;
		marker++;
	}
	memset(m_pRpClumpArray, 0, sizeof(RpClump*[7]));
	NumActiveMarkers = 0;
	m_angleDiamond = 0;

	RpClump* arrow_model = (settings.MARKERS_USE_ARROWS && (std::string)settings.MARKERS_ARROW_MODEL != "") ?
		CFileLoader__LoadAtomicFile2Return(settings.MARKERS_ARROW_MODEL) : LoadMarker("diamond_3");

	int slot;
	if ((std::string)settings.MARKERS_CYLINDER_MODEL != "")
		slot = CTxdStore::FindTxdSlot("markers");
	else slot = CTxdStore::FindTxdSlot("particle");
	CTxdStore::PushCurrentTxd();
	CTxdStore::SetCurrentTxd(slot);
	m_pRpClumpArray[1] = ((std::string)settings.MARKERS_CYLINDER_MODEL != "") ?
		CFileLoader__LoadAtomicFile2Return(settings.MARKERS_CYLINDER_MODEL) : LoadMarker("cylinder");
	m_pRpClumpArray[4] = LoadMarker("hoop");
	m_pRpClumpArray[0] = LoadMarker("diamond_3");
	m_pRpClumpArray[6] = m_pRpClumpArray[0];
	m_pRpClumpArray[5] = arrow_model;
	CTxdStore::PopCurrentTxd();
}

void
C3dMarkers::Render(void)
{
	int alphafunc;
	C3dMarker *marker;
	static RwRGBAReal markerAmbient = { 0.0, 0.0, 0.0, 0.0 };
	static RwRGBAReal markerDirectional = { 0.0, 0.0, 0.0, 0.0 };

	RwRenderStateGet(rwRENDERSTATEALPHATESTFUNCTION, &alphafunc);
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)rwALPHATESTFUNCTIONALWAYS);
	NumActiveMarkers = 0;
	ActivateDirectional();
	SetAmbientColours(&markerAmbient);
	SetDirectionalColours(&markerDirectional);
	User3dMarkersDraw();
	for (marker = m_aMarkerArray; marker < &m_aMarkerArray[32]; marker++) {
		if (marker->m_bIsUsed2) {
			if (marker->m_fCameraRange < 150.0f || IgnoreRenderLimit || marker->m_nType == MARKER_HOOP) {
				marker->Render();
				if (marker->m_nType == MARKER_ARROW && settings.MARKERS_CONE_GLOW)
					CCoronas::RegisterCorona((uint32)marker, NULL,
						marker->m_color.red, marker->m_color.green, marker->m_color.blue, 0x84,
						marker->m_mat.pos, 1.2f*marker->m_fSize, TheCamera.m_fLODDistMultiplier * 50.0f,
						gpCoronaTexture[1], (eCoronaFlareType)0, 0, 0, 0, 0.0,
						false, 1.5f, false, 255, false, true);
			}
			NumActiveMarkers++;
			marker->m_bIsUsed = false;
			marker->m_bIsUsed2 = false;
		}
		else {
			if (marker->m_pAtomic) {
				marker->m_nIdentifier = 0;
				marker->m_nStartTime = 0;
				marker->m_bIsUsed = false;
				marker->m_bIsUsed2 = false;
				marker->m_nType = 0x101;
				RwFrame *f = (RwFrame *)marker->m_pAtomic->object.object.parent;
				RpAtomicDestroy(marker->m_pAtomic);
				RwFrameDestroy(f);
				marker->m_pAtomic = NULL;
			}
		}
	}
	DirectionArrowsDraw();
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)alphafunc);
}

WRAPPER RpClump *C3dMarkers::LoadMarker(const char *name) { WRAPARG(name); EAXJMP(0x722810); }

WRAPPER void C3dMarkers::User3dMarkersDraw(void) { EAXJMP(0x723240); }
WRAPPER void C3dMarkers::DirectionArrowsDraw(void) { EAXJMP(0x7211F0); }

WRAPPER C3dMarker* C3dMarkers::PlaceMarker(unsigned int nIndex, unsigned short markerID, CVector& vecPos, float fSize, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned short pulsePeriod, float pulseFraction, short rotateRate, float normalX, float normalY, float normalZ, bool checkZ)
{
	WRAPARG(nIndex); WRAPARG(markerID); WRAPARG(vecPos); WRAPARG(fSize); WRAPARG(red); WRAPARG(green); WRAPARG(blue); WRAPARG(alpha);
	WRAPARG(pulsePeriod); WRAPARG(pulseFraction); WRAPARG(rotateRate); WRAPARG(normalX); WRAPARG(normalY); WRAPARG(normalZ);
	WRAPARG(checkZ); EAXJMP(0x725120);
}

void C3dMarkers::PlaceMarkerSet(unsigned int nIndex, unsigned short markerID, CVector& vecPos, float fSize, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned short pulsePeriod, float pulseFraction)
{
	red = settings.MARKERS_R, green = settings.MARKERS_G, blue = settings.MARKERS_B;
	PlaceMarker(nIndex, markerID, vecPos, fSize, red, green, blue, static_cast<unsigned char>(alpha * (1.0f / 3.0f)), pulsePeriod, pulseFraction, 1, 0.0, 0.0, 0.0, false);
	PlaceMarker(nIndex, markerID, vecPos, fSize * 0.9f, red, green, blue, static_cast<unsigned char>(alpha * (1.0f / 3.0f)), pulsePeriod, pulseFraction, -1, 0.0, 0.0, 0.0, false);
}

void C3dMarkers::PlaceMarkerCone(int id, CVector *posn, float size, char r, char g, char b, int alpha, __int16 pulsePeriod, float pulseFraction, int type, char bEnableCollision)
{
	r = settings.ENEX_R, g = settings.ENEX_G, b = settings.ENEX_B;
	//r = 0, g = 0x64, b = 0x96; // VCS colors

	CVector m_Pos;
	m_Pos.x = posn->x - TheCamera.GetPosition().x;
	m_Pos.y = posn->y - TheCamera.GetPosition().y;
	m_Pos.z = posn->z - TheCamera.GetPosition().z;

	if (sqrt(m_Pos.z * m_Pos.z + m_Pos.x * m_Pos.x + m_Pos.y * m_Pos.y) >= 1.6)
	{
		if (bEnableCollision)
			C3dMarkers::PlaceMarker(id, 5, *posn, size, r, g, b, 255, pulsePeriod, pulseFraction, 0, 0.0f, 0.0f, 0.0f, 0);
		else
			C3dMarkers::PlaceMarker(id, 6, *posn, size, r, g, b, 255, pulsePeriod, pulseFraction, 0, 0.0f, 0.0f, 0.0f, 0);
	}
}

long double C3dMarker::CalculateRealSize()
{
	long double		fVariable = (((m_nPulsePeriod - 1)) & (CTimer::m_snTimeInMilliseconds /*- m_nStartTime*/));
	long double		res = (2 * M_PI) * fVariable / static_cast<long double>(m_nPulsePeriod);
	static float m_angleDiamond = *(float*)0xC7C700;
	m_angleDiamond = (float)res;
	return res;
}

void
C3dMarker::Render(void)
{
	if (m_pAtomic == NULL)
		return;
	if (m_nType == 1)
		RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
	else
		RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
	RpMaterialSetColor(m_pMaterial, &m_color);
	m_mat.UpdateRW();
	CMatrix mat(m_mat.m_pAttachMatrix, true);
	mat.Scale(m_fSize);
	mat.UpdateRW();
	RwFrameUpdateObjects(RpClumpGetFrame(m_pAtomic));
	SetBrightMarkerColours(0.0f);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
	if (m_nType == 3)
		RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
	m_pAtomic->renderCallBack(m_pAtomic);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
	ReSetAmbientAndDirectionalColours();
}

void __declspec(naked) C3dMarkerSizeHack()
{
	_asm
	{
		push	eax		// Not keeping it causes marker to flicker
		mov		ecx, esi
		call	C3dMarker::CalculateRealSize
		fst		C3dMarkers::m_PosZMult
		pop		eax
		retn
	}
}

void __fastcall C3dMarker_Render(C3dMarker* pMarker, int edx0)
{
	pMarker->Render();
}

void C3dMarkers::InstallPatches()
{
	patch::RedirectCall(0x7269FA, C3dMarkers::Init);
	patch::RedirectJump(0x7250B1, C3dMarker_Render);//patch::RedirectCall(0x7250B1, C3dMarkers::Render);//patch::RedirectCall(0x7250B1, &C3dMarker::Render);
	patch::RedirectCall(0x726AE4, C3dMarkers::Render);
	*(uint8*)0x8D5D8B = settings.MARKERS_CONE_ALPHA;
	
	// some radar color
	patch::Set<BYTE>(0x585CCB, settings.MARKERS_B);
	patch::Set<BYTE>(0x585CCD, settings.MARKERS_G);
	patch::Set<BYTE>(0x585CCF, settings.MARKERS_R);

	// Growing/shrinking 3DMarkers
	if (settings.MARKERS_SIZE_CHANGE) {
		patch::Set<float>(0x440F26, 0.0f);
		patch::RedirectCall(0x72576B, &C3dMarkerSizeHack);
		patch::Nop(0x725770, 1);
	}
	else
	{
		patch::Set<BYTE>(0x72576B, 0xDD);
		patch::Set<BYTE>(0x72576B + 1, 0x05);
		patch::Set<const float*>(0x72576B + 2, (float*)0x859EF8);
	}

	// New style of markers
	// What is this?
	patch::RedirectJump(0x725BA0, &C3dMarkers::PlaceMarkerSet);
	
	// Enex markers RGB
	patch::RedirectCall(0x440F4E, C3dMarkers::PlaceMarkerCone);

	// arrow.dff as marker
	if (settings.MARKERS_SIZE_CHANGE) {
		patch::Set<const float*>(0x725636, C3dMarkers::GetPosZMult());
		patch::Set<const float*>(0x7259A1, C3dMarkers::GetPosZMult());
		//Patch<const float*>(0x7232C7, C3dMarkers::GetPosZMult());
		patch::Set<const float*>(0x72564B, C3dMarkers::GetMovingMult());
		patch::Set<const float*>(0x7259A9, C3dMarkers::GetMovingMult());
		patch::Nop(0x72563A, 6);
		patch::Nop(0x72599F, 6);
		patch::Nop(0x72502B, 6);
		patch::Nop(0x725647, 2);
	}
	else {
		patch::Set<const float*>(0x725636, &C3dMarkers::m_angleDiamond);
		patch::Set<const float*>(0x7259A1, &C3dMarkers::m_angleDiamond);
		patch::Set<const float*>(0x72564B, (float*)0x858FCC);
		patch::Set<const float*>(0x7259A9, (float*)0x858C24);

		patch::Set<BYTE>(0x72563A, 0xD8);
		patch::Set<BYTE>(0x72563A + 1, 0x0D);
		patch::Set<const float*>(0x72563A + 2, (float*)0x8595EC);

		patch::Set<BYTE>(0x72599F, 0xD8);
		patch::Set<BYTE>(0x72599F + 1, 0x0D);
		patch::Set<const float*>(0x72599F + 2, (float*)0x8595EC);

		patch::Set<BYTE>(0x72502B, 0x89);
		patch::Set<BYTE>(0x72502B + 1, 0x35);
		patch::Set<const float*>(0x72502B + 2, (float*)0xC7C6F0);

		patch::Set<BYTE>(0x725647, 0x7A);
		patch::Set<BYTE>(0x725647 + 1, 0x08);
	}
	if (settings.MARKERS_ROTATE)
		patch::Set<uint8>(0x726DA6, 5);	// arrow (old cone) rotate rate
	else
		patch::Set<uint8>(0x726DA6, 0);
	patch::SetPointer(0x7232C1, &C3dMarkers::m_pRpClumpArray[0]);	// marker 0 (user marker)
	
}