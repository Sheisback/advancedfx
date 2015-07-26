#include "stdafx.h"

// Copyright (c) advancedfx.org
//
// Last changes:
// 2015-07-25 dominik.matrixstorm.com
//
// First changes:
// 2015-07-25 dominik.matrixstorm.com

#include "csgo_CSkyBoxView.h"

#include "addresses.h"

#include <shared/detours.h>

typedef void (__stdcall *csgo_CSkyBoxView_Draw_t)(DWORD *this_ptr);

csgo_CSkyBoxView_Draw_t detoured_csgo_CSkyBoxView_Draw;

bool g_bIn_CSkyBoxView_Draw = false;

void __stdcall touring_csgo_CSkyBoxView_Draw(DWORD *this_ptr)
{
	g_bIn_CSkyBoxView_Draw = true;

	detoured_csgo_CSkyBoxView_Draw(this_ptr);
	
	g_bIn_CSkyBoxView_Draw = false;
}

bool csgo_CSkyBoxView_Draw_Install(void)
{
	static bool firstResult = false;
	static bool firstRun = true;
	if(!firstRun) return firstResult;
	firstRun = false;

	if(AFXADDR_GET(csgo_CHudDeathNotice_FireGameEvent))
	{
		detoured_csgo_CSkyBoxView_Draw = (csgo_CSkyBoxView_Draw_t)DetourClassFunc((BYTE *)AFXADDR_GET(csgo_CSkyboxView_Draw), (BYTE *)touring_csgo_CSkyBoxView_Draw, (int)AFXADDR_GET(csgo_CSkyboxView_Draw_DSZ));

		firstResult = true;
	}

	return firstResult;
}
