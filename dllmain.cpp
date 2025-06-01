// dllmain.cpp : Defines entry point of DLL application.
// This module is for the game Vampire: The Masquerade - Bloodlines
// 
// Author: L@Zar0
// Date: 27/03/2025
// Version: 1.0 for spanish translation based on UP 11.5 (rc14.1)
// Contact: formerly ClanDLAN.net member, now in Discord.

// This module includes some updates for the spanish translation:
//	a.-	Now the '$' symbol of the money appears at the right of the value					(client.dll)
//  b.- Months and Days names																(FileSystem_Stdio.dll)
//  c.- 'Quick Save' and 'Auto Save' words													(engine.dll)
//  d.- Initial 'Loading...' word -máx 10 chars with points-								(engine.dll)
//		This text appears 2 times:
//			1. When we run for first time Vampire.exe. In this case, loader has not
//			   reached yet the module and updated any text.
//			2. After loading modules, and before intro cinematics.
//			   In this case, the loader has reached the module and has updated texts.
//		In any case, I prefer to avoid changing 'engine.dll' directly.
//  e.- When no voiced files are present the game shows "(press 1 to continue)"				(client.dll)
//  f.- 'Current' text when saving a new game, in the date/time column.						(GameUI.dll)
//  g.- 'Name' text when creating new character.											(client.dll)
//  h.- Localize the words for Trait Effects like "Duration" / "Damage"...					(client.dll)
//      -thanks to Niko from Planet Vampire Discord-
//  i.- Fix Terminal font updating chars from external file (.ini)							(client.dll)
//	j.-	Minor fix for counting/showing of ANSI chars over 0x80								(vguimatsurface.dll)
//  k.- Swap Subdir name/"Menu" word, example: "Personnel Menu"->"Menú Personal"			(vampire.dll)
//  l.- Fixes for bottom menu for [n]ext, [p]rev, [d]elete, [m]enu and (From:/Subject:)		(vampire.dll)
//  m.- Pressing ESC in terminal it writes "quit" word automatically. Must much Name33 string.txt word.	(client.dll)+(vampire.dll)


#include "pch.h"
#include <windows.h>
#include <winbase.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char			UInt8;
typedef short unsigned int		UInt16;
typedef unsigned int			UInt32;

char value[37] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
				   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
				   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
				   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };


const char* valueeffectStr;
char valueeffect[16] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
						 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
const char* costStr;
char cost[16] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
				  0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
const char* durationStr;
char duration[16] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
					  0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
const char* damageStr;
char damage[16] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
					0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

char quitWord[16] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
					  0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
char quitWordHackcmd[24] = { 'h', 'a', 'c', 'k', 'c', 'm', 'd', ' ',
							 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
							 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

const char* effectQuantStr[5];
const char* plusStr = "+\0";
const char* multStr = "*\0";
const char* divStr = "/\0";
char max[16] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
			     0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
char min[16] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
				 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

BOOL APIENTRY DllMain(HMODULE hModule,
				   	  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
				     )
{
	return TRUE;
}

void SafeWrite8(UInt32 addr, UInt32 data)
{
	DWORD	oldProtect;

	VirtualProtect((void*)addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
	*((UInt8*)addr) = data;
	VirtualProtect((void*)addr, 4, oldProtect, &oldProtect);
}

void SafeWrite32(UInt32 addr, UInt32 data)
{
	DWORD oldProtect;

	VirtualProtect((void*)addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
	*((UInt32*)addr) = data;
	VirtualProtect((void*)addr, 4, oldProtect, &oldProtect);
}

void SafeWriteBuf(UInt32 addr, void* data, UInt32 len)
{
	DWORD	oldProtect;

	VirtualProtect((void*)addr, len, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy((void*)addr, data, len);
	VirtualProtect((void*)addr, len, oldProtect, &oldProtect);
}

void SafeWriteBufWithOffsetChange(UInt32 addr_data, void* data, UInt32 offset, UInt32 len) {

	SafeWriteBuf(addr_data, data, len);
	SafeWrite32(offset, addr_data);
}

unsigned char* Read8BytesBuffer(UInt32 addr_data) {
	unsigned char fontBuffer[8];
	unsigned char* p;

	for (int i = 0; i < 8; i++)
	{
		p = (unsigned char*)(addr_data + i);
		fontBuffer[i] = *p;
	}
		
	return fontBuffer;
}

char* ConvertByte2String(int bufByte) {
	char binaryString[10];
	char mask = 1; // Bit mask

	for (int i = 0; i < 8; i++) {
		// Mask each bit in the byte and store it
		if ((bufByte & (mask << i)) != 0) {
			binaryString[7 - i] = '#';
		}
		else {
			binaryString[7 - i] = '.';
		}
	}
	binaryString[8] = 0xA;
	binaryString[9] = 0x0;

	return binaryString;
}

void ConvertDBStringToByteArray(char* byteBuf, char* byteArray, int len) {	// Convert Double Byte String to Byte Array
	char cByte[3];
	int i;

	for (i = 0; i < len; i++) {
		cByte[0] = byteBuf[i * 2];
		cByte[1] = byteBuf[(i * 2) + 1];
		cByte[2] = 0;

		byteArray[i] = strtol(cByte, NULL, 16);
	}
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
			SafeWriteBufWithOffsetChange((UInt32)client + 0x288A30, value, (UInt32)client + 0x54339, 35);

			//addr = (UInt32)client + 0x54339;
			//unsigned char PressOneAddress[2] = { 0x30, 0x8A };
			//SafeWriteBuf(addr, PressOneAddress, 2);

			//// Update text
			//addr = (UInt32)client + 0x288A30;
			//SafeWriteBuf(addr, value, 35);
		}

		//  g.- 'Name' text when creating new character.		(client.dll)
		if (GetPrivateProfileIntA("Name", "enabled", 0, ".\\Bin\\loader\\localization_addon.ini"))
		{
			GetPrivateProfileStringA("Name",
									 "Text",
								 	 "Name:",
								 	 value, 8, ".\\Bin\\loader\\localization_addon.ini");
									 // value, 24, ".\\Bin\\loader\\localization_addon.ini");

			// Update text
			addr = (UInt32)client + 0x2CFFF8;
			SafeWriteBuf(addr, value, 7);
		}

		// h.- Localize the words for Trait Effects like "Duration"/"Damage"... (thanks to Niko from Planet Vampire Discord)
		if (GetPrivateProfileIntA("Effects", "enabled", 0, ".\\Bin\\loader\\localization_addon.ini"))
		{
			// Value
			GetPrivateProfileStringA("Effects",
									 "ValueEffect",
									 "Value",
									 valueeffect, 16, ".\\Bin\\loader\\localization_addon.ini");

			addr = (UInt32)client + 0x156BDF + 2;
			valueeffect[15] = '\0';
			valueeffectStr = valueeffect;
			SafeWrite32(addr, (UInt32)(&valueeffectStr));

			// Cost
			GetPrivateProfileStringA("Effects",
								 	 "Cost",
								 	 "Cost",
									 cost, 16, ".\\Bin\\loader\\localization_addon.ini");

			addr = (UInt32)client + 0x156BDF + 1;
			cost[15] = '\0';
			costStr = cost;
			SafeWrite32(addr, (UInt32)(&costStr));

			// Duration
			GetPrivateProfileStringA("Effects",
									 "Duration",
									 "Duration",
									 duration, 16, ".\\Bin\\loader\\localization_addon.ini");

			addr = (UInt32)client + 0x156E4C + 1;
			duration[15] = '\0';
			durationStr = duration;
			SafeWrite32(addr, (UInt32)&durationStr);

			// Damage
			GetPrivateProfileStringA("Effects",
									 "Damage",
									 "Damage",
									 damage, 16, ".\\Bin\\loader\\localization_addon.ini");

			addr = (UInt32)client + 0x156E31 + 1;
			damage[15] = '\0';
			damageStr = damage;
			SafeWrite32(addr, (UInt32)(&damageStr));


			// +, *, /, Max, Min	In this case we need an array of pointers
			effectQuantStr[0] = plusStr;
			effectQuantStr[1] = multStr;
			effectQuantStr[2] = divStr;

			// Max
			GetPrivateProfileStringA("Effects",
									 "Max",
									 "Max",
									 max, 16, ".\\Bin\\loader\\localization_addon.ini");

			max[15] = '\0';
			effectQuantStr[3] = max;

			// Min
			GetPrivateProfileStringA("Effects",
									 "Min",
									 "Min",
									 min, 16, ".\\Bin\\loader\\localization_addon.ini");

			min[15] = '\0';
			effectQuantStr[4] = min;

			addr = ((UInt32)client + 0x156E67 + 3);
			SafeWrite32(addr, (UInt32)(&effectQuantStr));
		}

		// i.- Fix Terminal font
		addr = (UInt32)client + 0x232F10;

		// ExportTable Debug Function
		if (GetPrivateProfileIntA("TerminalFontUpdate", "ExportTable_Debug", 0, ".\\Bin\\loader\\localization_addon.ini"))
		{
			unsigned char* bufBytes;
			int i, j;
			char strfnt[100];

			FILE* fout = fopen(".\\Bin\\loader\\ExportTableTerminalFontDebug.txt", "wt");

			for (i = 0; i < 141; i++) // 141: Max available chars in table for terminal font
			{
				bufBytes = Read8BytesBuffer(addr + (i * 8));

				sprintf(strfnt, "Char: %i\n", i);
				fputs(strfnt, fout);

				fputs(ConvertByte2String(bufBytes[0]), fout);
				fputs(ConvertByte2String(bufBytes[1]), fout);
				fputs(ConvertByte2String(bufBytes[2]), fout);
				fputs(ConvertByte2String(bufBytes[3]), fout);
				fputs(ConvertByte2String(bufBytes[4]), fout);
				fputs(ConvertByte2String(bufBytes[5]), fout);
				fputs(ConvertByte2String(bufBytes[6]), fout);
				fputs(ConvertByte2String(bufBytes[7]), fout);

				//sprintf(strfnt, "Char: %i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n\n", i, bufBytes[0], bufBytes[1], bufBytes[2], bufBytes[3], bufBytes[4], bufBytes[5], bufBytes[6], bufBytes[7]);
				//fputs(strfnt, fout);

				sprintf(strfnt, "\n");
				fputs(strfnt, fout);
			}

			fclose(fout);

		}

		// Fix Fonts
		// Only the fonts assigned in .ini will be updated.
		if (GetPrivateProfileIntA("TerminalFontUpdate", "enabled", 0, ".\\Bin\\loader\\localization_addon.ini"))
		{
			int i;
			char numchar[4];
			char strfnt[17];
			char byteArray[8];

			for (i = 0; i < 141; i++)
			{
				sprintf(numchar, "%i", i);
				GetPrivateProfileStringA("TerminalFontUpdate",
										 numchar,
										 "",
										 strfnt, 17, ".\\Bin\\loader\\localization_addon.ini");

				if (strlen(strfnt)  > 0)
				{
					ConvertDBStringToByteArray(strfnt, byteArray, 8);
					SafeWriteBuf(addr + (i * 8), byteArray, 8);
				}
			}
		}

		// m. - Pressing ESC in terminal it writes "quit" word automatically.Must much Name33 string.txt word.	(client.dll)
		if (GetPrivateProfileIntA("QuitESCTerminal", "enabled", 0, ".\\Bin\\loader\\localization_addon.ini"))
		{
			GetPrivateProfileStringA("QuitESCTerminal",
									 "quit",
									 "quit",
									 quitWord, 16, ".\\Bin\\loader\\localization_addon.ini");

			// There are two addresses accesing to "hackcmd quit" in code
			quitWord[15] = '\0';
			for (int i = 0; i < 16; i++) quitWordHackcmd[i + 8] = quitWord[i];			

			addr = (UInt32)client + 0x0C7177 + 1;
			SafeWrite32(addr, (UInt32)(&quitWordHackcmd));

			addr = (UInt32)client + 0x0C7220 + 1;
			SafeWrite32(addr, (UInt32)(&quitWordHackcmd));
		}

	}

	HMODULE vampire = GetModuleHandleA("vampire.dll");
	if (vampire != NULL)
	{
		// k.- Swap Subdir name/"Menu" word, example: "Personnel Menu"->"Menú Personal"			(vampire.dll) 
		if (GetPrivateProfileIntA("SwapSubDirNameMenuWord", "enabled", 0, ".\\Bin\\loader\\localization_addon.ini"))
		{
			addr = ((UInt32)vampire + 0x21ADBF);
			SafeWrite8(addr, 0x51);
			addr = ((UInt32)vampire + 0x21ADC0);
			SafeWrite8(addr, 0x50);
		}

		// l.- Fixes for bottom menu for [n]ext, [p]rev, [d]elete, [m]enu and (From:/Subject:)
		if (GetPrivateProfileIntA("FixBottomMenu", "enabled", 0, ".\\Bin\\loader\\localization_addon.ini"))
		{
			// This is: "\n%s, %s, %s, %s, %s: "
			unsigned char buffer[21] = { 0x0A, 0x25, 0x73, 0x2C, 0x20, 0x25, 0x73, 0x2C, 0x20, 0x25,
										 0x73, 0x2C, 0x20, 0x25, 0x73, 0x2C, 0x20, 0x25, 0x73, 0x3A, 0x20 };
			addr = ((UInt32)vampire + 0x5B06B4);
			SafeWriteBuf(addr, buffer, 21);

			unsigned char buffer2[9] = { 0x0A, 0x28, 0x25, 0x73, 0x29, 0x20, 0x25, 0x73, 0x0A };
			addr = ((UInt32)vampire + 0x5B06E2);
			SafeWriteBuf(addr, buffer2, 9);
		}

		// m. - Pressing ESC in terminal it writes "quit" word automatically.Must much Name33 string.txt word.	(vampire.dll)
		if (GetPrivateProfileIntA("QuitESCTerminal", "enabled", 0, ".\\Bin\\loader\\localization_addon.ini"))
		{
			GetPrivateProfileStringA("QuitESCTerminal",
				"quit",
				"quit",
				quitWord, 16, ".\\Bin\\loader\\localization_addon.ini");

			// Here there is only one address and it does not use the 'hackcmd' command.
			// This one is used in keypads
			quitWord[15] = '\0';

			addr = (UInt32)vampire + 0x217F94;
			SafeWrite32(addr, (UInt32)(&quitWord));

			addr = (UInt32)vampire + 0x21DEE3;
			SafeWrite32(addr, (UInt32)(&quitWord));
		}

	}

	HMODULE vguimatsurface = GetModuleHandleA("vguimatsurface.dll");
	if (client != NULL)
	{
		//	j.-	This will fix the counting/showing of ANSI chars over 0x80
		if (GetPrivateProfileIntA("CharsOver0x80Fix", "enabled", 0, ".\\Bin\\loader\\localization_addon.ini"))
		{

			// changes char (supposedly signed) to unsigned int8 in the comparision
			addr = ((UInt32)vguimatsurface + 0x10BCA);
			SafeWrite8(addr, 0x77);

			// changes char (supposedly signed) to unsigned int8 in the char array
			addr = ((UInt32)vguimatsurface + 0x10D2D);
			SafeWrite8(addr, 0xB6);

			// table to mark countable/showable chars over 0x80
			addr = ((UInt32)vguimatsurface + 0x3B602);
			SafeWrite8(addr, 0x01);

			addr = ((UInt32)vguimatsurface + 0x3B622);
			SafeWrite8(addr, 0x02);

			addr = ((UInt32)vguimatsurface + 0x3B632);
			unsigned char tablecharsover0x80[128] = {
								   01, 00, 01, 00, 01, 00, 01, 00, 01, 00,
								   01, 00, 01, 00, 01, 00, 01, 00, 01, 00,
								   01, 00, 01, 00, 01, 00, 01, 00, 01, 00,
								   01, 00, 01, 00, 01, 00, 01, 00, 01, 00,
								   01, 00, 01, 00, 01, 00, 01, 00, 01, 00,
								   01, 00, 01, 00, 01, 00, 01, 00, 01, 00,
								   01, 00, 01, 00,
								   02, 00, 02, 00, 02, 00, 02, 00, 02, 00,
								   02, 00, 02, 00, 02, 00, 02, 00, 02, 00,
								   02, 00, 02, 00, 02, 00, 02, 00, 02, 00,
								   02, 00, 02, 00, 02, 00, 02, 00, 02, 00,
								   02, 00, 02, 00, 02, 00, 02, 00, 02, 00,
								   02, 00, 02, 00, 02, 00, 02, 00, 02, 00,
								   02, 00, 02, 00
								};
			
			SafeWriteBuf(addr, tablecharsover0x80, 128);
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

			//addr = (UInt32)engine + 0x195FDC;	// Original address
			//SafeWriteBuf(addr, value, 19);

			SafeWriteBufWithOffsetChange((UInt32)engine + 0x195FA0, value, (UInt32)engine + 0x98F02, 35);
		}

		// Auto Save
		if (GetPrivateProfileIntA("AutoSave", "enabled", 0, ".\\Bin\\loader\\localization_addon.ini"))
		{
			GetPrivateProfileStringA("AutoSave",
									 "Text",
									 "Auto Save",
									 value, 36, ".\\Bin\\loader\\localization_addon.ini");
									 //value, 13, ".\\Bin\\loader\\localization_addon.ini");

			//addr = (UInt32)engine + 0x195FF0;	// Original address
			//SafeWriteBuf(addr, value, 12);

			// Update address
			SafeWriteBufWithOffsetChange((UInt32)engine + 0x195FD0, value, (UInt32)engine + 0x990FB, 35);

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
                                     value, 16, ".\\Bin\\loader\\localization_addon.ini");
                                     //value, 8, ".\\Bin\\loader\\localization_addon.ini");

			//addr = (UInt32)GameUI + 0x64C7C;	// Original address
			//SafeWriteBuf(addr, value, 7);

			// Update address
			SafeWriteBufWithOffsetChange((UInt32)GameUI + 0x667DC, value, (UInt32)GameUI + 0x1838B, 16);
		
		}
	}

// For tracing if we access .ini file
	//char sample[37];
	//FILE* fout = fopen(".\\Bin\\loader\\test.txt", "wt");
	//sprintf(sample, "%s", value);
	//fputs(sample, fout);
	//fclose(fout);
}


unsigned char pushText[101];

void WriteRelLibCall(UInt32 jumpSrc, UInt32 jumpTgt)
{
	// call rel32
	SafeWrite8(jumpSrc, 0xE8);
	SafeWrite32(jumpSrc + 1, jumpTgt - jumpSrc - 1 - 4);
	SafeWrite8(jumpSrc + 5, 0x90);
}

int __stdcall DrawTextHook(HDC hdc, LPCSTR lpchText, int cchText, LPRECT lprc, UINT format)
{
	////////////////////////////////////////////////
	// This lines are only for testing the work with the "Loading...." text
	//////
	//RECT test;

	//test.left = 700;
	//test.right = 1920;
	//test.top = 100;
	//test.bottom = 1080;

	//lprc = &test;

	//format = format & DT_WORDBREAK;
	//format = format & DT_CENTER;

	return DrawTextA(hdc, (LPCSTR)pushText, cchText, lprc, format);
}

extern "C" __declspec(dllexport) void loaded_engine()
{
	UInt32 addr;

	HMODULE engine = GetModuleHandleA("engine.dll");
	if (engine != NULL)
	{
		//  d.- Initial 'Loading...' word -máx 10 chars with points-	(engine.dll)
		// Loading...
		//if (GetPrivateProfileIntA("Loading", "enabled", 0, ".\\Bin\\loader\\localization_addon.ini"))
		//{
		//	GetPrivateProfileStringA("Loading",
		//		"Text",
		//		"Loading....",
		//		value, 12, ".\\Bin\\loader\\localization_addon.ini");

		//	addr = (UInt32)engine + 0x1AD510;
		//	SafeWriteBuf(addr, value, 11);
		//}

		if (GetPrivateProfileIntA("Loading", "enabled", 0, ".\\Bin\\loader\\localization_addon.ini"))
		{
			GetPrivateProfileStringA("Loading",
									 "Text",
									 "Loading....",
									 (LPSTR)pushText, 101, ".\\Bin\\loader\\localization_addon.ini");

			WriteRelLibCall((UInt32)engine + 0xFCD32, (UInt32)DrawTextHook);


			////////////////////////////////////////////////
			// This lines are only for testing the work with the "Loading...." text
			//////
			//GetPrivateProfileStringA("Loading",
			//						 "Text",
			//						 "Loading....",
			//						 (LPSTR)pushText, 13000, ".\\Bin\\loader\\localization_addon.ini");

			//for (int i = 0; i < 13000; i++) {
			//	if (pushText[i] == '#') pushText[i] = 0x0D;
			//}

			//// Font Size
			//addr = (UInt32)engine + 0xFCCF6;
			//SafeWrite8(addr, 0x01);

			//// Font Name
			//addr = (UInt32)engine + 0x1AD51C;
			//value[0] = 'C'; value[1] = 'o'; value[2] = 'u'; value[3] = 'r'; value[4] = 'i'; value[5] = 'e'; value[6] = 'r'; value[7] = 0x0;
			//SafeWriteBuf(addr, value, 7);

		}
	}

	// For tracing if we access .ini file
		//char sample[37];
		//FILE* fout = fopen(".\\Bin\\loader\\test.txt", "wt");
		//sprintf(sample, "%s", value);
		//fputs(sample, fout);
		//fclose(fout);


}


