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
//  e.- When no .vcd/.mp3 files are present the game shows (press 1 to continue)			(client.dll)
//  f.- 'Current' text when saving a new game, in the date/time column.		(GameUI.dll)

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

			addr = (UInt32)client + 0x2D22EE;
			SafeWriteBuf(addr, symbolDollar_2, 5);

			addr = (UInt32)client + 0x2D231F;
			SafeWriteBuf(addr, symbolDollar_2, 5);

			addr = (UInt32)client + 0x2D7924;
			SafeWriteBuf(addr, symbolDollar_1, 3);
		}

		//  e.- When no .vcd/.mp3 files are present the game shows (press 1 to continue)			(client.dll)
		if (GetPrivateProfileIntA("PressOne", "enabled", 0, ".\\Bin\\loader\\localization_addon.ini"))
		{
			GetPrivateProfileStringA("PressOne",
									 "Text",
									 "(press 1 to continue)",
									 value, 36, ".\\Bin\\loader\\localization_addon.ini");
									 // value, 24, ".\\Bin\\loader\\localization_addon.ini");

			//addr = (UInt32)client + 0x285CA8;
			//SafeWriteBuf(addr, value, 23);
			
			// Update address
			addr = (UInt32)client + 0x54339;
			unsigned char PressOneAddress[2] = { 0x30, 0x8A };
			SafeWriteBuf(addr, PressOneAddress, 2);

			// Update text
			addr = (UInt32)client + 0x288A30;
			SafeWriteBuf(addr, value, 35);
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
				value, 36, ".\\Bin\\loader\\localization_addon.ini");
				//value, 20, ".\\Bin\\loader\\localization_addon.ini");

			//addr = (UInt32)engine + 0x195FDC;
			//SafeWriteBuf(addr, value, 19);

			// Update address
			addr = (UInt32)engine + 0x98F02;
			unsigned char QuickSaveAddress[2] = { 0xA0, 0x5F };
			SafeWriteBuf(addr, QuickSaveAddress, 2);

			addr = (UInt32)engine + 0x195FA0;
			SafeWriteBuf(addr, value, 35);
		}

		// Auto Save
		if (GetPrivateProfileIntA("AutoSave", "enabled", 0, ".\\Bin\\loader\\localization_addon.ini"))
		{
			GetPrivateProfileStringA("AutoSave",
				"Text",
				"Auto Save",
				value, 36, ".\\Bin\\loader\\localization_addon.ini");
				//value, 13, ".\\Bin\\loader\\localization_addon.ini");

			//addr = (UInt32)engine + 0x195FF0;
			//SafeWriteBuf(addr, value, 12);

			// Update address
			addr = (UInt32)engine + 0x990FB;
			unsigned char AutoSaveAddress[2] = { 0xD0, 0x5F };
			SafeWriteBuf(addr, AutoSaveAddress, 2);

			addr = (UInt32)engine + 0x195FD0;
			SafeWriteBuf(addr, value, 35);
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

	HMODULE GameUI = GetModuleHandleA("GameUI.dll");
	if (GameUI != NULL)
	{
		//  f.- 'Current' text when saving a new game, in the date/time column.		(GameUI.dll)
		if (GetPrivateProfileIntA("Current", "enabled", 0, ".\\Bin\\loader\\localization_addon.ini"))
		{
			GetPrivateProfileStringA("Current",
                                     "Text",
				                     "Current",
                                     value, 8, ".\\Bin\\loader\\localization_addon.ini");

			addr = (UInt32)GameUI + 0x64C7C;
			SafeWriteBuf(addr, value, 7);
		}
	}



// For tracing if we access .ini file
	//char sample[37];
	//FILE* fout = fopen(".\\Bin\\loader\\test.txt", "wt");
	//sprintf(sample, "%s", value);
	//fputs(sample, fout);
	//fclose(fout);
}

