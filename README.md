# Source SDK 2013 BDS Base

This is a Source SDK 2013 fork made for the purpose of giving a reliable mod base for currently updated and future Bitl Development Studio (BDS) mod projects.
This was based off the TF2/64-bit SDK release, and will be updated as the SDK updates.

This will feature sources for some currently updated and future Bitl Development Studio (BDS) mod projects. 
This base is specific to multiplayer mod projects.

## Projects using this base:
- SURVIVOR II (Based on HL2DM)

## Features:
- Restored game project generator scripts. No more having to generate every project!
- Implements various pull requests from the master repo, allowing for a stable and reliable mod base.
- Build scripts for Half-Life 2, Lost Coast, and HL2 episodes, with NextBot support.
    - NOTE: You may need to do some tinkering to make these singleplayer projects work well with the new SDK base. There are some bugs and crashes that won't be addressed by this base.
- NPC NextBot sensing support from FIREFIGHT RELOADED with the BDSBASE_NPC preprocessor definition.
- Integrated Python binaries (on Windows) for more simple buiding.
- Implemented Discord RPC support with the BDSBASE_DISCORD preprocessor definition. Mod authors can change the DiscordAppId parameter in their gameinfo.txt file for more personalized icons. Mods can also use the DiscordAllowMapIcons option to allow map specific icons. Mod icons must be named "ModImage" and map icons must be named the map name.
- Full support of the Half-Life 2 Survivor animation set for all mods.
- reset.bat file in each mod folder, used for cleaning up config/temporary files for easy mod distribution
- Implemented bhopping functionality that can be enabled or disabled by server owners in TF2 and HL2DM.
- Multiplayer NPC support with the BDSBASE_NPC preprocessor definition.
- Custom items support for TF2 with the BDSBASE_CUSTOM_SCHEMA preprocessor definition.

## Options/Preprocessor Definitions
These options are meant to be added to the VPC files (client and server) of the mod you wish to modify.
These are optional and don't impact the usability of the core fork if they're not enabled.

BDSBASE_NPC: 
- Games: All
- Enables multiplayer NPC support as well as NextBot visibility fixes for NPCs.

BDSBASE_DISCORD: 
- Games: All
- Enables optional Discord integration. Note that this is a Windows only feature due to the unstability of Discord clients on Linux, and the Discord RPC library will be included with your solution. However, the DLL file wouldn't be required to load it.

BDSBASE_CUSTOM_SCHEMA
- Games: TF2
- Enables the creation of a items_custom.txt file that allows the addition and modification of items without needing to modify the items_game.txt file.

BDSBASE_TF2_LEGACY
- Games: TF2
- Enables features from older versions of TF2, such as the old main menu, older flame thrower particles, and various other features.

## Setup:
Read Autumn/Misyl's setup guide at README_FROG.md for detailed setup.
You may also read a more detailed guide here:
https://developer.valvesoftware.com/wiki/Source_SDK_2013