"Resource/UI/HudPlayerArmor.res"
{	
	// player health data
	"HudPlayerArmor"
	{
		"ControlName"	"EditablePanel"
		"fieldName"		"HudPlayerArmor"
		"xpos"			"50"		[$WIN32]
		"xpos_minmode"	"60"		[$WIN32]
		"ypos"			"r127"	[$WIN32]
		"ypos_minmode"	"r94"	[$WIN32]
		"zpos"			"0"
		"wide"			"250"
		"tall"			"120"
		"visible"		"1"
		"enabled"		"1"	
		"ArmorBonusPosAdj"	"10"
		"ArmorDeathWarning"	"0.49"
		"ArmorDeathWarningColor"	"HUDDeathWarning"
	}	
	"PlayerStatusArmorImage"
	{
		"ControlName"	"ImagePanel"
		"fieldName"		"PlayerStatusArmorImage"
		"xpos"			"75"
		"xpos_minmode"	"60"
		"ypos"			"35"
		"zpos"			"2"
		"wide"			"51"
		"tall"			"51"
		"visible"		"1"
		"enabled"		"1"
		"scaleImage"	"1"	
	}		
	"PlayerStatusArmorImageBG"
	{
		"ControlName"	"ImagePanel"
		"fieldName"		"PlayerStatusArmorImageBG"
		"xpos"			"73"
		"xpos_minmode"	"58"
		"ypos"			"33"
		"zpos"			"1"
		"wide"			"55"
		"tall"			"55"
		"visible"		"1"
		"enabled"		"1"
		"image"			"../hud/armor_bg"
		"scaleImage"	"1"	
	}	
	"PlayerStatusArmorBonusImage"
	{
		"ControlName"	"ImagePanel"
		"fieldName"		"PlayerStatusArmorBonusImage"
		"xpos"			"73"	[$WIN32]
		"xpos_minmode"	"65"	[$WIN32]
		"xpos"			"83"	[$X360]
		"ypos"			"33"	[$WIN32]
		"ypos_minmode"	"40"	[$WIN32]
		"ypos"			"43"	[$X360]
		"zpos"			"0"
		"wide"			"55"	[$WIN32]
		"wide_minmode"	"40"	[$WIN32]
		"wide"			"35"	[$X360]
		"tall"			"55"	[$WIN32]
		"tall_minmode"	"40"	[$WIN32]
		"tall"			"35"	[$X360]
		"visible"		"0"
		"enabled"		"1"
		"image"			"../hud/armor_over_bg"
		"scaleImage"	"1"	
	}
	"PlayerStatusArmorValue"
	{
		"ControlName"	"CExLabel"
		"fieldName"		"PlayerStatusArmorValue"
		"xpos"			"76"
		"xpos_minmode"	"61"
		"ypos"			"52"	[$WIN32]
		"ypos"			"55"	[$X360]
		"zpos"			"3"
		"wide"			"50"
		"tall"			"18"
		"visible"		"1"
		"enabled"		"1"
		"labelText"		"%Armor%"
		"textAlignment"	"center"	
		"font"			"HudClassHealth"
		"fgcolor"		"TanDark"
	}
	"PlayerStatusMaxArmorValue"
	{
		"ControlName"	"CExLabel"
		"fieldName"		"PlayerStatusMaxArmorValue"
		"xpos"			"76"
		"xpos_minmode"	"61"
		"ypos"			"26"	[$WIN32]
		"ypos"			"55"	[$X360]
		"zpos"			"4"
		"wide"			"50"
		"tall"			"18"
		"visible"		"1"
		"enabled"		"1"
		"labelText"		"%MaxArmor%"
		"textAlignment"	"center"	
		"font"			"DefaultSmall"
		"fgcolor"		"TanDark"
	}
}
