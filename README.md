# anope-modules
These are additional modules for [Anope](https://www.anope.org/). The modules are currently being used on the anope-2.0.6 version.

## ask_version
This one asks each connecting user for the CTCP VERSION reply (the request is sent from Global). The reply data is then logged (can be viewed on a log channel).

Load with `module { name = "ask_version" }` line in config file.

## os_tssync
This one reimplements the /OS TSSYNC command that still sometimes may be useful (for example one server had been started before the machine time was synchronized). It causes all servers' TSTIME to be forcibly set to services time.

Load with these lines in config file:

```C
module { name = "os_tssync" }
command { service = "OperServ"; name = "TSSYNC"; command = "operserv/tssync"; permission = "operserv/tssync"; }
```

## spamtrap_join
This one works together with [Gottem's m_listrestrict UnrealIRCd module](https://gitgud.malvager.net/Wazakindjes/unrealircd_mods/#m_listrestrict). The listrestrict module should be configured with some fake channels and no gline option. Clients joining honeypot channels (you should protect them from joining by other users: at least set +sP and register) will be killed and reported in services log. You can also configure it to automatically reports all the hosts to DroneBL using [node-droneircbot](https://github.com/pirc-pl/utils/tree/master/node-droneircbot).

To work correctly, it needs the following config:

```C
module {
	name = "spamtrap_join";
	killreason = "You shouldn't have joined!"; // clients joining the channels will be killed with this message
	dronebot = yes; // comment this line if you don't want to use dronebl reporting via dronebot
	dronebotnick = "dronebot"; // nick of dronebot
	dronebotchannel = "#services"; // channel, where dronebot waits for commands
	channels = "#asdf #qwerty #zxcvbnm #123"; // list of channels, separated by spaces
}
```

## ns_login
### (not used on PIRC)
A hybrid of "RECOVER" and "IDENTIFY" commands of NickServ. This will identify you to a nickname, and change the nick if it does not match the requested one.
