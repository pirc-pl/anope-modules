#include "module.h"
#define VER "1.0"
/************************************************
* TSSync Module
* 
* Examples taken from "os_nofakelag" by Cronus
* 
* Author: k4be
*
* For UnrealIRCd ONLY!
*
*************************************************
* Syntax: /msg %s TSSYNC
* Adds command operserv/tssync
*************************************************/

class CommandOSTssync : public Command
{
public:
	CommandOSTssync(Module *creator) : Command(creator, "operserv/tssync", 0, 0)
	{
		this->SetDesc(_("Set global TS Time on unrealircd"));
		this->SetSyntax("");
	}

	void Execute(CommandSource &source, const std::vector<Anope::string> &params)
	{
		User *u = source.GetUser();
		
		time_t now = time(NULL);

		if (!u) {
			return; 
		}
		
		else if (!source.IsServicesOper())
		{
			source.Reply(ACCESS_DENIED);
		}
		else
		{
			Log(LOG_ADMIN, source, this) << "to sync all servers' TS Time (TS: " << (long int) now << ")";
			source.Reply(_("You have set global TS Time to \002%ld\002"), (long int) now);
			UplinkSocket::Message() << "TSCTL SVSTIME " << (long int) now;
		}
		return;
	} 
	bool OnHelp(CommandSource &source, const Anope::string &subcommand)
	{ 
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply(_("Set global TS Time on unrealircd"));
		return true;
	}
};

class OSTssync : public Module
{
	CommandOSTssync commandostssync;
	
public:
	OSTssync(const Anope::string &modname, const Anope::string &creator) : Module(modname, creator),	
		commandostssync(this)
	{
		if (!ModuleManager::FindModule("unreal") && !ModuleManager::FindModule("unreal4")) 
		{ 
			Log() << "ERROR: You are not running UnrealIRCd, this module only works on UnrealIRCd.";
			return;
		}
		this->SetAuthor("k4be");
		this->SetVersion(VER);
	}
};

MODULE_INIT(OSTssync) 
