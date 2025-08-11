#ifndef SERVERADMIN_COMMAND_REGISTER_H
#define SERVERADMIN_COMMAND_REGISTER_H

// ADD NEW MODULES HERE
#include "command\serveradmin_command_base.h"

#ifdef BDSBASE
//THEN ADD THEM HERE SO THAT THEY CAN BE LOADED
static void RegisterCommands(void)
{
	LoadBaseCommandModule();
}
#endif

#endif // SERVERADMIN_COMMAND_REGISTER_H