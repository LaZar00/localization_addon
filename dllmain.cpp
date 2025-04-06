// dllmain.cpp : Defines entry point of DLL application.
// This module is for the game Vampire: The Masquerade - Bloodlines
// 
// Author: L@Zar0
// Date: 27/03/2025
// Version: 1.0 for spanish translation based on UP 11.5 (rc14.1)
// Contact: formerly ClanDLAN.net member, now in Discord.

// This module includes some updates for the spanish translation:
//	a.-	Now the '$' symbol of the money appears at the right of the value	(client.dll)
//  b.- Months and Days names									(FileSystem_Stdio.dll)
//  c.- 'Quick Save' and 'Auto Save' words						(engine.dll)
//  d.- Initial 'Loading...' word -máx 10 chars with points-	(engine.dll)
//		This text appears 2 times:
//			1. When we run for first time Vampire.exe. In this case, loader has not
//			   reached yet the module and updated any text.
//			2. After loading modules, and before intro cinematics.
//			   In this case, the loader has reached the module and has updated texts.
//		In any case, I prefer to avoid changing 'engine.dll' directly.
//  e.- Sometimes the game shows (press 1 to continue)			(client.dll)

#include "pch.h"
#include <windows.h>
#include <winbase.h>
#include <stdio.h>

typedef unsigned char			UInt8;
typedef short unsigned int		UInt16;
typedef unsigned int			UInt32;

char value[37] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
				   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
				   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
				   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };


BOOL APIENTRY DllMain(HMODULE hModule,
				   	  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
				     )
{
	return TRUE;
}

void SafeWriteBuf(UInt32 addr, void* data, UInt32 len)
{
	DWORD	oldProtect;

	VirtualProtect((void*)addr, len, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy((void*)addr, data, len);
	VirtualProtect((void*)addr, len, oldProtect, &oldProtect);
}

void SafeWrite32(UInt32 addr, UInt32 data)
{
	DWORD oldProtect;

	VirtualProtect((void*)addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
	*((UInt32*)addr) = data;
	VirtualProtect((void*)addr, 4, oldProtect, &oldProtect);
}

extern "C" __declspec(dllexport) void loaded_client()
{
	UInt32 addr;

	// For this case you need to update the file:		string.txt
	//		* You have to REMOVE the symbol '$' in UIInfoStrs -> Name4 section to ' ' (space)
	unsigned char symbolDollar_2[5] = { 0x25, 0x73, 0x25, 0x64, 0x24 };	// from 24 25 64 '%s %s%d.' to '%s%s%d$.'
	// This is the normal case in .dll
	unsigned char symbolDollar_1[3] = { 0x25, 0x64, 0x24 };	// from 24 25 64 '$%d' to '%d$'

	HMODULE client = GetModuleHandleA("client.dll");
	if (client != NULL)
	{
		//	a.-	Now the '$' symbol of the money appears at the right of the value
		if (GetPrivateProfileIntA("DollarSymbolRightPos", "enabled", 0, ".\\Bin\\loader\\localization_addon.ini"))
		{
			addr = (UInt32)client + 0x287377;
			SafeWriteBuf(addr, symbolDollar_1, 3);

			addr = (UInt32)client + 0x2CFB17;
			SafeWriteBuf(addr, symbolDollar_1, 3);

			addr = (UInt32)client + 0x2D2EE;
			SafeWriteBuf(addr, symbolDollar_2, 5);

			addr = (UInt32)client + 0x2D7924;
			SafeWriteBuf(addr, symbolDollar_1, 3);
		}

		//  e.- Sometimes the game shows (press 1 to continue)
		if (GetPrivateProfileIntA("PressOne", "enabled", 0, ".\\Bin\\loader\\localization_addon.ini"))
		{
			GetPrivateProfileStringA("PressOne",
									 "Text",
									 "(press 1 to continue)",
									 value, 24, ".\\Bin\\loader\\localization_addon.ini");

			addr = (UInt32)client + 0x285CA8;
			SafeWriteBuf(addr, value, 23);
		}

	}

	HMODULE FileSystem_Stdio = GetModuleHandleA("FileSystem_Stdio.dll");
	if (FileSystem_Stdio != NULL)
	{

		//  b.- Months and Days names									(FileSystem_Stdio.dll)
		if (GetPrivateProfileIntA("Months", "enabled", 0, ".\\Bin\\loader\\localization_addon.ini"))
		{
			// Days
			GetPrivateProfileStringA("Days",
									 "Names",
									 "SunMonTueWedThuFriSat",
									 value, 22, ".\\Bin\\loader\\localization_addon.ini");

			addr = (UInt32)FileSystem_Stdio + 0x1D858;
			SafeWriteBuf(addr, value, 21);

			// Months
			GetPrivateProfileStringA("Months", 
									 "Names",
									 "JanFebMarAprMayJunJulAugSepOctNovDec",
									 value, 37, ".\\Bin\\loader\\localization_addon.ini");

			addr = (UInt32)FileSystem_Stdio + 0x1D870;
			SafeWriteBuf(addr, value, 36);
		}
	}

	HMODULE engine = GetModuleHandleA("engine.dll");
	if (engine != NULL)
	{
		//  c.- 'Quick Save' and 'Auto Save' words						(engine.dll)
		// Quick Save
		if (GetPrivateProfileIntA("QuickSave", "enabled", 0, ".\\Bin\\loader\\localization_addon.ini"))
		{
			GetPrivateProfileStringA("QuickSave",
				"Text",
				"Quick Save",
				value, 20, ".\\Bin\\loader\\localization_addon.ini");

			addr = (UInt32)engine + 0x195FDC;
			SafeWriteBuf(addr, value, 19);
		}

		// Auto Save
		if (GetPrivateProfileIntA("AutoSave", "enabled", 0, ".\\Bin\\loader\\localization_addon.ini"))
		{
			GetPrivateProfileStringA("AutoSave",
				"Text",
				"Auto Save",
				value, 13, ".\\Bin\\loader\\localization_addon.ini");

			addr = (UInt32)engine + 0x195FF0;
			SafeWriteBuf(addr, value, 12);
		}

		//  d.- Initial 'Loading...' word -máx 10 chars with points-	(engine.dll)
		// Loading...
		if (GetPrivateProfileIntA("Loading", "enabled", 0, ".\\Bin\\loader\\localization_addon.ini"))
		{
			GetPrivateProfileStringA("Loading",
				"Text",
				"Loading....",
				value, 12, ".\\Bin\\loader\\localization_addon.ini");

			addr = (UInt32)engine + 0x1AD510;
			SafeWriteBuf(addr, value, 11);
		}

	}

// For tracing if we access .ini file
	//char sample[37];
	//FILE* fout = fopen(".\\Bin\\loader\\test.txt", "wt");
	//sprintf(sample, "%s", value);
	//fputs(sample, fout);
	//fclose(fout);
}

