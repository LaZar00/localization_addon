# localization_addon

This module is for the game Vampire: The Masquerade - Bloodlines

I decided to code this external module to avoid changing the original libraries that the Unofficial Patch or even the original game includes in the game.
 
Author: L@Zar0\
Date: 27/03/2025\
Update: 21/05/2025\
Version: 1.9 for spanish translation based on UP 11.5\
Contact: formerly ClanDLAN.net member, now in Discord.

This module includes some updates for localized versions (coded for the spanish translation):
* a. Now the '$' symbol of the money appears at the right of the value	(client.dll)\
  __IT IS MANDATORY to delete the '$' symbol in strings.txt, Name4 keyword in UIInfoStrs section.__
* b. Months and Days names in Load/Save window	(FileSystem_Stdio.dll)
* c. 'Quick Save' and 'Auto Save' words in Load/Save window	(engine.dll)
* d. Initial 'Loading...' word -máx 10 chars with points-	(engine.dll)
		This text appears 2 times:\
  		1. When we run for first time Vampire.exe.\
		2. After loading modules, and before intro cinematics.
* e.- When no .vcd/.mp3 files are present the game shows (press 1 to continue)	(client.dll)
* f.- 'Current' text when saving a new game, in the date/time column.		(GameUI.dll)
* g.- 'Name' text when creating new character.											(client.dll)
* h.- Localize the words for Trait Effects like "Duration" / "Damage"...					(client.dll)\
          -thanks to Niko from Planet Vampire Discord-
* i.- Fix Terminal font updating chars from external file (.ini)							(client.dll)
* j.-	This will fix the counting/showing of ANSI chars over 0x80							(vguimatsurface.dll)
* k.- Swap Subdir name/"Menu" word, example: "Personnel Menu"->"Menú Personal"			(vampire.dll)
* l.- Pressing ESC in terminal it writes "quit" word automatically. Must much Name33 string.txt word.	(client.dll)

# Install

You will need to install this in `\Bin\loader` folder in the path where Vampire the Masquerade - Bloodlines has been installed.
You can enable (1) or disable (0) the features modifying the .ini file.
Unofficial Patch 11.5 by Wesp5 required.

# Acknowledgements
+ Niko from Planet Vampire Discord in appointing me to documentation for coding this.
