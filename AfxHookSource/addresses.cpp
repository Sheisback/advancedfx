#include "stdafx.h"

// Copyright (c) advancedfx.org
//
// Last changes:
// 2016-06-26 dominik.matrixstorm.com
//
// First changes:
// 2010-09-27 dominik.matrixstorm.com

#include "addresses.h"

#include <shared/binutils.h>

using namespace Afx::BinUtils;

AFXADDR_DEF(csgo_C_BaseEntity_ToolRecordEnties)
AFXADDR_DEF(csgo_C_BaseEntity_ToolRecordEnties_DSZ)
AFXADDR_DEF(csgo_C_BasePlayer_OFS_m_skybox3d_scale)
AFXADDR_DEF(csgo_CUnknown_GetPlayerName)
AFXADDR_DEF(csgo_CUnknown_GetPlayerName_DSZ)
AFXADDR_DEF(csgo_CHudDeathNotice_FireGameEvent)
AFXADDR_DEF(csgo_CHudDeathNotice_FireGameEvent_DSZ)
AFXADDR_DEF(csgo_CHudDeathNotice_UnkAddDeathNotice)
AFXADDR_DEF(csgo_CHudDeathNotice_UnkAddDeathNotice_DSZ)
AFXADDR_DEF(csgo_CHudDeathNotice_UnkAddDeathNotice_AddMovie_AfterModTime)
//AFXADDR_DEF(csgo_CScaleformSlotInitControllerClientImpl_UnkCheckSwf)
//AFXADDR_DEF(csgo_CScaleformSlotInitControllerClientImpl_UnkCheckSwf_DSZ)
AFXADDR_DEF(csgo_CSkyboxView_Draw)
AFXADDR_DEF(csgo_CSkyboxView_Draw_DSZ)
//AFXADDR_DEF(csgo_CViewRender_Render)
//AFXADDR_DEF(csgo_CViewRender_Render_DSZ)
AFXADDR_DEF(csgo_CViewRender_RenderView_AfterVGui_DrawHud)
AFXADDR_DEF(csgo_CViewRender_RenderSmokeOverlay_OnStoreAlpha)
AFXADDR_DEF(csgo_DS_CanRecord_ConsoleOpenCall)
AFXADDR_DEF(csgo_SplineRope_CShader_vtable)
AFXADDR_DEF(csgo_Spritecard_CShader_vtable)
AFXADDR_DEF(csgo_UnlitGeneric_CShader_vtable)
AFXADDR_DEF(csgo_VertexLitGeneric_CShader_vtable)
AFXADDR_DEF(csgo_S_StartSound_StringConversion)
AFXADDR_DEF(csgo_Scaleformui_CUnkown_Loader)
AFXADDR_DEF(csgo_Scaleformui_CUnkown_Loader_DSZ)
AFXADDR_DEF(csgo_pLocalPlayer)
AFXADDR_DEF(csgo_gpGlobals_OFS_curtime)
AFXADDR_DEF(csgo_gpGlobals_OFS_interpolation_amount)
AFXADDR_DEF(csgo_gpGlobals_OFS_interval_per_tick)
AFXADDR_DEF(csgo_snd_mix_timescale_patch)
AFXADDR_DEF(csgo_snd_mix_timescale_patch_DSZ)
AFXADDR_DEF(csgo_view)
AFXADDR_DEF(csgo_writeWaveConsoleOpenJNZ)
AFXADDR_DEF(cstrike_gpGlobals_OFS_absoluteframetime)
AFXADDR_DEF(cstrike_gpGlobals_OFS_curtime)
AFXADDR_DEF(cstrike_gpGlobals_OFS_interpolation_amount)
AFXADDR_DEF(cstrike_gpGlobals_OFS_interval_per_tick)

void ErrorBox(char const * messageText);

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define MkErrStr(file,line) "Problem in " file ":" STRINGIZE(line)

void Addresses_InitEngineDll(AfxAddr engineDll, bool isCsgo)
{
	if(isCsgo)
	{
		// csgo_snd_mix_timescale_patch:
		{
			DWORD addr = 0;
			DWORD strAddr = 0;
			{
				ImageSectionsReader sections((HMODULE)engineDll);
				if(!sections.Eof())
				{
					sections.Next(); // skip .text
					if(!sections.Eof())
					{
						MemRange result = FindCString(sections.GetMemRange(), "Start profiling MIX_PaintChannels\n");
						if(!result.IsEmpty())
						{
							strAddr = result.Start;
						}
						else ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(strAddr)
			{
				ImageSectionsReader sections((HMODULE)engineDll);
			
				MemRange baseRange = sections.GetMemRange();
				MemRange result = FindBytes(baseRange, (char const *)&strAddr, sizeof(strAddr));
				if(!result.IsEmpty())
				{
					DWORD tempAddr = result.Start +0xB9;

					MemRange result = FindPatternString(MemRange(tempAddr-3, tempAddr-3+3), "51 52 E8");
					if (!result.IsEmpty())
					{
						tempAddr = tempAddr + 4 + *(DWORD *)tempAddr;
						// in MIX_PaintChannels now.

						tempAddr = tempAddr + 0x293;

						MemRange result = FindPatternString(MemRange(tempAddr - 7, tempAddr - 7 + 7), "8D 8D 50 FE FF FF E8");
						if (!result.IsEmpty())
						{
							tempAddr = tempAddr + 4 + *(DWORD *)tempAddr;
							// In SoundMixFunction2 now.

							tempAddr = tempAddr + 0xED;

							MemRange result = FindPatternString(MemRange(tempAddr - 5, tempAddr - 5 + 5), "33 D2 8B CF E8");
							if (!result.IsEmpty())
							{
								tempAddr = tempAddr + 4 + *(DWORD *)tempAddr;
								// In SoundMixFunction now.

								tempAddr = tempAddr + 0x66;

								MemRange result = FindPatternString(MemRange(tempAddr - 2, tempAddr - 2 + 2), "FF D0");
								if (!result.IsEmpty())
								{
									// After call VEnglineClient013::GetTimeScale now.

									addr = tempAddr;
								}
							}
						}
					}
					else ErrorBox(MkErrStr(__FILE__, __LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(addr)
			{
				AFXADDR_SET(csgo_snd_mix_timescale_patch, addr);
			}
			else
			{
				AFXADDR_SET(csgo_snd_mix_timescale_patch, 0x0);
			}
		}

		// csgo_S_StartSound_StringConversion:
		{
			DWORD addr = 0;
			DWORD strAddr = 0;
			{
				ImageSectionsReader sections((HMODULE)engineDll);
				if(!sections.Eof())
				{
					sections.Next(); // skip .text
					if(!sections.Eof())
					{
						MemRange result = FindCString(sections.GetMemRange(), "Starting sound '%s' while system disabled.\n");
						if(!result.IsEmpty())
						{
							strAddr = result.Start;
						}
						else ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(strAddr)
			{
				ImageSectionsReader sections((HMODULE)engineDll);
			
				MemRange baseRange = sections.GetMemRange();
				MemRange result = FindBytes(baseRange, (char const *)&strAddr, sizeof(strAddr));
				if(!result.IsEmpty())
				{
					addr = result.Start -0x1d;

					// check for pattern to see if it is the right address:
					unsigned char pattern[14] = { 0x8B, 0x01, 0x8D, 0x54, 0x24, 0x78, 0x68, 0x04, 0x01, 0x00, 0x00, 0x52, 0xFF, 0x10 };

					DWORD patternSize = sizeof(pattern)/sizeof(pattern[0]);
					MemRange patternRange(addr, addr+patternSize);
					MemRange result = FindBytes(patternRange, (char *)pattern, patternSize);
					if(result.Start != patternRange.Start || result.End != patternRange.End)
					{
						addr = 0;
						ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(addr)
			{
				AFXADDR_SET(csgo_S_StartSound_StringConversion, addr);
			}
			else
			{
				AFXADDR_SET(csgo_S_StartSound_StringConversion, 0x0);
			}
		}

		// csgo_writeWaveConsoleOpenJNZ:
		//
		// The pattern seached is the check for the console being open at the beginning of a function
		// that calls the function to write the WAV file (in case startmovie was told to do so) and
		// that WAV write function refrences the ".WAV" string the _2nd_ time.
		// So to get the function in IDA search for second usage of ".WAV" and search for
		// refrence of that function.
		{
			DWORD addr = 0;
			{
				ImageSectionsReader sections((HMODULE)engineDll);
				if (!sections.Eof())
				{
					MemRange result = FindPatternString(sections.GetMemRange(), "55 8B EC 51 80 3D ?? ?? ?? ?? 00 56 57 0F 84 ?? ?? ?? ?? A1 ?? ?? ?? ?? B9 ?? ?? ?? ?? 8b 40 48 FF D0 84 C0 0F 85 ?? ?? ?? ??");
					if (!result.IsEmpty())
					{
						addr = result.End - 6;
					}
					else ErrorBox(MkErrStr(__FILE__, __LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__, __LINE__));
			}
			if (addr)
			{
				AFXADDR_SET(csgo_writeWaveConsoleOpenJNZ, addr);
			}
			else
			{
				AFXADDR_SET(csgo_writeWaveConsoleOpenJNZ, 0x0);
			}
		}

		// csgo_DS_CanRecord_ConsoleOpenCall:
		//
		// This is inside a function that is called for DirectSound (DS) before
		// the function that calls csgo_writeWaveConsoleOpenJNZ is called.
		// It checks if a recording name has been set and if the console is open
		// and a few other things.
		// We match a pattern longer a bit in order to be able to tell it apart from
		// a similar inlined version for screenshots, that doesn't check the time /
		// frame / whatever count as our function does.
		{
			DWORD addr = 0;
			{
				ImageSectionsReader sections((HMODULE)engineDll);
				if (!sections.Eof())
				{
					MemRange result = FindPatternString(sections.GetMemRange(), "80 3D ?? ?? ?? ?? 00 74 ?? A1 ?? ?? ?? ?? B9 ?? ?? ?? ?? 8B 40 48 FF D0 84 C0 75 ?? A1 ?? ?? ?? ?? 3B 05 ?? ?? ?? ??");
					if (!result.IsEmpty())
					{
						addr = result.End - 17;
					}
					else ErrorBox(MkErrStr(__FILE__, __LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__, __LINE__));
			}
			if (addr)
			{
				AFXADDR_SET(csgo_DS_CanRecord_ConsoleOpenCall, addr);
			}
			else
			{
				AFXADDR_SET(csgo_DS_CanRecord_ConsoleOpenCall, 0x0);
			}
		}

	}
	else
	{
		AFXADDR_SET(csgo_snd_mix_timescale_patch, 0x0);
		AFXADDR_SET(csgo_S_StartSound_StringConversion, 0x0);
		AFXADDR_SET(csgo_writeWaveConsoleOpenJNZ, 0x0);
		AFXADDR_SET(csgo_DS_CanRecord_ConsoleOpenCall, 0x0);
	}
	AFXADDR_SET(csgo_snd_mix_timescale_patch_DSZ, 0x09);
}

void Addresses_InitScaleformuiDll(AfxAddr scaleformuiDll, bool isCsgo)
{
	if(isCsgo)
	{
		// csgo_Scaleformui_CUnkown_Loader:
		{
			DWORD addr = 0;
			DWORD strAddr = 0;
			{
				ImageSectionsReader sections((HMODULE)scaleformuiDll);
				if(!sections.Eof())
				{
					sections.Next(); // skip .text
					if(!sections.Eof())
					{
						MemRange result = FindCString(sections.GetMemRange(), "Loader failed to open '%s', FileOpener not installe");
						if(!result.IsEmpty())
						{
							strAddr = result.Start;
						}
						else ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(strAddr)
			{
				ImageSectionsReader sections((HMODULE)scaleformuiDll);
			
				MemRange baseRange = sections.GetMemRange();
				MemRange result = FindBytes(baseRange, (char const *)&strAddr, sizeof(strAddr));
				if(!result.IsEmpty())
				{
					addr = result.Start -0x26;

					// check for pattern to see if it is the right address:
					unsigned char pattern[3] = { 0x55, 0x8B, 0xEC };

					DWORD patternSize = sizeof(pattern)/sizeof(pattern[0]);
					MemRange patternRange(addr, addr+patternSize);
					MemRange result = FindBytes(patternRange, (char *)pattern, patternSize);
					if(result.Start != patternRange.Start || result.End != patternRange.End)
					{
						addr = 0;
						ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(addr)
			{
				AFXADDR_SET(csgo_Scaleformui_CUnkown_Loader, addr);
			}
			else
			{
				AFXADDR_SET(csgo_Scaleformui_CUnkown_Loader, 0x0);
			}
		}
	}
	else
	{
		AFXADDR_SET(csgo_Scaleformui_CUnkown_Loader, 0x0);
	}

	AFXADDR_SET(csgo_Scaleformui_CUnkown_Loader_DSZ, 0x9);
}

void Addresses_InitClientDll(AfxAddr clientDll, bool isCsgo)
{
	if(isCsgo)
	{
		// csgo_CUnknown_GetPlayerName:
		{
			DWORD addr = 0;
			DWORD strAddr = 0;
			{
				ImageSectionsReader sections((HMODULE)clientDll);
				if(!sections.Eof())
				{
					sections.Next(); // skip .text
					if(!sections.Eof())
					{
						MemRange result = FindCString(sections.GetMemRange(), "#SFUI_bot_controlled_by");
						if(!result.IsEmpty())
						{
							strAddr = result.Start;
						}
						else ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(strAddr)
			{
				ImageSectionsReader sections((HMODULE)clientDll);
			
				MemRange baseRange = sections.GetMemRange();
				MemRange result = FindBytes(baseRange, (char const *)&strAddr, sizeof(strAddr));
				if(!result.IsEmpty())
				{
					addr = result.Start -0x396;

					// check for pattern to see if it is the right address:
					unsigned char pattern[3] = { 0x55, 0x8B, 0xEC };

					DWORD patternSize = sizeof(pattern)/sizeof(pattern[0]);
					MemRange patternRange(addr, addr+patternSize);
					MemRange result = FindBytes(patternRange, (char *)pattern, patternSize);
					if(result.Start != patternRange.Start || result.End != patternRange.End)
					{
						addr = 0;
						ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(addr)
			{
				AFXADDR_SET(csgo_CUnknown_GetPlayerName, addr);
			}
			else
			{
				AFXADDR_SET(csgo_CUnknown_GetPlayerName, 0x0);
			}
		}

		// csgo_CHudDeathNotice_FireGameEvent:
		{
			DWORD addr = 0;
			DWORD strAddr = 0;
			{
				ImageSectionsReader sections((HMODULE)clientDll);
				if(!sections.Eof())
				{
					sections.Next(); // skip .text
					if(!sections.Eof())
					{
						MemRange result = FindCString(sections.GetMemRange(), "realtime_passthrough");
						if(!result.IsEmpty())
						{
							strAddr = result.Start;
						}
						else ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(strAddr)
			{
				ImageSectionsReader sections((HMODULE)clientDll);
			
				MemRange baseRange = sections.GetMemRange();
				MemRange result = FindBytes(baseRange, (char const *)&strAddr, sizeof(strAddr));
				if(!result.IsEmpty())
				{
					addr = result.Start +0x0D;

					// check for pattern to see if it is the right address:
					unsigned char pattern[4] = { 0x56, 0x8B, 0xCF, 0xE8 };

					DWORD patternSize = sizeof(pattern)/sizeof(pattern[0]);
					MemRange patternRange(addr, addr+patternSize);
					MemRange result = FindBytes(patternRange, (char *)pattern, patternSize);
					if(result.Start != patternRange.Start || result.End != patternRange.End)
					{
						addr = 0;
						ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
					else
					{
						addr += patternSize;
						addr = addr +4 + *(DWORD *)addr; // get CALL address
					}
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			AFXADDR_SET(csgo_CHudDeathNotice_FireGameEvent, addr);
		}

		// csgo_CHudDeathNotice_UnkAddDeathNotice:
		//
		// This function is called at the end of csgo_CHudDeathNotice_FireGameEvent,
		// however we search for the string near the call instead to be more stable
		// against updates.
		{
			DWORD addr = 0;
			DWORD strAddr = 0;
			{
				ImageSectionsReader sections((HMODULE)clientDll);
				if(!sections.Eof())
				{
					sections.Next(); // skip .text
					if(!sections.Eof())
					{
						MemRange result = FindWCString(sections.GetMemRange(), L"%s%s%s%s%s%s%s%s");
						if(!result.IsEmpty())
						{
							strAddr = result.Start;
						}
						else ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(strAddr)
			{
				ImageSectionsReader sections((HMODULE)clientDll);
			
				MemRange baseRange = sections.GetMemRange();
				MemRange result = FindBytes(baseRange, (char const *)&strAddr, sizeof(strAddr));
				if(!result.IsEmpty())
				{
					addr = (DWORD)((char const *)result.Start +0x2c);
					addr = addr +4 + *(DWORD *)addr;
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			AFXADDR_SET(csgo_CHudDeathNotice_UnkAddDeathNotice, addr);
		}

		// csgo_CHudDeathNotice_UnkAddDeathNotice_AddMovie_AfterModTime:
		//
		// The function csgo_CHudDeathNotice_UnkAddDeathNotice_AddMovie
		// is called at the end of csgo_CHudDeathNotice_UnkAddDeathNotice
		// however we search for the string near the desired detour location
		// to be more stable agains updates.
		{
			DWORD addr = 0;
			DWORD strAddr = 0;
			{
				ImageSectionsReader sections((HMODULE)clientDll);
				if(!sections.Eof())
				{
					sections.Next(); // skip .text
					if(!sections.Eof())
					{
						MemRange result = FindCString(sections.GetMemRange(), "SetPanelText");
						if(!result.IsEmpty())
						{
							strAddr = result.Start;
						}
						else ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(strAddr)
			{
				ImageSectionsReader sections((HMODULE)clientDll);
			
				MemRange baseRange = sections.GetMemRange();
				MemRange result = FindBytes(baseRange, (char const *)&strAddr, sizeof(strAddr));
				if(!result.IsEmpty())
				{
					DWORD tmpAddr = result.Start +0x6a;

					// check for pattern to see if it is the right address:
					unsigned char pattern[11] = { 0x8D, 0x53, 0x7C, 0xF3, 0x0F, 0x11, 0x87, 0x18, 0x04, 0x00, 0x00 };

					DWORD patternSize = sizeof(pattern)/sizeof(pattern[0]);
					MemRange patternRange(tmpAddr +0x05, tmpAddr +0x05 +patternSize);
					MemRange result = FindBytes(patternRange, (char *)pattern, patternSize);
					if(result.Start != patternRange.Start || result.End != patternRange.End)
						ErrorBox(MkErrStr(__FILE__,__LINE__));
					else
						addr = tmpAddr;

				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			AFXADDR_SET(csgo_CHudDeathNotice_UnkAddDeathNotice_AddMovie_AfterModTime, addr);
		}

		// csgo_CViewRender_Render:
		/*
		{
			DWORD addr = 0;
			DWORD strAddr = 0;
			{
				ImageSectionsReader sections((HMODULE)clientDll);
				if(!sections.Eof())
				{
					sections.Next(); // skip .text
					if(!sections.Eof())
					{
						MemRange result = FindCString(sections.GetMemRange(), "CViewRender::Render");
						if(!result.IsEmpty())
						{
							strAddr = result.Start;
						}
						else ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(strAddr)
			{
				ImageSectionsReader sections((HMODULE)clientDll);
			
				MemRange baseRange = sections.GetMemRange();
				MemRange result = FindBytes(baseRange, (char const *)&strAddr, sizeof(strAddr));
				if(!result.IsEmpty())
				{
					addr = result.Start -0x31;
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(addr)
			{
				AFXADDR_SET(csgo_CViewRender_Render, addr);
			}
			else
			{
				AFXADDR_SET(csgo_CViewRender_Render, 0x0);
			}
		}
		*/

		// csgo_CViewRender_RenderView_AfterVGui_DrawHud:
		{
			DWORD addr = 0;
			DWORD strAddr = 0;
			{
				ImageSectionsReader sections((HMODULE)clientDll);
				if(!sections.Eof())
				{
					sections.Next(); // skip .text
					if(!sections.Eof())
					{
						MemRange result = FindCString(sections.GetMemRange(), "VGui_DrawHud");
						if(!result.IsEmpty())
						{
							strAddr = result.Start;
						}
						else ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(strAddr)
			{
				ImageSectionsReader sections((HMODULE)clientDll);
			
				MemRange baseRange = sections.GetMemRange();
				MemRange result = FindBytes(baseRange, (char const *)&strAddr, sizeof(strAddr));
				if(!result.IsEmpty())
				{
					DWORD targetAddr = result.Start +0x0a;

					// check for pattern nearby to see if it is the right address:
					unsigned char pattern[5] = { 0x6a, 0x04, 0x6a, 0x00, 0x68};

					DWORD patternSize = sizeof(pattern)/sizeof(pattern[0]);
					MemRange patternRange(result.Start -0x0C, result.Start -0x0C +patternSize);
					MemRange result = FindBytes(patternRange, (char *)pattern, patternSize);
					if(result.Start != patternRange.Start || result.End != patternRange.End)
						ErrorBox(MkErrStr(__FILE__,__LINE__));
					else
						addr = targetAddr;
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			AFXADDR_SET(csgo_CViewRender_RenderView_AfterVGui_DrawHud, addr);
		}

		// csgo_CViewRender_RenderSmokeOverlay_OnStoreAlpha:
		{
			DWORD addr = 0;
			DWORD strAddr = 0;
			{
				ImageSectionsReader sections((HMODULE)clientDll);
				if(!sections.Eof())
				{
					sections.Next(); // skip .text
					if(!sections.Eof())
					{
						MemRange result = FindCString(sections.GetMemRange(), "effects/overlaysmoke");
						if(!result.IsEmpty())
						{
							strAddr = result.Start;
						}
						else ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(strAddr)
			{
				ImageSectionsReader sections((HMODULE)clientDll);
			
				MemRange baseRange = sections.GetMemRange();
				MemRange result = FindBytes(baseRange, (char const *)&strAddr, sizeof(strAddr));
				if(!result.IsEmpty())
				{
					baseRange = MemRange(result.End, baseRange.End);
					result = FindBytes(baseRange, (char const *)&strAddr, sizeof(strAddr));
					if(!result.IsEmpty())
					{
						baseRange = MemRange(result.End, baseRange.End);
						result = FindBytes(baseRange, (char const *)&strAddr, sizeof(strAddr));
						if(!result.IsEmpty())
						{
							DWORD targetAddr = result.Start -0x26;

							// check for pattern nearby to see if it is the right address:
							unsigned char pattern[8] = { 0xf3, 0x0f, 0x11, 0x8a, 0x88, 0x05, 0x00, 0x00};

							DWORD patternSize = sizeof(pattern)/sizeof(pattern[0]);
							MemRange patternRange(targetAddr, targetAddr +patternSize);
							MemRange result = FindBytes(patternRange, (char *)pattern, patternSize);
							if(result.Start != patternRange.Start || result.End != patternRange.End)
								ErrorBox(MkErrStr(__FILE__,__LINE__));
							else
								addr = targetAddr;
						}
						else ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			AFXADDR_SET(csgo_CViewRender_RenderSmokeOverlay_OnStoreAlpha, addr);
		}

		// csgo_pLocalPlayer:
		{
			DWORD addr = 0;
			DWORD strAddr = 0;
			{
				ImageSectionsReader sections((HMODULE)clientDll);
				if(!sections.Eof())
				{
					sections.Next(); // skip .text
					if(!sections.Eof())
					{
						MemRange result = FindCString(sections.GetMemRange(), "time to initial render = %f\n");
						if(!result.IsEmpty())
						{
							strAddr = result.Start;
						}
						else ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(strAddr)
			{
				ImageSectionsReader sections((HMODULE)clientDll);
			
				MemRange baseRange = sections.GetMemRange();
				MemRange result = FindBytes(baseRange, (char const *)&strAddr, sizeof(strAddr));
				if(!result.IsEmpty())
				{
					addr = result.Start;
					addr += 0xe;
					addr = *(DWORD *)addr;
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(addr)
			{
				AFXADDR_SET(csgo_pLocalPlayer, addr);
			}
			else
			{
				AFXADDR_SET(csgo_pLocalPlayer, 0x0);
			}
		}

		// csgo_CSkyboxView_Draw:
		{
			DWORD addr = 0;
			DWORD strAddr = 0;
			{
				ImageSectionsReader sections((HMODULE)clientDll);
				if(!sections.Eof())
				{
					sections.Next(); // skip .text
					if(!sections.Eof())
					{
						MemRange result = FindCString(sections.GetMemRange(), "CViewRender::Draw3dSkyboxworld");
						if(!result.IsEmpty())
						{
							strAddr = result.Start;
						}
						else ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(strAddr)
			{
				ImageSectionsReader sections((HMODULE)clientDll);
			
				MemRange baseRange = sections.GetMemRange();
				MemRange result = FindBytes(baseRange, (char const *)&strAddr, sizeof(strAddr));
				if(!result.IsEmpty())
				{
					addr = result.Start;
					addr -= 0x2a;
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(addr)
			{
				AFXADDR_SET(csgo_CSkyboxView_Draw, addr);
			}
			else
			{
				AFXADDR_SET(csgo_CSkyboxView_Draw, 0x0);
			}
		}

		// csgo_view
		{
			DWORD addr = 0;
			DWORD strAddr = 0;
			{
				ImageSectionsReader sections((HMODULE)clientDll);
				if(!sections.Eof())
				{
					sections.Next(); // skip .text
					if(!sections.Eof())
					{
						MemRange result = FindCString(sections.GetMemRange(), "CViewRender::SetUpView->OnRenderEnd");
						if(!result.IsEmpty())
						{
							strAddr = result.Start;
						}
						else ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(strAddr)
			{
				ImageSectionsReader sections((HMODULE)clientDll);
			
				MemRange baseRange = sections.GetMemRange();
				MemRange result = FindBytes(baseRange, (char const *)&strAddr, sizeof(strAddr));
				if(!result.IsEmpty())
				{
					addr = result.Start;
					addr += 0xb;
					addr = *(DWORD *)addr;
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(addr)
			{
				AFXADDR_SET(csgo_view, addr);
			}
			else
			{
				AFXADDR_SET(csgo_view, 0x0);
			}
		}

		/*
		// csgo_CScaleformSlotInitControllerClientImpl_UnkCheckSwf
		{
			DWORD addr = 0;
			DWORD strAddr = 0;
			{
				ImageSectionsReader sections((HMODULE)clientDll);
				if(!sections.Eof())
				{
					sections.Next(); // skip .text
					if(!sections.Eof())
					{
						MemRange result = FindCString(sections.GetMemRange(), "resource/flash/");
						if(!result.IsEmpty())
						{
							strAddr = result.Start;
						}
						else ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(strAddr)
			{
				ImageSectionsReader sections((HMODULE)clientDll);
			
				MemRange baseRange = sections.GetMemRange();
				MemRange result = FindBytes(baseRange, (char const *)&strAddr, sizeof(strAddr));
				if(!result.IsEmpty())
				{
					addr = result.Start;
					addr -= 0x10;
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			if(addr)
			{
				AFXADDR_SET(csgo_CScaleformSlotInitControllerClientImpl_UnkCheckSwf, addr);
			}
			else
			{
				AFXADDR_SET(csgo_CScaleformSlotInitControllerClientImpl_UnkCheckSwf, 0x0);
			}
		}
		*/

		// csgo_C_BasePlayer_OFS_m_skybox3d_scale
		{
			// this basically uses the RecvProp* functions in c_baseplayer.cpp to get the offsets of the fields.

			DWORD strAddr_m_Local = 0;
			DWORD strAddr_m_skybox3d_scale = 0;
			DWORD ofs_m_Local_m_skybox3d_scale = 0;
			{
				ImageSectionsReader sections((HMODULE)clientDll);
				if(!sections.Eof())
				{
					sections.Next(); // skip .text
					if(!sections.Eof())
					{
						{
							MemRange result = FindCString(sections.GetMemRange(), "m_Local");
							if(!result.IsEmpty())
							{
								strAddr_m_Local = result.Start;
							}
							else ErrorBox(MkErrStr(__FILE__,__LINE__));
						}
						{
							MemRange result = FindCString(sections.GetMemRange(), "m_skybox3d.scale");
							if(!result.IsEmpty())
							{
								strAddr_m_skybox3d_scale = result.Start;
							}
							else ErrorBox(MkErrStr(__FILE__,__LINE__));
						}
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}

			if(strAddr_m_Local && strAddr_m_skybox3d_scale)
			{
				bool has_m_Local = false;
				bool has_m_skybox3d_scale = false;

				DWORD ofs_m_Local = 0;
				DWORD ofs_m_skybox3d_scale = 0;
				DWORD addr = 0;

				{
					ImageSectionsReader sections((HMODULE)clientDll);
			
					MemRange baseRange = sections.GetMemRange();
					MemRange result = FindBytes(baseRange, (char const *)&strAddr_m_Local, sizeof(strAddr_m_Local));
					if(!result.IsEmpty())
					{
						addr = result.Start;
						addr += 4+2+4;
						addr = *(DWORD *)addr;
						ofs_m_Local = addr;
						has_m_Local = true;
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));				
				}
				{
					ImageSectionsReader sections((HMODULE)clientDll);
			
					MemRange baseRange = sections.GetMemRange();
					MemRange result = FindBytes(baseRange, (char const *)&strAddr_m_skybox3d_scale, sizeof(strAddr_m_skybox3d_scale));
					if(!result.IsEmpty())
					{
						addr = result.Start;
						addr += 4+2+4;
						addr = *(DWORD *)addr;
						ofs_m_skybox3d_scale = addr;
						has_m_skybox3d_scale = true;
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));				
				}

				if(has_m_Local && has_m_skybox3d_scale)
					ofs_m_Local_m_skybox3d_scale = ofs_m_Local + ofs_m_skybox3d_scale;
			}

			AFXADDR_SET(csgo_C_BasePlayer_OFS_m_skybox3d_scale, ofs_m_Local_m_skybox3d_scale);
		}

		// csgo_C_BaseEntity_ToolRecordEnties:
		{
			DWORD addr = 0;
			DWORD strAddr = 0;
			{
				ImageSectionsReader sections((HMODULE)clientDll);
				if (!sections.Eof())
				{
					sections.Next(); // skip .text
					if (!sections.Eof())
					{
						MemRange result = FindCString(sections.GetMemRange(), "C_BaseEntity::ToolRecordEnties");
						if (!result.IsEmpty())
						{
							strAddr = result.Start;
						}
						else ErrorBox(MkErrStr(__FILE__, __LINE__));
					}
					else ErrorBox(MkErrStr(__FILE__, __LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__, __LINE__));
			}
			if (strAddr)
			{
				ImageSectionsReader sections((HMODULE)clientDll);

				MemRange baseRange = sections.GetMemRange();
				MemRange result = FindBytes(baseRange, (char const *)&strAddr, sizeof(strAddr));
				if (!result.IsEmpty())
				{
					DWORD targetAddr = result.Start - 0x27;

					// check for pattern nearby to see if it is the right address:
					unsigned char pattern[6] = { 0x55, 0x8b, 0xec, 0x51, 0x8b, 0x0d };

					DWORD patternSize = sizeof(pattern) / sizeof(pattern[0]);
					MemRange patternRange(targetAddr, targetAddr + patternSize);
					MemRange result = FindBytes(patternRange, (char *)pattern, patternSize);
					if (result.Start != patternRange.Start || result.End != patternRange.End)
						ErrorBox(MkErrStr(__FILE__, __LINE__));
					else
						addr = targetAddr;
				}
				else ErrorBox(MkErrStr(__FILE__, __LINE__));
			}
			if (addr)
			{
				AFXADDR_SET(csgo_C_BaseEntity_ToolRecordEnties, addr);
			}
			else
			{
				AFXADDR_SET(csgo_C_BaseEntity_ToolRecordEnties, 0x0);
			}
		}

	}
	else
	{
		AFXADDR_SET(csgo_C_BaseEntity_ToolRecordEnties, 0x0);
		AFXADDR_SET(csgo_C_BasePlayer_OFS_m_skybox3d_scale, (AfxAddr)-1);
		AFXADDR_SET(csgo_CUnknown_GetPlayerName, 0x0);
		AFXADDR_SET(csgo_CHudDeathNotice_FireGameEvent, 0x0);
		AFXADDR_SET(csgo_CHudDeathNotice_UnkAddDeathNotice, 0x0);
		AFXADDR_SET(csgo_CHudDeathNotice_UnkAddDeathNotice_AddMovie_AfterModTime, 0x0);
		//AFXADDR_SET(csgo_CScaleformSlotInitControllerClientImpl_UnkCheckSwf, 0x0);
		AFXADDR_SET(csgo_CSkyboxView_Draw, 0x0);
		//AFXADDR_SET(csgo_CViewRender_Render, 0x0);
		AFXADDR_SET(csgo_CViewRender_RenderView_AfterVGui_DrawHud, 0x0);
		AFXADDR_SET(csgo_CViewRender_RenderSmokeOverlay_OnStoreAlpha, 0x0);
		AFXADDR_SET(csgo_pLocalPlayer, 0x0);
		AFXADDR_SET(csgo_view, 0x0);
	}

	AFXADDR_SET(csgo_C_BaseEntity_ToolRecordEnties_DSZ, 0xa);
	AFXADDR_SET(csgo_CUnknown_GetPlayerName_DSZ, 0x0b);
	AFXADDR_SET(csgo_CHudDeathNotice_FireGameEvent_DSZ, 0x0b);
	AFXADDR_SET(csgo_CHudDeathNotice_UnkAddDeathNotice_DSZ, 0x09);
	//AFXADDR_SET(csgo_CScaleformSlotInitControllerClientImpl_UnkCheckSwf_DSZ,0x0c);
	AFXADDR_SET(csgo_CSkyboxView_Draw_DSZ, 0x0d);
	AFXADDR_SET(csgo_gpGlobals_OFS_curtime, 4*4);
	AFXADDR_SET(csgo_gpGlobals_OFS_interpolation_amount, 9*4);
	AFXADDR_SET(csgo_gpGlobals_OFS_interval_per_tick, 8*4);
	//AFXADDR_SET(csgo_CViewRender_Render_DSZ, 0x0c);
	AFXADDR_SET(cstrike_gpGlobals_OFS_curtime, 3*4);
	AFXADDR_SET(cstrike_gpGlobals_OFS_absoluteframetime, 2*4);
	AFXADDR_SET(cstrike_gpGlobals_OFS_interpolation_amount, 8*4);
	AFXADDR_SET(cstrike_gpGlobals_OFS_interval_per_tick, 7*4);
}

void Addresses_InitStdshader_dx9Dll(AfxAddr stdshader_dx9Dll, bool isCsgo)
{
	if(isCsgo)
	{
		// csgo_SplineRope_CShader_vtable:
		{
			DWORD addr = 0;
			{
				ImageSectionsReader sections((HMODULE)stdshader_dx9Dll);
				if(!sections.Eof())
				{
					sections.Next(); // skip .text
					if(!sections.Eof())
					{
						MemRange firstDataRange = sections.GetMemRange();

						sections.Next(); // skip first .data
						if(!sections.Eof())
						{
							MemRange result = FindCString(sections.GetMemRange(), ".?AVCShader@SplineRope@@");
							if(!result.IsEmpty())
							{
								DWORD tmpAddr = result.Start;
								tmpAddr -= 0x8;

								result = FindBytes(firstDataRange, (char const *)&tmpAddr, sizeof(tmpAddr));
								if(!result.IsEmpty())
								{
									DWORD tmpAddr = result.Start;
									tmpAddr -= 0xC;

									result = FindBytes(firstDataRange, (char const *)&tmpAddr, sizeof(tmpAddr));
									if(!result.IsEmpty())
									{
										DWORD tmpAddr = result.Start;
										tmpAddr += (1)*4;

										addr = tmpAddr;
									}
									else ErrorBox(MkErrStr(__FILE__,__LINE__));
								}
								else ErrorBox(MkErrStr(__FILE__,__LINE__));
							}
							else ErrorBox(MkErrStr(__FILE__,__LINE__));
						}
						else ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			AFXADDR_SET(csgo_SplineRope_CShader_vtable, addr);
		}

		// csgo_Spritecard_CShader_vtable:
		{
			DWORD addr = 0;
			{
				ImageSectionsReader sections((HMODULE)stdshader_dx9Dll);
				if(!sections.Eof())
				{
					sections.Next(); // skip .text
					if(!sections.Eof())
					{
						MemRange firstDataRange = sections.GetMemRange();

						sections.Next(); // skip first .data
						if(!sections.Eof())
						{
							MemRange result = FindCString(sections.GetMemRange(), ".?AVCShader@Spritecard@@");
							if(!result.IsEmpty())
							{
								DWORD tmpAddr = result.Start;
								tmpAddr -= 0x8;

								result = FindBytes(firstDataRange, (char const *)&tmpAddr, sizeof(tmpAddr));
								if(!result.IsEmpty())
								{
									DWORD tmpAddr = result.Start;
									tmpAddr -= 0xC;

									result = FindBytes(firstDataRange, (char const *)&tmpAddr, sizeof(tmpAddr));
									if(!result.IsEmpty())
									{
										DWORD tmpAddr = result.Start;
										tmpAddr += (1)*4;

										addr = tmpAddr;
									}
									else ErrorBox(MkErrStr(__FILE__,__LINE__));
								}
								else ErrorBox(MkErrStr(__FILE__,__LINE__));
							}
							else ErrorBox(MkErrStr(__FILE__,__LINE__));
						}
						else ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			AFXADDR_SET(csgo_Spritecard_CShader_vtable, addr);
		}

		// csgo_UnlitGeneric_CShader_vtable:
		{
			DWORD addr = 0;
			{
				ImageSectionsReader sections((HMODULE)stdshader_dx9Dll);
				if(!sections.Eof())
				{
					sections.Next(); // skip .text
					if(!sections.Eof())
					{
						MemRange firstDataRange = sections.GetMemRange();

						sections.Next(); // skip first .data
						if(!sections.Eof())
						{
							MemRange result = FindCString(sections.GetMemRange(), ".?AVCShader@UnlitGeneric@@");
							if(!result.IsEmpty())
							{
								DWORD tmpAddr = result.Start;
								tmpAddr -= 0x8;

								result = FindBytes(firstDataRange, (char const *)&tmpAddr, sizeof(tmpAddr));
								if(!result.IsEmpty())
								{
									DWORD tmpAddr = result.Start;
									tmpAddr -= 0xC;

									result = FindBytes(firstDataRange, (char const *)&tmpAddr, sizeof(tmpAddr));
									if(!result.IsEmpty())
									{
										DWORD tmpAddr = result.Start;
										tmpAddr += (1)*4;

										addr = tmpAddr;
									}
									else ErrorBox(MkErrStr(__FILE__,__LINE__));
								}
								else ErrorBox(MkErrStr(__FILE__,__LINE__));
							}
							else ErrorBox(MkErrStr(__FILE__,__LINE__));
						}
						else ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			AFXADDR_SET(csgo_UnlitGeneric_CShader_vtable, addr);
		}

		// csgo_VertexLitGeneric_CShader_vtable:
		{
			DWORD addr = 0;
			{
				ImageSectionsReader sections((HMODULE)stdshader_dx9Dll);
				if(!sections.Eof())
				{
					sections.Next(); // skip .text
					if(!sections.Eof())
					{
						MemRange firstDataRange = sections.GetMemRange();

						sections.Next(); // skip first .data
						if(!sections.Eof())
						{
							MemRange result = FindCString(sections.GetMemRange(), ".?AVCShader@VertexLitGeneric@@");
							if(!result.IsEmpty())
							{
								DWORD tmpAddr = result.Start;
								tmpAddr -= 0x8;

								result = FindBytes(firstDataRange, (char const *)&tmpAddr, sizeof(tmpAddr));
								if(!result.IsEmpty())
								{
									result = FindBytes(MemRange(result.End,firstDataRange.End), (char const *)&tmpAddr, sizeof(tmpAddr));
									if(!result.IsEmpty())
									{
										DWORD tmpAddr = result.Start;
										tmpAddr -= 0xC;

										result = FindBytes(firstDataRange, (char const *)&tmpAddr, sizeof(tmpAddr));
										if(!result.IsEmpty())
										{
											DWORD tmpAddr = result.Start;
											tmpAddr += (1)*4;

											addr = tmpAddr;
										}
										else ErrorBox(MkErrStr(__FILE__,__LINE__));
									}
									else ErrorBox(MkErrStr(__FILE__,__LINE__));
								}
								else ErrorBox(MkErrStr(__FILE__,__LINE__));
							}
							else ErrorBox(MkErrStr(__FILE__,__LINE__));
						}
						else ErrorBox(MkErrStr(__FILE__,__LINE__));
					}
					else ErrorBox(MkErrStr(__FILE__,__LINE__));
				}
				else ErrorBox(MkErrStr(__FILE__,__LINE__));
			}
			AFXADDR_SET(csgo_VertexLitGeneric_CShader_vtable, addr);
		}
	}
	else
	{
		AFXADDR_SET(csgo_Spritecard_CShader_vtable, 0x0);
		AFXADDR_SET(csgo_UnlitGeneric_CShader_vtable, 0x0);
		AFXADDR_SET(csgo_VertexLitGeneric_CShader_vtable, 0x0);
	}
}
