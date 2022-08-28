#include <SADXModLoader.h>
#include "NowLoading.h"
#include "ScaleInfo.h";
#include <vector>
using namespace uiscale;

TEX_PVMTABLE NowLoadingPVMEntry = { "LOADING", &texlist_loading };

// Draws "Now Loading" with SA1 logo instead of regular black fade
void Loading()
{
	gHelperFunctions->PushScaleUI(Align::Align_Bottom, false, 1.0f, 1.0f);
	Uint8 v0; // bl
	float right; // [esp+0h] [ebp-18h]
	float bottom; // [esp+4h] [ebp-14h]
	float r; // [esp+0h] [ebp-38h]
	NJS_SPRITE _sp; // [esp+18h] [ebp-20h] BYREF

	v0 = usFadeLevel;
	if (usFadeLevel >= 255u)
	{
		v0 = 255;
	}
	njSetZCompare(7u);
	njColorBlendingMode(NJD_SOURCE_COLOR, NJD_COLOR_BLENDING_SRCALPHA);
	njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_INVSRCALPHA);
	r = v0 * 0.0039215689f;
	SetMaterial(r, 1.0f, 1.0f, 1.0f);
	_sp.tlist = &texlist_loading;
	_sp.tanim = &anim_fade_0;
	_sp.sx = 1.0f;
	_sp.sy = 1.0f;
	_sp.p.x = 320.0f;
	_sp.p.y = 240.0f;
	for (int i = 0; i < 2; i++)
	{
		late_DrawSprite2D(&_sp, i, 22047.0, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR, LATE_LIG);
	}
	_sp.sx = 24.0;
	_sp.sy = 60.0;
	_sp.p.x = 192.0;
	_sp.p.y = 240.0;
	late_DrawSprite2D(&_sp, 2, 22046.99, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR, LATE_LIG);
	_sp.p.x = 448.0;
	_sp.p.y = 240.0;
	late_DrawSprite2D(&_sp, 3, 22046.99, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR, LATE_LIG);
	_sp.sx = 32.0;
	_sp.sy = 22.0;
	_sp.p.x = 320.0;
	_sp.p.y = 176.0;
	late_DrawSprite2D(&_sp, 4, 22046.99, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR, LATE_LIG);
	_sp.p.x = 320.0;
	_sp.p.y = 304.0;
	late_DrawSprite2D(&_sp, 5, 22046.99, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR, LATE_LIG);
	ResetMaterial();
	gHelperFunctions->PopScaleUI();
	njSetZCompare(1u);
}

// Expands an array of TEX_PVMTable, useful for level PVM lists
TEX_PVMTABLE* ExpandPVMList(TEX_PVMTABLE* sourcepvmlist, const TEX_PVMTABLE& newpvmentry)
{
	std::vector<TEX_PVMTABLE> pvmlistvector;

	// Add original and new data to vector
	const TEX_PVMTABLE* oldlist = &sourcepvmlist[0];
	for (; oldlist->ptexlist != nullptr; oldlist++)
	{
		pvmlistvector.push_back(*oldlist);
	}
	pvmlistvector.push_back(newpvmentry);
	// Create a new PVM Entry list
	auto size = pvmlistvector.size();
	auto newlist = new TEX_PVMTABLE[size + 1];
	memcpy(newlist, pvmlistvector.data(), sizeof(TEX_PVMTABLE) * size);
	newlist[size].ptexlist = nullptr;
	return newlist;
}

// Hook to load the LOADING pvm
void LoadPVMHook()
{
	texLoadTexturePvmFile("LOADING", &texlist_loading);
}

// Another hook to load the LOADING pvm
void LoadLoading(const char* filename, NJS_TEXLIST* texlist)
{
	texLoadTexturePvmFile(filename, texlist);
	texLoadTexturePvmFile("LOADING", &texlist_loading);
}

// Force draw "Now Loading" with maximum opacity
void LoadingMissingFix(float left, float top, float right, float bottom, float depth, int color)
{
	gHelperFunctions->PushScaleUI(Align::Align_Bottom, false, 1.0f, 1.0f);
	NJS_SPRITE _sp; // [esp+18h] [ebp-20h] BYREF

	njSetZCompare(7u);
	//ds_DrawBoxFill2D(0.0, 0.0, right, bottom, -1.0, 0xFF000000);
	njColorBlendingMode(NJD_SOURCE_COLOR, NJD_COLOR_BLENDING_SRCALPHA);
	njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_INVSRCALPHA);
	SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
	_sp.tlist = &texlist_loading;
	_sp.tanim = &anim_fade_0;
	_sp.sx = 1.0f;
	_sp.sy = 1.0f;
	_sp.p.x = 320.0f;
	_sp.p.y = 240.0f;
	njSetZCompare(3u);
	for (int i = 0; i < 2; i++)
	{
		dsDrawSprite2D(&_sp, i, -1.0f, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR);
	}
	_sp.sx = 24.0;
	_sp.sy = 60.0;
	_sp.p.x = 192.0;
	_sp.p.y = 240.0;
	dsDrawSprite2D(&_sp, 2, -1.0f, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR);
	_sp.p.x = 448.0;
	_sp.p.y = 240.0;
	dsDrawSprite2D(&_sp, 3, -1.0f, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR);
	_sp.sx = 32.0;
	_sp.sy = 22.0;
	_sp.p.x = 320.0;
	_sp.p.y = 176.0;
	dsDrawSprite2D(&_sp, 4, -1.0f, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR);
	_sp.p.x = 320.0;
	_sp.p.y = 304.0;
	dsDrawSprite2D(&_sp, 5, -1.0f, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR);
	ResetMaterial();
	gHelperFunctions->PopScaleUI();
	npSetZCompare();
}

// Replace the event fade with "Now Loading"
void EVFade_r(task* a1)
{
	taskwk* twp = a1->twp;
	usFadeLevel = (__int16)twp->value.f;
	Loading();
}

// Replace the empty background with "Now Loading" when screen fade is supposed to be drawn
void BackgroundHack(NJS_POINT2COL* p, Int n, Float pri, Uint32 attr)
{
	if (usMainFadeStatus != 0)
		LoadingMissingFix(0, 0, 0, 0, 0, 0);
	else
		njDrawPolygon2D(p, n, pri, attr);
}

// Replace the level title card
Sint32 __cdecl StageNameMode_r()
{
	_BOOL1 v0; // cc
	Sint32 result; // eax
	float v2; // [esp+0h] [ebp-Ch]
	float a2; // [esp+0h] [ebp-Ch]
	float a2a; // [esp+0h] [ebp-Ch]
	float v5; // [esp+4h] [ebp-8h]
	float a3; // [esp+4h] [ebp-8h]
	float a3a; // [esp+4h] [ebp-8h]

	v0 = StageNameWork.timer++ <= StageNameWork.DispTime;
	if (!v0 && !now_saving)
	{
		late_ReleaseTexture(StageNameWork.texlist);
		result = 1;
		pause_flg = 1;
		return result;
	}
	LoadingMissingFix(0, 0, 0, 0, 0, 0);
	return 1;
}

void NowLoading_Init()
{
	WriteJump((void*)0x0047E170, StageNameMode_r);
	WriteCall((void*)0x0078B922, BackgroundHack);
	WriteJump((void*)0x00412F70, EVFade_r);
	WriteCall((void*)0x0040901C, LoadingMissingFix);
	WriteCall((void*)0x0040901C, LoadingMissingFix);
	ListofFixedPvmList[0] = ExpandPVMList(ListofFixedPvmList[0], NowLoadingPVMEntry);
	ListofFixedPvmList[1] = ExpandPVMList(ListofFixedPvmList[0], NowLoadingPVMEntry);
	ListofFixedPvmList[2] = ExpandPVMList(ListofFixedPvmList[2], NowLoadingPVMEntry); // For Sky Chase
	WriteCall((void*)0x006421C7, LoadPVMHook);
	WriteCall((void*)0x00642711, LoadPVMHook);
	WriteCall((void*)0x415A90, LoadLoading);
	WriteJump(ScreenFade_Draw, Loading);
}