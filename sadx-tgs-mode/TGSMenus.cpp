#include <SADXModLoader.h>
#include "TGSMenus.h"
#include "ScaleInfo.h";

using namespace uiscale;

// Patch to bypass TitleLoop2 return value check
unsigned char patch1[] = { 0xB8, 0xFF, 0xFF, 0xFF, 0xFF };

// Patch to prevent the game from quitting on pressing X+Y (Level Select)
unsigned char patch2[] = { 0x33, 0xC0, 0x90 };

// White fade fix (queue + blending)
void TGSFadeFixWhite(NJS_POINT2COL* a1, int a2, float a3, NJD_DRAW a4)
{
	njColorBlendingMode(NJD_SOURCE_COLOR, NJD_COLOR_BLENDING_SRCALPHA);
	njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_INVSRCALPHA);
	late_DrawPolygon2D(a1, a2, 40000.0f, NJD_TRANSPARENT | NJD_FILL, LATE_LIG);
}

// Black fade fix (queue + blending)
void TGSFadeFixBlack(float left, float top, float right, float bottom, float depth, int color, LATE queueflags)
{
	njColorBlendingMode(NJD_SOURCE_COLOR, NJD_COLOR_BLENDING_ZERO);
	njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_SRCALPHA);
	if (color != 0xFF000000) // SADX doesn't set the transparency attribute if alpha is FF, so it's rendered as opaque which is not needed here
		late_DrawBoxFill2D(left, top, right, bottom, 40000.0f, color, queueflags);
}

// Debug Level Select display function
void __cdecl DisplaySelectingStage_ss_r()
{
	unsigned int color1; // [sp+10h] [bp-4h]@2
	unsigned int color2; // ebp@2
	signed int loop_x; // ebx@4
	signed int loop_y; // edi@1
	signed int stage; // esi@4
	int stage_sel; // esi@5
	int stage_text; // ecx@8

	loop_y = 10;
	color1 = (TGSTitleMode == 10) ? 0xFF00FF00 : 0xFFDFDFDF;
	color2 = (TGSTitleMode == 10) ? 0xFFDFDFDF : 0xFF7F7F7F;
	njPrintColor(color2);
	njPrintC(0x100008, "STAGE SELECT");
	stage = scSelectedStage + 71;
	loop_x = 13;
	do
	{
		stage_sel = stage % 76;
		njPrintColor(stage_sel == scSelectedStage ? color1 : color2);
		stage_text = (stageact_tbl[stage_sel][0] << 3) | stageact_tbl[stage_sel][1];
		njPrintC(loop_y | 0x100000, LevelSelectText[stage_text]);
		stage = stage_sel + 1;
		++loop_y;
		--loop_x;
	} while (loop_x);
	njPrintColor(0xFFFFFFFF);
}

// Hack to prevent the game from quitting on X+Y
Sint32 MenuFix(NJS_TEXLIST* texlist)
{
	TGSTitleMode = 9;
	TGSLock = 1;
	return 1;
}

// Depth hack for the title screen
void TitleScreenFix(NJS_SPRITE* sp, Int n, Float pri, NJD_SPRITE attr, LATE queue_flags)
{
	if (tlFade > 12) // Prevent flicker when fading in after playing a demo
		late_DrawSprite2D(sp, n, 22000.0f, attr, queue_flags);
}

void TGSMenus_Init()
{
	// TGS features
	WriteData((char*)0x0040C51F, patch1); // Patch to bypass TitleLoop2 return value check
	WriteData((char*)0x0042ED9D, patch2); // Patch to prevent the game from quitting on pressing X+Y (Level Select)
	WriteJump((void*)0x0042DE70, DisplaySelectingStage_ss_r); // Display Level Select text
	WriteData((int*)0x0042E919, 900); // Make the title screen wait longer before going to the demo
	WriteCall((void*)0x0042ED81, MenuFix); // Patch to prevent the game from quitting on pressing X+Y (Level Select)
	// Other patches for Level Select
	WriteData<10>((char*)0x0042E0A5, 0x90u);
	WriteData<10>((char*)0x0042E187, 0x90u);
	WriteData<10>((char*)0x0042E289, 0x90u);
	WriteData<10>((char*)0x0042E399, 0x90u);
	// Menu speed fixes
	WriteData((float**)0x0042D931, (float*)0x007DD568); // Character select fade speed: subtract 0.1 instead of 0.2
	WriteData<1>((char*)0x0042E8F8, 0xCu); // Fade in speed (12 instead of 24)
	WriteData<1>((char*)0x0042E9F1, 0xCu); // Fade in speed
	WriteData<1>((char*)0x0042EA25, 0xCu); // Fade in speed
	WriteData<1>((char*)0x0042EA96, 0xCu); // Fade in speed
	WriteData<1>((char*)0x0042EAF6, 0xCu); // Fade in speed
	WriteData<1>((char*)0x0042EBAA, 0xCu); // Fade in speed
	WriteData<1>((char*)0x0042EBE8, 0xCu); // Fade in speed
	WriteData<1>((char*)0x0042EC75, 0xCu); // Fade in speed
	WriteData<1>((char*)0x0042ECD2, 0xCu); // Fade in speed
	WriteData<1>((char*)0x0042EEC6, 0xCu); // Fade in speed
	WriteData<1>((char*)0x0042EF3C, 0xCu); // Fade in speed
	// Rendering depth fixes
	WriteCall((void*)0x0042D8D2, TitleScreenFix);
	WriteCall((void*)0x0042E5DB, TGSFadeFixWhite); // White fade
	WriteCall((void*)0x0042E4D6, TGSFadeFixBlack); // Black fade
	WriteData((float*)0x0042E695, 28000.0f); // Loading
	WriteData((float*)0x0042E6B2, 27800.0f); // Loading fade 1
	WriteData((float*)0x0042E6E7, 27800.0f); // Loading fade 2
	WriteData((float*)0x0042E70C, 27800.0f); // Loading fade 3
	WriteData((float*)0x0042E741, 27800.0f); // Loading fade 4
	WriteData((float*)0x0042DBE3, 23800.0f); // Press Start
	// Disable demos to prevent crashes
	WriteData<1>((char*)0x00413CEF, 0i8);
}