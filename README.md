# Source SDK 2013 BDS Base

This is a Source SDK 2013 fork made for the purpose of giving a reliable mod base for currently updated and future Bitl Development Studio (BDS) mod projects.
This was based off the TF2 SDK release, and will be updated as the SDK updates.

This will feature sources for some currently updated and future Bitl Development Studio (BDS) mod projects. 
This base is specific to multiplayer mod projects.

## Projects using this base:
- SURVIVOR II
- Grey's Revenge

## Features:
- Restored game project generator scripts. No more having to generate every project!
- Implements various pull requests from the master repo, allowing for a stable and reliable mod base.
- Build scripts for Half-Life 2, Lost Coast, and HL2 episodes, with NextBot support.
    - NOTE: You may need to do some tinkering to make these singleplayer projects work well with the new SDK base. There are some bugs and crashes that won't be addressed by this base.
- NPC NextBot sensing support from FIREFIGHT RELOADED.
- Integrated Python binaries (on Windows) for more simple buiding.
- Implemented Discord RPC support. Mod authors can change the DiscordAppId parameter in their gameinfo.txt file for more personalized icons. Mods can also use the DiscordAllowMapIcons option to allow map specific icons. Mod icons must be named "ModImage" and map icons must be named the map name.
- Full support of the Half-Life 2 Survivor animation set for all mods.
- reset.bat file in each mod folder, used for cleaning up config/temporary files for easy mod distribution
- Implemented bhopping functionality that can be enabled or disabled by server owners in TF2 and HL2DM.

## Setup:
Read Autumn's setup guide at README_FROG.md for detailed setup.
You may also read a more detailed guide here:
https://developer.valvesoftware.com/wiki/Source_SDK_2013