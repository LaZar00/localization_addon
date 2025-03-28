# localization_addon

This module is for the game Vampire: The Masquerade - Bloodlines
 
Author: L@Zar0
Date: 27/03/2025
Version: 1.0 for spanish translation based on UP 11.5 (rc14.1)
Contact: formerly ClanDLAN.net member, now in Discord.

This module includes some updates for the spanish translation:ç
a.- Now the '$' symbol of the money appears at the right of the value	(client.dll)
b.- Months and Days names	(FileSystem_Stdio.dll)
c.- 'Quick Save' and 'Auto Save' words	(engine.dll)
d.- Initial 'Loading...' word -máx 10 chars with points-	(engine.dll)
	This text appears 2 times:
  	1. When we run for first time Vampire.exe. In this case, loader has not
	   reached yet the module and updated any text.
	2. After loading modules, and before intro cinematics.
	   In this case, the loader has reached the module and has updated texts.
	In any case, I prefer to avoid changing 'engine.dll' directly.
