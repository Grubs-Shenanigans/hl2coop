//========= Copyright Valve Corporation, All rights reserved. ============//
// tf_bot_locomotion.cpp
// Team Fortress NextBot locomotion interface
// Michael Booth, May 2010

#include "cbase.h"

#include "tf_bot.h"
#include "tf_bot_locomotion.h"
#include "particle_parse.h"

#ifdef BDSBASE
ConVar tf_bot_nav_crouch("tf_bot_nav_crouch", "1", FCVAR_CHEAT);
#endif

//-----------------------------------------------------------------------------------------
void CTFBotLocomotion::Update( void )
{
	BaseClass::Update();

	CTFBot *me = ToTFBot( GetBot()->GetEntity() );
	if ( !me )
	{
		return;
	}

	// always 'crouch jump'
	if ( IsOnGround() )
	{
		if ( !me->IsPlayerClass( TF_CLASS_ENGINEER ) )
		{
			// engineers need to crouch behind their guns
			me->ReleaseCrouchButton();
		}

#ifdef BDSBASE
		if (tf_bot_nav_crouch.GetBool())
		{
			const PathFollower* path = me->GetCurrentPath();
			if (path && path->GetCurrentGoal() && path->GetCurrentGoal()->area)
			{
				if (path->GetCurrentGoal()->area->GetAttributes() & NAV_MESH_CROUCH)
				{
					// moving through a crouch area
					me->PressCrouchButton(0.3f);
				}
			}
		}
#endif
	}
	else
	{
		me->PressCrouchButton( 0.3f );
	}
}


//-----------------------------------------------------------------------------------------
// Move directly towards the given position
void CTFBotLocomotion::Approach( const Vector &pos, float goalWeight )
{
	if ( TFGameRules()->IsMannVsMachineMode() )
	{
		if ( !IsOnGround() && !IsClimbingOrJumping() )
		{
			// no air control
			return;
		}
	}

#ifdef BDSBASE
	CTFBot* me = ToTFBot(GetBot()->GetEntity());
	if (!me)
	{
		return;
	}

	if (me->m_Shared.InCond(TF_COND_HALLOWEEN_KART) || me->IsInAVehicle())
	{
		me->ReleaseForwardButton();
		me->ReleaseBackwardButton();
		me->ReleaseLeftButton();
		me->ReleaseRightButton();

		Vector dir = (pos - me->GetAbsOrigin());
		dir.z = 0.0f;
		dir.NormalizeInPlace();

		Vector eye; me->EyeVectors(&eye);
		eye.z = 0.0f;
		eye.NormalizeInPlace();

		Vector eye_90 = Vector(eye.y, -eye.x, 0.0f);

		float fwd = dir.Dot(eye);
		float side = dir.Dot(eye_90);

		if (fwd > 0.50f) 
		{
			me->PressForwardButton();
		}
		/* else if (fwd < -0.50f) 
		{
			me->PressBackwardButton();
		}*/

		const float cos_3deg = cos(DEG2RAD(3.0f));

		if (fwd > 0.0f && fwd < cos_3deg) 
		{
			if (side > 0.0f) 
			{
				me->PressRightButton();
			}
			else 
			{
				me->PressLeftButton();
			}
		}
		else if (fwd > -cos_3deg) 
		{
			if (side > 0.0f) 
			{
				me->PressLeftButton();
			}
			else 
			{
				me->PressRightButton();
			}
		}
	}
#endif

	BaseClass::Approach( pos, goalWeight );
}


//-----------------------------------------------------------------------------------------
// Distance at which we will die if we fall
float CTFBotLocomotion::GetDeathDropHeight( void ) const
{
	return 1000.0f;
}


//-----------------------------------------------------------------------------------------
// Get maximum running speed
float CTFBotLocomotion::GetRunSpeed( void ) const
{
	CTFBot *me = (CTFBot *)GetBot()->GetEntity();
	return me->GetPlayerClass()->GetMaxSpeed();
}


//-----------------------------------------------------------------------------------------
// Return true if given area can be used for navigation
bool CTFBotLocomotion::IsAreaTraversable( const CNavArea *baseArea ) const
{
	CTFBot *me = (CTFBot *)GetBot()->GetEntity();
	CTFNavArea *area = (CTFNavArea *)baseArea;

	if ( area->IsBlocked( me->GetTeamNumber() ) )
	{
		return false;
	}

	if ( !TFGameRules()->RoundHasBeenWon() || TFGameRules()->GetWinningTeam() != me->GetTeamNumber() )
	{
		if ( area->HasAttributeTF( TF_NAV_SPAWN_ROOM_RED ) && me->GetTeamNumber() == TF_TEAM_BLUE )
		{
			return false;
		}

		if ( area->HasAttributeTF( TF_NAV_SPAWN_ROOM_BLUE ) && me->GetTeamNumber() == TF_TEAM_RED )
		{
			return false;
		}
	}

	return true;
}


//-----------------------------------------------------------------------------------------
bool CTFBotLocomotion::IsEntityTraversable( CBaseEntity *obstacle, TraverseWhenType when ) const
{
	// assume all players are "traversable" in that they will move or can be killed
	if ( obstacle && obstacle->IsPlayer() )
	{
		return true;
	}

	return PlayerLocomotion::IsEntityTraversable( obstacle, when );
}


void CTFBotLocomotion::Jump( void )
{
	BaseClass::Jump();

	CTFBot *me = ToTFBot( GetBot()->GetEntity() );
	if ( !me )
	{
		return;
	}

	if ( TFGameRules() && TFGameRules()->IsMannVsMachineMode() )
	{
		int iCustomJumpParticle = 0;
		CALL_ATTRIB_HOOK_INT_ON_OTHER( me, iCustomJumpParticle, bot_custom_jump_particle );
		if ( iCustomJumpParticle )
		{
			const char *pEffectName = "rocketjump_smoke";
			DispatchParticleEffect( pEffectName, PATTACH_POINT_FOLLOW, me, "foot_L" );
			DispatchParticleEffect( pEffectName, PATTACH_POINT_FOLLOW, me, "foot_R" );
		}
	}
}
