#ifndef SERVERADMIN_COMMAND_TF_H
#define SERVERADMIN_COMMAND_TF_H

#include "admin\base_serveradmin.h"
#include "filesystem.h"
#include "tf_player.h"

#ifdef BDSBASE

#define TF_COMMAND_MODULE_NAME "Team Fortress 2 Commands"
#if defined(QUIVER_DLL)
#define QF_COMMAND_SUB_MODULE_NAME "Quiver Fortress Commands"
#endif

static void LoadTFCommandModule()
{
	// TODO: add commands
}

#endif

#endif // SERVERADMIN_COMMAND_TF_H