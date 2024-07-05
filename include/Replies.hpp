#pragma once

#define ERR_BADCHANMASK(chan)									": 476 " + chan + " :Bad Channel Mask\r\n"	   
#define ERR_NOSUCHNICK(src, nick, chan)					":" + src + " 401 " + nick + " " + chan + " :No such Nick/channel\r\n" //
#define ERR_NOSUCHSERVER(src, serv)							":" + src + " 402 " + serv + " :No such server\r\n"
#define ERR_NOSUCHCHANNEL(src, chan)						":" + src + " 403 " + chan + " :No such channel\r\n"
#define ERR_CANNOTSENDTOCHAN(src, chan)					":" + src + " 404 " + chan + " :Cannot send to channel\r\n"
#define ERR_TOOMANYCHANNELS(src, chan)						":" + src + " 405 " + chan + " :You have joined too many channels\r\n"
#define ERR_NOTEXTTOSEND(src, nick)							":" + src + " 412 " + nick + " :No text to send\r\n"
#define ERR_INPUTTOOLONG(src)									":" + src + " 417 :Input line too long\r\n"
#define ERR_UNKNOWNCOMMAND(src, cmd, nick)				":" + src + " 421 " + nick + " " + cmd + " :Unknown command\r\n" //
#define ERR_NONICKNAMEGIVEN(src)								":" + src + " 431 :No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME(src, nick)					":" + src + " 432 " + nick + " :Erroneus nickname\r\n"
#define ERR_NICKNAMEINUSE(src, nick)						":" + src + " 433 " + nick + " :Nickname is already in use\r\n"
#define ERR_USERNOTINCHANNEL(src, nick, chan)			":" + src + " 441 " + nick + " " + chan + " :The user does not belong to this channel\r\n" 
#define ERR_NOTONCHANNEL(src, chan)							":" + src + " 442 " + chan + " :You're not on that channel\r\n"
#define ERR_USERONCHANNEL(src, nick, tnick, chan)		":" + src + " 443 " + nick + " " + tnick + " " + chan + " :is already on channel\r\n"
#define ERR_NOTREGISTERED(source)							":" + source + " 451 :You have not registered\r\n"
#define ERR_NEEDMOREPARAMS(src, cmd)						":" + src + " 461 " + cmd + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTERED(src)							":" + src + " 462 :You may not reregister\r\n"
#define ERR_PASSWDMISMATCH(src)								":" + src + " 464 " "PASS" + " :Password incorrect\r\n"
#define ERR_CHANNELISFULL(src, chan)						":" + src + " 471 " + chan + " :Cannot join channel (+l)\r\n"
#define ERR_UNKNOWNMODE(src, mode)							":" + src + " 472 " + mode + " :is unknown mode char to me\r\n"
#define ERR_INVITEONLYCHAN(src, chan)						":" + src + " 473 " + chan + " :Cannot join channel (+i)\r\n"
#define ERR_BADCHANNELKEY(src, chan)						":" + src + " 475 " + chan + " :Cannot join channel (+k)\r\n"
#define ERR_NOPRIVILIGES(src)									":" + src + " 481 :Permission denied- You're not an IRC operator\r\n"
#define ERR_CHANOPRIVSNEEDED(src, chan)					":" + src + " 482 " + chan + " :You're not channel operator\r\n"
#define ERR_UMODEUNKNOWNFLAG(src)							":" + src + " 501 :Unknown MODE flag\r\n"
#define ERR_USERDONTMATCH(src)								":" + src + " 502 :Cant change mode for other users\r\n"
#define ERR_ALREADYOPERATOR(src, nick, chan, dest_nick)				":" + src + " 905 " + nick + " " + chan + " :" + dest_nick + " Already An operator\r\n"
#define ERR_INVALIDKEY(src, nick, chan)					":" + src + " 525 " + nick + " " + chan +  " :Cannot join channel (+k)\r\n"
#define ERR_INVALIDMODEPARAM(src, nick, chan, param, wprm)	":" + src + " 696 " + nick + " " + chan + " " + param + " " + wprm + " :Invalid mode parameter\r\n"

//replies
#define RPL_NAMERPLY(host, nick, chan, list)				":" + host + " 353 " + nick + " = " + chan + " :" + list + "\r\n" //
#define RPL_ENDOFNAMES(host, nick, chan)              ":" + host + " 366 " + nick + " " + chan + " :End of /NAMES list.\r\n" //
#define RPL_PRIVMSG(src, dis_nick, msg)					":" + src + " PRIVMSG " + dis_nick + " :" + msg + "\r\n" //
#define RPL_NICK(src, nick)									":" + src + " :Your nickname has been set to " + nick + "\r\n" //
#define RPL_PASS(src)											":" + src + " :Password is correct, you may continue the registration\r\n" //
#define RPL_USER(src, user)									":" + src + " :Your username is now set to: " + user + "\r\n"
#define RPL_NOTOPERATORANYMORE(src, chan)					":" + src + " :You are no longer an IRC operator in " + chan + "\r\n"
#define RPL_BECOMINGOPERATOR(src, chan)					":" + src + " :You are now an IRC operator in " + chan + "\r\n"
#define RPL_WHOISUSER(src, nick, user, username, host, rname)			":" + src + " 311 " + nick + " " + user + " " + username + " " + host + " * :" + rname + "\r\n"//
#define RPL_ENDOFWHOIS(src, user, name)										":" + src + " 318 " + user + " " + name + " :End of WHOIS list\r\n" //
#define RPL_CAP(src)												":" + src + " CAP * LS : you may enter PASS now...\r\n"
// #define RPL_NICKCHANGE(src, nick)							":" + src + " Your new nickname has been set to " + nick + "\r\n"
#define RPL_QUIT(src, nick, reply)							":" + src + " " + nick + " " + reply + "\r\n" 
#define RPL_KICK(src, chan, reason, user)					":" + src + " KICK " + chan + " " + user + " :" + reason + "\r\n" //
#define RPL_PART(src, chan, msg)                           ":" + src + " PART " + chan + " :" + msg + "\r\n"
#define RPL_WELCOME(src, user, host, nick)				":" + src + " 001 " + nick + " :Welcome to the IRC_server network " + nick + "!" + user + "@" + host + "\r\n" //
#define RPL_UMODEIS(src, nick, mode)						":" + src + " 221 " + nick + " " + mode + "\r\n"
#define RPL_CHANNELMODEIS(src, chan, nick,mode)				":" + src + " 324 " + nick + " " + chan + " " + mode + "\r\n"
#define RPL_NOTOPIC(src,nick, chan)								":" + src + " 331 " + nick + " " + chan + " :No topic is set\r\n"
#define RPL_TOPIC(host, nick, chan, topic)				":" + host + " 332 " + nick + " " + chan + " :" + topic + "\r\n"
#define RPL_NICKCHANGE(oldnick, user, host, newnick)  ":" + oldnick + "!" + user + "@" + host + " NICK :" + newnick + "\r\n" //
#define RPL_JOIN(nick, user, user_host, chan)							":" + nick + "!" + user + "@" + user_host + " JOIN :" + chan + "\r\n" //
#define RPL_INVITING(src, user, nick, chan)				":" + src + " 341 " + user + " " + nick + " " + chan + " :has been invited\r\n"

//Bot replies


#define RPL_BOT()
#define RPL_BOT_HC(msg)										":KNIGHT:  " + msg + "\r\n"
#define RPL_BOT_BC(msg)										": " + msg + "\r\n"
#define RPL_BOT_UL(help)									": " + help + " \r\n"
#define RPL_BOT_CL(help)									": " + help + " \r\n"
#define RPL_BOT_EB(help)									": " + help + " \r\n"
#define RPL_BOT_D20(help)									": " + help + " \r\n"
#define RPL_BOT_WELCOME(nick)								nick + ": Welcome to the server! my name is The Knight, if you need any help please type <BOT BOTC> or <BOT HELPC>, I will do my best to assist you\r\n"
#define RPL_BOT_CWELCOME(nick, chan)					":KNIGHT!KNIGHT@0.0.0.0 PRIVMSG " + chan + " : good folks, i would like you to please make " + nick + " feel at home within our noble walls\r\n"
#define RPL_BOT_WHOISOP(list)								list + "\r\n"
#define RPL_BOT_()


#define ERR_()
#define ERR_()
#define ERR_()
#define ERR_()
#define ERR_()
#define ERR_()
#define ERR_()
#define ERR_()
#define ERR_()
#define ERR_()
#define ERR_()
#define ERR_()
#define ERR_()
#define ERR_()
#define ERR_()