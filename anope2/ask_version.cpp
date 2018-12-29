#include "module.h"

class AskVersion : public Module
{
 public:
	AskVersion(const Anope::string &modname, const Anope::string &creator) : Module(modname, creator)
	{
	}
	void OnUserConnect(User *user, bool &)
	{
		if (user->Quitting() || !user->server->IsSynced())
			return;
		BotInfo * bi = BotInfo::Find(Config->GetModule(this)->Get<const Anope::string>("global_announcer", "Global"), true);
		if (bi == NULL)
			return;
		IRCD->SendPrivmsg(bi, user->GetUID(), "\1VERSION\1");
	}
	void OnBotNotice(User *user, BotInfo *bi, Anope::string &message){
		if (message[0] != '\1' || message[message.length() - 1] != '\1') return;
		if (!message.substr(0, 9).equals_ci("\1VERSION ")) return;
		Anope::string versionstr = Anope::NormalizeBuffer(message.substr(9, message.length() - 10));
		Log(LOG_ADMIN) << "VERSION reply for " << user->nick << ": " << versionstr;
	}
};

MODULE_INIT(AskVersion)
