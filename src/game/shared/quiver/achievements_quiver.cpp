//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================


#include "cbase.h"

#ifdef CLIENT_DLL

#include "achievementmgr.h"
#include "baseachievement.h"
#include "tf_hud_statpanel.h"
#include "c_tf_team.h"
#include "c_tf_player.h"
#include "c_tf_playerresource.h"
#include "tf_gamerules.h"
#include "achievements_tf.h"
#include "c_tf_playerresource.h"

//======================================================================================================================================
// QUIVER ACHIEVEMENT PACK
//======================================================================================================================================
class CAchievementQuiver_KillsWith0Armor : public CBaseTFAchievement
{
	void Init()
	{
		SetFlags(ACH_LISTEN_KILL_EVENTS | ACH_SAVE_GLOBAL);
		SetGoal(1);
		m_iConsecutiveKills = 0;
	}

	virtual void ListenForEvents()
	{
		ListenForGameEvent("teamplay_round_active");
		ListenForGameEvent("localplayer_respawn");
	}

	void FireGameEvent_Internal(IGameEvent* event)
	{
		if (FStrEq(event->GetName(), "teamplay_round_active"))
		{
			m_iConsecutiveKills = 0;
		}
		else if (FStrEq(event->GetName(), "localplayer_respawn"))
		{
			m_iConsecutiveKills = 0;
		}
	}

	virtual void Event_EntityKilled(CBaseEntity* pVictim, CBaseEntity* pAttacker, CBaseEntity* pInflictor, IGameEvent* event)
	{
		C_BasePlayer* pLocalPlayer = C_BasePlayer::GetLocalPlayer();
		if (pLocalPlayer == pVictim)
		{
			m_iConsecutiveKills = 0;
		}
		else if (pLocalPlayer == pAttacker)
		{
			C_TFPlayer* pTFAttacker = ToTFPlayer(pAttacker);
			if (pTFAttacker && pTFAttacker->ArmorValue() <= 0)
			{
				m_iConsecutiveKills++;
				if (m_iConsecutiveKills >= GetNumKillsNeeded())
				{
					IncrementCount();
				}
			}
		}
	}

	virtual int GetNumKillsNeeded(void)
	{
		return 3;
	}

private:
	int m_iConsecutiveKills;
};
DECLARE_ACHIEVEMENT(CAchievementQuiver_KillsWith0Armor, ACHIEVEMENT_QUIVER_KILL3WITHARMORAT0, "QUIVER_KILL3WITHARMORAT0", 5);

class CAchievementQuiver_KillsWith0ArmorLarge : public CAchievementQuiver_KillsWith0Armor
{
	virtual int GetNumKillsNeeded(void)
	{
		return 6;
	}
};
DECLARE_ACHIEVEMENT(CAchievementQuiver_KillsWith0ArmorLarge, ACHIEVEMENT_QUIVER_KILL6WITHARMORAT0, "QUIVER_KILL6WITHARMORAT0", 5);

//----------------------------------------------------------------------------------------------------------------
class CAchievementQuiver_CaberKill : public CBaseTFAchievement
{
	void Init()
	{
		SetFlags(ACH_SAVE_GLOBAL);
		SetGoal(1);
		SetStoreProgressInSteam(true);
	}
};
DECLARE_ACHIEVEMENT(CAchievementQuiver_CaberKill, ACHIEVEMENT_QUIVER_CABERTAUNTKILL, "QUIVER_CABERTAUNTKILL", 5);

//----------------------------------------------------------------------------------------------------------------
class CAchievementQuiver_Assassination : public CBaseTFAchievement
{
	void Init()
	{
		SetFlags(ACH_SAVE_GLOBAL | ACH_LISTEN_KILL_ENEMY_EVENTS);
		SetGoal(1);
		SetStoreProgressInSteam(true);
	}

	virtual void Event_EntityKilled(CBaseEntity* pVictim, CBaseEntity* pAttacker, CBaseEntity* pInflictor, IGameEvent* event)
	{
		if (!pVictim || !pVictim->IsPlayer())
			return;

		if (pAttacker == C_BasePlayer::GetLocalPlayer() && event->GetInt("weaponid") == TF_WEAPON_REVOLVER)
		{
			C_TFPlayer* pTFVictim = ToTFPlayer(pVictim);
			if (pTFVictim && pTFVictim->IsPlayerClass(TF_CLASS_SNIPER))
			{
				if (FStrEq(event->GetString("weapon_logclassname", ""), "hitmans_executioner"))
				{
					if (IsHeadshot(event->GetInt("customkill")))
					{
						IncrementCount();
					}
				}
			}
		}
	}
};
DECLARE_ACHIEVEMENT(CAchievementQuiver_Assassination, ACHIEVEMENT_QUIVER_KILLINGHITMAN, "QUIVER_KILLINGHITMAN", 5);

#endif // CLIENT_DLL
