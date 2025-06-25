//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//

#ifndef AI_SENSES_H
#define AI_SENSES_H

#include "tier1/utlvector.h"
#include "tier1/utlmap.h"
#include "simtimer.h"
#include "ai_component.h"
#include "soundent.h"

#if defined( _WIN32 )
#pragma once
#endif

class CBaseEntity;
class CSound;

//-------------------------------------

DECLARE_POINTER_HANDLE( AISightIter_t );
DECLARE_POINTER_HANDLE( AISoundIter_t );

// GetFirstSeenEntity can take these as optional parameters to search for 
// a specific type of entity.
enum seentype_t
{
	SEEN_ALL = -1,	// Default
	SEEN_HIGH_PRIORITY = 0,
	SEEN_NPCS,
#if defined(BDSBASE) && defined(BDSBASE_NPC)
#ifdef NEXT_BOT
	SEEN_MISC,
	SEEN_NEXTBOTS
#else
	SEEN_MISC
#endif
#else
	SEEN_MISC
#endif
};

#define SENSING_FLAGS_NONE			0x00000000
#define SENSING_FLAGS_DONT_LOOK		0x00000001 // Effectively makes the NPC blind
#define SENSING_FLAGS_DONT_LISTEN	0x00000002 // Effectively makes the NPC deaf

//-----------------------------------------------------------------------------
// class CAI_ScriptConditions
//
// Purpose: 
//-----------------------------------------------------------------------------

class CAI_Senses : public CAI_Component
{
public:
	CAI_Senses()
	 : 	m_LookDist(2048),
		m_LastLookDist(-1),
		m_TimeLastLook(-1),
		m_iAudibleList(0),
		m_TimeLastLookHighPriority( -1 ),
		m_TimeLastLookNPCs( -1 ),
		m_TimeLastLookMisc( -1 )
	{
		m_SeenArrays[0] = &m_SeenHighPriority;
		m_SeenArrays[1] = &m_SeenNPCs;
		m_SeenArrays[2] = &m_SeenMisc;
#if defined(BDSBASE) && defined(BDSBASE_NPC)
#ifdef NEXT_BOT
		m_SeenArrays[3] = &m_SeenNextBots;
#endif
#endif
		m_iSensingFlags = SENSING_FLAGS_NONE;
	}
	
	float			GetDistLook() const				{ return m_LookDist; }
	void			SetDistLook( float flDistLook ) { m_LookDist = flDistLook; }

	void			PerformSensing();

	void			Listen( void );
	void			Look( int iDistance );// basic sight function for npcs

	bool			ShouldSeeEntity( CBaseEntity *pEntity ); // logical query
	bool			CanSeeEntity( CBaseEntity *pSightEnt ); // more expensive cone & raycast test
#ifdef PORTAL
	bool			CanSeeEntityThroughPortal( const CProp_Portal *pPortal, CBaseEntity *pSightEnt ); // more expensive cone & raycast test
#endif
	
	bool			DidSeeEntity( CBaseEntity *pSightEnt ) const; //  a less expensive query that looks at cached results from recent conditionsa gathering

	CBaseEntity *	GetFirstSeenEntity( AISightIter_t *pIter, seentype_t iSeenType = SEEN_ALL ) const;
	CBaseEntity *	GetNextSeenEntity( AISightIter_t *pIter ) const;

	CSound *		GetFirstHeardSound( AISoundIter_t *pIter );
	CSound *		GetNextHeardSound( AISoundIter_t *pIter );
	CSound *		GetClosestSound( bool fScent = false, int validTypes = ALL_SOUNDS | ALL_SCENTS, bool bUsePriority = true );

	bool 			CanHearSound( CSound *pSound );

	//---------------------------------
	
	float			GetTimeLastUpdate( CBaseEntity *pEntity );

	//---------------------------------

	void			AddSensingFlags( int iFlags )		{ m_iSensingFlags |= iFlags; }
	void			RemoveSensingFlags( int iFlags )	{ m_iSensingFlags &= ~iFlags; }
	bool			HasSensingFlags( int iFlags )		{ return (m_iSensingFlags & iFlags) == iFlags; }

	DECLARE_SIMPLE_DATADESC();

private:
	int				GetAudibleList() const { return m_iAudibleList; }

	bool			WaitingUntilSeen( CBaseEntity *pSightEnt );

	void			BeginGather();
	void 			NoteSeenEntity( CBaseEntity *pSightEnt );
	void			EndGather( int nSeen, CUtlVector<EHANDLE> *pResult );
	
	bool 			Look( CBaseEntity *pSightEnt );
#ifdef PORTAL
	bool 			LookThroughPortal( const CProp_Portal *pPortal, CBaseEntity *pSightEnt );
#endif

	int 			LookForHighPriorityEntities( int iDistance );
	int 			LookForNPCs( int iDistance );
	int 			LookForObjects( int iDistance );
#if defined(BDSBASE) && defined(BDSBASE_NPC)
#ifdef NEXT_BOT
	int 			LookForNextBots(int iDistance);
#endif
#endif
	
	bool			SeeEntity( CBaseEntity *pEntity );
	
	float			m_LookDist;				// distance npc sees (Default 2048)
	float			m_LastLookDist;
	float			m_TimeLastLook;
	
	int				m_iAudibleList;				// first index of a linked list of sounds that the npc can hear.
	
	CUtlVector<EHANDLE> m_SeenHighPriority;
	CUtlVector<EHANDLE> m_SeenNPCs;
	CUtlVector<EHANDLE> m_SeenMisc;
#if defined(BDSBASE) && defined(BDSBASE_NPC)
#ifdef NEXT_BOT
	CUtlVector<EHANDLE> m_SeenNextBots;
#endif

#ifdef NEXT_BOT
	CUtlVector<EHANDLE>* m_SeenArrays[4];
#else
	CUtlVector<EHANDLE>* m_SeenArrays[3];
#endif
#else
	CUtlVector<EHANDLE>* m_SeenArrays[3];
#endif
	
	float			m_TimeLastLookHighPriority;
	float			m_TimeLastLookNPCs;
	float			m_TimeLastLookMisc;
#if defined(BDSBASE) && defined(BDSBASE_NPC)
#ifdef NEXT_BOT
	float			m_TimeLastLookNextBots;
#endif
#endif

	int				m_iSensingFlags;
};

//-----------------------------------------------------------------------------

class CAI_SensedObjectsManager : public IEntityListener
{
public:
	void Init();
	void Term();

	CBaseEntity *	GetFirst( int *pIter );
	CBaseEntity *	GetNext( int *pIter );

	virtual void 	AddEntity( CBaseEntity *pEntity );

private:
	virtual void 	OnEntitySpawned( CBaseEntity *pEntity );
	virtual void 	OnEntityDeleted( CBaseEntity *pEntity );

	CUtlVector<EHANDLE> m_SensedObjects;
};

extern CAI_SensedObjectsManager g_AI_SensedObjectsManager;

//-----------------------------------------------------------------------------



#endif // AI_SENSES_H
