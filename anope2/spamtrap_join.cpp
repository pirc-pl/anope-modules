#include "module.h"
#include <fstream>
#include <iostream>

class SpamtrapJoin : public Module
{
 private:
 	std::vector<Anope::string> SpamtrapChannels;
 public:
	SpamtrapJoin(const Anope::string &modname, const Anope::string &creator) : Module(modname, creator)
	{
		Anope::string channel;

		Configuration::Block *b = Config->GetModule(this);
		if(!b) return;
		spacesepstream sep(b->Get<const Anope::string>("channels"));
		
		while (sep.GetToken(channel))
			this->SpamtrapChannels.push_back(channel);

	/*	this->SpamtrapChannels.push_back("#A117ZX2");
		this->SpamtrapChannels.push_back("#OP19AB1");
		this->SpamtrapChannels.push_back("#DW10190");
		this->SpamtrapChannels.push_back("#G19A79Z");*/
	}
	void OnJoinChannel(User *user, Channel *c)
	{
		if(user->IsProtected() || user->IsServicesOper() || user->IsIdentified()) return;
		
		if (user->Quitting() || !user->server->IsSynced())
			return;
		
		bool match = false;
		for(std::vector<Anope::string>::iterator it = this->SpamtrapChannels.begin(); it != SpamtrapChannels.end(); ++it) {
			if(it->equals_ci(c->name)){
				match = true;
				break;
			}
		}

		if(!match) return;

		bool dronebotEnable = false;
		Anope::string killReason;
		Anope::string dronebotChannel;
		Anope::string dronebotNick;

		Configuration::Block *b = Config->GetModule(this);		
		killReason = b->Get<const Anope::string>("killreason", "Didn't I tell you not to join?");
		dronebotChannel = b->Get<const Anope::string>("dronebotchannel", "#services");
		dronebotNick = b->Get<const Anope::string>("dronebotnick", "dronebot");
		dronebotEnable = b->Get<bool>("dronebot");

		if(user->GetIdent()[0] != '~'){
			Log(LOG_ADMIN) << "SPAMTRAP; " << user->GetDisplayedMask() << " joined " << c->name << " and has ident, ignored.";
			return;
		}
		BotInfo * bi = BotInfo::Find(Config->GetModule(this)->Get<const Anope::string>("operserv", "OperServ"), true);
		
		if(!bi){
			Log(LOG_ADMIN) << "ERROR: failure to obtain handle for OperServ!";
			return;
		}
		
		Log(LOG_ADMIN) << "SPAMTRAP: " << user->GetDisplayedMask() << " joined " << c->name << ", killing!";
		user->Kill(bi, killReason);
		
		if(dronebotEnable){		
			Anope::string droneCommand = dronebotNick + ": report " + user->ip.addr() + " 6 A_connection_from_this_host_joined_an_IRC_spam_trap.";
			IRCD->SendPrivmsg(bi, dronebotChannel, "%s", droneCommand.c_str());
		}
	}
};

MODULE_INIT(SpamtrapJoin)

