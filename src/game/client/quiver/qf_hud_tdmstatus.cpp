//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//

#include "cbase.h"
#include "hudelement.h"
#include "iclientmode.h"
#include <KeyValues.h>
#include <vgui/IScheme.h>
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui_controls/AnimationController.h>
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui/IVGui.h>
#include <vgui/ISurface.h>
#include <vgui/IImage.h>
#include <vgui_controls/Label.h>

#include "c_playerresource.h"
#include "teamplay_round_timer.h"
#include "utlvector.h"
#include "entity_capture_flag.h"
#include "c_tf_player.h"
#include "c_team.h"
#include "c_tf_team.h"
#include "c_team_objectiveresource.h"
#include "tf_hud_objectivestatus.h"
#include "tf_spectatorgui.h"
#include "teamplayroundbased_gamerules.h"
#include "tf_gamerules.h"
#include "tf_hud_freezepanel.h"
#include "c_func_capture_zone.h"
#include "clientmode_shared.h"
#include "tf_hud_mediccallers.h"
#include "view.h"
#include "prediction.h"
#include "tf_logic_robot_destruction.h"

using namespace vgui;

extern ConVar fraglimit;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CQFHudTDM::CQFHudTDM( Panel *parent, const char *name ) : EditablePanel( parent, name )
{
	m_pPlayingTo = NULL;

	vgui::ivgui()->AddTickSignal( GetVPanel(), 250 );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CQFHudTDM::IsVisible( void )
{
	if( IsTakingAFreezecamScreenshot() )
		return false;

	return BaseClass::IsVisible();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CQFHudTDM::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );
	
	LoadControlSettings( "resource/UI/HudObjectiveTDMPanel.res", NULL, NULL, NULL );
	
	m_pPlayingTo = dynamic_cast<CExLabel *>( FindChildByName( "PlayingTo" ) );
	m_pPlayingToBG = FindChildByName( "PlayingToBG" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CQFHudTDM::SetPlayingToLabelVisible( bool bVisible )
{
	if ( m_pPlayingTo && m_pPlayingToBG )
	{
		if ( m_pPlayingTo->IsVisible() != bVisible )
		{
			m_pPlayingTo->SetVisible( bVisible );
		}

		if ( m_pPlayingToBG->IsVisible() != bVisible )
		{
			m_pPlayingToBG->SetVisible( bVisible );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CQFHudTDM::OnTick()
{
	// we're just playing straight score
	C_TFTeam *pTeam = GetGlobalTFTeam( TF_TEAM_BLUE );
	if ( pTeam )
	{
		SetDialogVariable( "bluescore", pTeam->Get_Score() );
	}

	pTeam = GetGlobalTFTeam( TF_TEAM_RED );
	if ( pTeam )
	{
		SetDialogVariable( "redscore", pTeam->Get_Score() );
	}

	SetPlayingToLabelVisible( true );
	
	// by rounds the original hud means cap limit. ugh.
	SetDialogVariable( "rounds", fraglimit.GetInt());
}