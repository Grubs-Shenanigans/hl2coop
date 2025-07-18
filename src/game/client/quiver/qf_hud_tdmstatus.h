//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef QF_HUD_TDMSTATUS_H
#define QF_HUD_TDMSTATUS_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_controls.h"
#include "tf_imagepanel.h"
#include "hudelement.h"

//-----------------------------------------------------------------------------
// Purpose:  
//-----------------------------------------------------------------------------
class CQFHudTDM : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE( CQFHudTDM, vgui::EditablePanel );

public:

	CQFHudTDM( vgui::Panel *parent, const char *name );

	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual bool IsVisible( void );
	void OnTick();

private:
	void SetPlayingToLabelVisible( bool bVisible );

private:
	CExLabel				*m_pPlayingTo;
	vgui::Panel				*m_pPlayingToBG;
};

#endif	// TF_HUD_FLAGSTATUS_H
