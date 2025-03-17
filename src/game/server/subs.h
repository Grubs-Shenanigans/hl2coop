//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//===========================================================================//

#ifdef BDSBASE
#ifndef SUBS_H
#define SUBS_H
#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"

class CBasePlayerSpawn : public CServerOnlyPointEntity
{
public:
	DECLARE_CLASS(CBasePlayerSpawn, CServerOnlyPointEntity);
};

class CBaseDMStart : public CBasePlayerSpawn
{
public:
	DECLARE_CLASS( CBaseDMStart, CBasePlayerSpawn);

	bool IsTriggered( CBaseEntity *pEntity );

	DECLARE_DATADESC();

	string_t m_Master;

private:
};

class CBaseTeamSpawn : public CBasePlayerSpawn
{
public:
	DECLARE_CLASS( CBaseTeamSpawn, CBasePlayerSpawn);
};

#endif
#endif