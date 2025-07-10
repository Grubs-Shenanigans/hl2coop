//========= Copyright Valve Corporation, All rights reserved. ============//
// tf_bot_get_armor.h
// Pick up any nearby armor kit
// Michael Booth, May 2009
// Bitl, July 2025

#include "cbase.h"
#include "tf_gamerules.h"
#include "tf_obj.h"
#include "bot/tf_bot.h"
#include "quiver/bot/behavior/tf_bot_get_armor.h"

extern ConVar tf_bot_path_lookahead_range;

ConVar tf_bot_armor_critical_ratio( "tf_bot_armor_critical_ratio", "0.3", FCVAR_CHEAT );
ConVar tf_bot_armor_ok_ratio( "tf_bot_armor_ok_ratio", "0.8", FCVAR_CHEAT );
ConVar tf_bot_armor_search_near_range( "tf_bot_armor_search_near_range", "1000", FCVAR_CHEAT );
ConVar tf_bot_armor_search_far_range( "tf_bot_armor_search_far_range", "2000", FCVAR_CHEAT );


//---------------------------------------------------------------------------------------------
class CArmorFilter : public INextBotFilter
{
public:
	CArmorFilter( CTFBot *me )
	{
		m_me = me;
	}

	bool IsSelected( const CBaseEntity *constCandidate ) const
	{
		if ( !constCandidate )
			return false;

		CBaseEntity *candidate = const_cast< CBaseEntity * >( constCandidate );

		CTFNavArea *area = (CTFNavArea *)TheNavMesh->GetNearestNavArea( candidate->WorldSpaceCenter() );
		if ( !area )
			return false;

		CClosestTFPlayer close( candidate );
		ForEachPlayer( close );

		// if the closest player to this candidate object is an enemy, don't use it
		if ( close.m_closePlayer && !m_me->InSameTeam( close.m_closePlayer ) )
			return false;

		// resupply cabinets (not assigned a team)
		if ( candidate->ClassMatches( "func_regenerate" ) )
		{
			if ( !area->HasAttributeTF( TF_NAV_SPAWN_ROOM_BLUE | TF_NAV_SPAWN_ROOM_RED ) )
			{
				// Assume any resupply cabinets not in a teamed spawn room are inaccessible.
				// Ex: pl_upward has forward spawn rooms that neither team can use until 
				// certain checkpoints are reached.
				return false;
			}

			if ( ( m_me->GetTeamNumber() == TF_TEAM_RED && area->HasAttributeTF( TF_NAV_SPAWN_ROOM_RED ) ) ||
				 ( m_me->GetTeamNumber() == TF_TEAM_BLUE && area->HasAttributeTF( TF_NAV_SPAWN_ROOM_BLUE ) ) )
			{
				// the supply cabinet is in my spawn room
				return true;
			}

			return false;
		}

		// ignore non-existent ammo to ensure we collect nearby existing ammo
		if ( candidate->IsEffectActive( EF_NODRAW ) )
			return false;

		// ammo pack contains armor.
		if (candidate->ClassMatches("tf_ammo_pack"))
			return true;

		if (candidate->ClassMatches("item_ammopack*"))
			return true;

		if ( m_me->InSameTeam( candidate ) )
		{
			// friendly engineer's dispenser
			if ( candidate->ClassMatches( "obj_dispenser*" ) )
			{
				CBaseObject	*dispenser = (CBaseObject *)candidate;
				if ( !dispenser->IsBuilding() && !dispenser->IsPlacing() && !dispenser->IsDisabled() )
				{
					return true;
				}
			}
		}

		return false;
	}

	CTFBot *m_me;
};


//---------------------------------------------------------------------------------------------
static CTFBot *s_possibleBot = NULL;
static CHandle< CBaseEntity > s_possibleArmor = NULL;
static int s_possibleFrame = 0;


//---------------------------------------------------------------------------------------------
/** 
 * Return true if this Action has what it needs to perform right now
 */
bool CTFBotGetArmor::IsPossible( CTFBot *me )
{
	VPROF_BUDGET( "CTFBotGetArmor::IsPossible", "NextBot" );

#ifdef TF_RAID_MODE
	// mobs don't heal
	if ( TFGameRules()->IsRaidMode() && me->HasAttribute( CTFBot::AGGRESSIVE ) )
	{
		return false;
	}
#endif // TF_RAID_MODE

	if ( TFGameRules()->IsMannVsMachineMode() )
	{
		return false;
	}

	if (me->GetDifficulty() < CTFBot::HARD)
	{
		return false;
	}

	float armorRatio = (float)me->ArmorValue() / (float)me->GetMaxArmor();

	float t = ( armorRatio - tf_bot_armor_critical_ratio.GetFloat() ) / ( tf_bot_armor_ok_ratio.GetFloat() - tf_bot_armor_critical_ratio.GetFloat() );
	t = clamp( t, 0.0f, 1.0f );

	if ( me->m_Shared.InCond( TF_COND_BURNING ) )
	{
		// on fire - get armor now
		t = 0.0f;
	}

	// the more we are hurt, the farther we'll travel to get armor
	float searchRange = tf_bot_armor_search_far_range.GetFloat() + t * ( tf_bot_armor_search_near_range.GetFloat() - tf_bot_armor_search_far_range.GetFloat() );

	CUtlVector< CHandle< CBaseEntity > > armorVector;
	CArmorFilter armorFilter( me );

	// use the ammo entity vector.
	me->SelectReachableObjects( TFGameRules()->GetAmmoEntityVector(), &armorVector, armorFilter, me->GetLastKnownArea(), searchRange);

	if ( armorVector.Count() == 0 )
	{
		if ( me->IsDebugging( NEXTBOT_BEHAVIOR ) )
		{
			Warning( "%3.2f: No armor nearby\n", gpGlobals->curtime );
		}
		return false;
	}

	// use the first item in the list, since it will be the closest to us (or nearly so)
	CBaseEntity *armor = armorVector[0];
	for( int i=0; i<armorVector.Count(); ++i )
	{
		if ( armorVector[i]->GetTeamNumber() != GetEnemyTeam( me->GetTeamNumber() ) )
		{
			armor = armorVector[i];
			break;
		}
	}

	if ( armor == NULL )
	{
		if ( me->IsDebugging( NEXTBOT_BEHAVIOR ) )
		{
			Warning( "%3.2f: No armor available to my team nearby\n", gpGlobals->curtime );
		}
		return false;
	}

	CTFBotPathCost cost( me, FASTEST_ROUTE );
	PathFollower path;
	if ( !path.Compute( me, armor->WorldSpaceCenter(), cost ) )
	{
		if ( me->IsDebugging( NEXTBOT_BEHAVIOR ) )
		{
			Warning( "%3.2f: No path to armor!\n", gpGlobals->curtime );
		}
		return false;
	}

	s_possibleBot = me;
	s_possibleArmor = armor;
	s_possibleFrame = gpGlobals->framecount;

	return true;
}

//---------------------------------------------------------------------------------------------
ActionResult< CTFBot >	CTFBotGetArmor::OnStart( CTFBot *me, Action< CTFBot > *priorAction )
{
	VPROF_BUDGET( "CTFBotGetArmor::OnStart", "NextBot" );

	m_path.SetMinLookAheadDistance( me->GetDesiredPathLookAheadRange() );

	// if IsPossible() has already been called, use its cached data
	if ( s_possibleFrame != gpGlobals->framecount || s_possibleBot != me )
	{
		if ( !IsPossible( me ) || s_possibleArmor == NULL )
		{
			return Done( "Can't get armor" );
		}
	}

	m_Armor = s_possibleArmor;
	m_isGoalDispenser = m_Armor->ClassMatches( "obj_dispenser*" );

	CTFBotPathCost cost( me, SAFEST_ROUTE );
	if ( !m_path.Compute( me, m_Armor->WorldSpaceCenter(), cost ) )
	{
		return Done( "No path to armor!" );
	}

	// if I'm a spy, cloak and disguise
	if ( me->IsPlayerClass( TF_CLASS_SPY ) )
	{
		if ( !me->m_Shared.IsStealthed() )
		{
			me->PressAltFireButton();
		}
	}

	return Continue();
}


//---------------------------------------------------------------------------------------------
ActionResult< CTFBot >	CTFBotGetArmor::Update( CTFBot *me, float interval )
{
	if (m_Armor == NULL || (m_Armor->IsEffectActive( EF_NODRAW ) && !FClassnameIs(m_Armor, "func_regenerate" ) ) )
	{
		return Done( "Armor I was going for has been taken" );
	}

	// if a medic is healing us, give up on getting a kit
	int i;
	for( i=0; i<me->m_Shared.GetNumHealers(); ++i )
	{
		if ( !me->m_Shared.HealerIsDispenser( i ) )
			break;
	}

	if ( me->m_Shared.GetNumHealers() )
	{
		// a dispenser is healing me, don't wait if I'm in combat
		const CKnownEntity *known = me->GetVisionInterface()->GetPrimaryKnownThreat();
		if ( known && known->IsVisibleInFOVNow() )
		{
			return Done( "No time to wait for armor, I must fight" );
		}
	}

	if ( me->ArmorValue() >= me->GetMaxArmor() )
	{
		return Done( "My armor has been repaired" );
	}

	// if the closest player to the item we're after is an enemy, give up
	CClosestTFPlayer close( m_Armor );
	ForEachPlayer( close );
	if ( close.m_closePlayer && !me->InSameTeam( close.m_closePlayer ) )
		return Done( "An enemy is closer to it" );

	// un-zoom
	CTFWeaponBase *myWeapon = me->m_Shared.GetActiveTFWeapon();
	if ( myWeapon && myWeapon->IsWeapon( TF_WEAPON_SNIPERRIFLE ) && me->m_Shared.InCond( TF_COND_ZOOMED ) )
		me->PressAltFireButton();

	if ( !m_path.IsValid() )
	{
		// this can occur if we overshoot the health kit's location
		// because it is momentarily gone
		CTFBotPathCost cost( me, SAFEST_ROUTE );
		if ( !m_path.Compute( me, m_Armor->WorldSpaceCenter(), cost ) )
		{
			return Done( "No path to armor!" );
		}
	}

	m_path.Update( me );

	// may need to switch weapons (ie: engineer holding toolbox now needs to heal and defend himself)
	const CKnownEntity *threat = me->GetVisionInterface()->GetPrimaryKnownThreat();
	me->EquipBestWeaponForThreat( threat );

	return Continue();
}


//---------------------------------------------------------------------------------------------
EventDesiredResult< CTFBot > CTFBotGetArmor::OnStuck( CTFBot *me )
{
	return TryDone( RESULT_CRITICAL, "Stuck trying to reach armor" );
}


//---------------------------------------------------------------------------------------------
EventDesiredResult< CTFBot > CTFBotGetArmor::OnMoveToSuccess( CTFBot *me, const Path *path )
{
	return TryContinue();
}


//---------------------------------------------------------------------------------------------
EventDesiredResult< CTFBot > CTFBotGetArmor::OnMoveToFailure( CTFBot *me, const Path *path, MoveToFailureType reason )
{
	return TryDone( RESULT_CRITICAL, "Failed to reach armor" );
}


//---------------------------------------------------------------------------------------------
// We are always hurrying if we need to collect health
QueryResultType CTFBotGetArmor::ShouldHurry( const INextBot *me ) const
{
	return ANSWER_YES;
}
