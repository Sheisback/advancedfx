#include "stdafx.h"

// Copyright (c) advancedfx.org
//
// Last changes:
// 2016-11-06 dominik.matrixstorm.com
//
// First changes:
// 2016-11-06 dominik.matrixstorm.com

#include "csgo_CCSGameMovement.h"

#include "addresses.h"

#include <shared/detours.h>

bool g_Enable_csgo_CCSGameMovement_DuckFix = false;


//:009
typedef void(__stdcall * csgo_CCSGameMovement_DuckShit_t)(
	DWORD *this_ptr);

csgo_CCSGameMovement_DuckShit_t detoured_csgo_CCSGameMovement_DuckShit;

void __stdcall csgo_CCSGameMovement_DuckShit(
	DWORD *this_ptr)
{
	if (!g_Enable_csgo_CCSGameMovement_DuckFix)
		detoured_csgo_CCSGameMovement_DuckShit(this_ptr);
}

bool Hook_csgo_CCSGameMovement_DuckFix(void)
{
	static bool firstResult = false;
	static bool firstRun = true;
	if (!firstRun) return firstResult;
	firstRun = false;

	if (AFXADDR_GET(csgo_CCSGameMovement_vtable))
	{
		DWORD * vtable = (DWORD *)(AFXADDR_GET(csgo_CCSGameMovement_vtable));

		detoured_csgo_CCSGameMovement_DuckShit = (csgo_CCSGameMovement_DuckShit_t)DetourIfacePtr((DWORD *)&(vtable[56]), csgo_CCSGameMovement_DuckShit);

		firstResult = true;
	}

	return firstResult;
}
