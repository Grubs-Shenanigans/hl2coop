"GameMenu" [$WIN32]
{
	"VRModeButton"
	{
		"label" "#MMenu_VRMode_Activate"
		"command" "engine vr_toggle"
		"subimage" "glyph_vr"
		"OnlyWhenVREnabled" "1"
	}

	// These buttons are only shown while in-game
	// and also are positioned by the .res file
	"CallVoteButton"
	{
		"label"			""
		"command"		"callvote"
		"OnlyInGame"	"1"
		"subimage" "icon_checkbox"
		"tooltip" "#MMenu_CallVote"
	}
	"MutePlayersButton"
	{
		"label"			""
		"command"		"OpenMutePlayerDialog"
		"OnlyInGame"	"1"
		"subimage" "glyph_muted"
		"tooltip" "#MMenu_MutePlayers"
	}
	"RequestCoachButton"
	{
		"label"			""
		"command"		"engine cl_coach_find_coach"
		"OnlyInGame"	"1"
		"subimage" "icon_whistle"
		"tooltip" "#MMenu_RequestCoach"
	}
	"ReportPlayerButton"
	{
		"label"			""
		"command"		"OpenReportPlayerDialog"
		"OnlyInGame"	"1"
		"subimage"		"glyph_alert"
		"tooltip"		"#MMenu_ReportPlayer"
	}

	"1"
	{
		"label" "#GameUI_GameMenu_ResumeGame"
		"command" "ResumeGame"
		"OnlyInGame" "1"
		"OnlyInLegacyMode"	"1"
	}
	"2"
	{
		"label" "#GameUI_GameMenu_Disconnect"
		"command" "Disconnect"
		"OnlyInGame" "1"
		"OnlyInLegacyMode"	"1"
	}
	"3"
	{
		"label" "#GameUI_GameMenu_PlayerList"
		"command" "OpenPlayerListDialog"
		"OnlyInGame" "1"
		"OnlyInLegacyMode"	"1"
	} 
	"4"
	{
		"label" "#GameUI_GameMenu_CallVote"
		"command" "engine open_vote"
		"OnlyInGame" "1"
		"OnlyInLegacyMode"	"1"
	} 
	
	"5"
	{
		"label" "------------------------"
		"OnlyInGame" "1"
		"OnlyInLegacyMode"	"1"
	}
	
	"6"
	{
		"label" "#GameUI_GameMenu_CharacterSetup"
		"command" "engine open_charinfo"
		"OnlyInLegacyMode"	"1"
	}
	"7"
	{
		"label" "#GameUI_GameMenu_Achievements"
		"command" "OpenAchievementsDialog"
		"OnlyInLegacyMode"	"1"
	}
	"8"
	{
		"label" "#GameUI_GameMenu_Options"
		"command" "OpenOptionsDialog"
		"OnlyInLegacyMode"	"1"
	}
	"9"
	{
		"label" "#GameUI_GameMenu_AdvancedOptions"
		"command" "engine opentf2options"
		"OnlyInLegacyMode"	"1"
	}
	
	"10"
	{
		"label" "------------------------"
		"OnlyInLegacyMode"	"1"
	}
	
	"11"
	{
		"label" "#GameUI_GameMenu_FindServers" 
		"command" "OpenServerBrowser"
		"OnlyInLegacyMode"	"1"
	} 
	"12"
	{
		"label" "#GameUI_GameMenu_CreateServer"
		"command" "OpenCreateMultiplayerGameDialog"
		"OnlyInLegacyMode"	"1"
	}
	"13"
	{
		"label"	"#GameUI_LoadCommentary"
		"command" "OpenLoadSingleplayerCommentaryDialog"
		"OnlyInLegacyMode"	"1"
	}
	"14"
	{
		"label" "#GameUI_Controller"
		"command" "OpenControllerDialog"
		"ConsoleOnly" "1"
		"OnlyInLegacyMode"	"1"
	}
	
	"15"
	{
		"label" "------------------------"
		"OnlyInLegacyMode"	"1"
	}
	
	"16"
	{
		"label" "#GameUI_GameMenu_Quit"
		"command" "Quit"
		"OnlyInLegacyMode"	"1"
	}
}
