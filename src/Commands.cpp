#include "../include/Server.hpp"
#include "../include/Lib.hpp"
#include "../include/Replies.hpp"

void	Server::AddPtoUser(Command cmd, Users *user){
	std::string pword = cmd.Rest;
	if (user->getStatus() == 0)
		return (user->setBuffer(ERR_NOTREGISTERED(this->getHost())));
	if (user->getStatus() == 1){
		if (!pword.empty() && pword == this->Password){
			user->setStatus(2);
			user->setBuffer(RPL_PASS(user->getHostname()));
		}
		else if (pword.empty())
			user->setBuffer(ERR_NEEDMOREPARAMS(this->getHost(), cmd.CmdName));
		else {
			user->setBuffer(ERR_PASSWDMISMATCH(this->getHost()));
		}
	}
	else if (user->getStatus() >= 2){
		user->setBuffer(ERR_ALREADYREGISTERED(this->getHost()));
	}
}

void	Server::AddNicktoUser(Command cmd, Users *user){
	if (user->getStatus() < 2)
		return (user->setBuffer(ERR_NOTREGISTERED(this->getHost())));
	if (user->getStatus() == 4 && user->getNickname() != "*")
	{
		std::string Nick = cmd.Rest;
		for (std::vector<Users *>::iterator it = this->AllUsers.begin(); it != this->AllUsers.end(); it++){
				if ((*it)->getNickname() == Nick){
					user->setBuffer(ERR_NICKNAMEINUSE(this->getHost(), Nick));
					return ;
				}
		}
		user->setBuffer(RPL_NICKCHANGE(user->getNickname(), user->getUsername(), user->getHostname(), cmd.Rest));
		return user->setNickname(cmd.Rest);
	}
	if (user->getStatus() >= 2 && user->getStatus() < 4)
	{
		std::string Nick = cmd.Rest;
		for (std::vector<Users *>::iterator it = this->AllUsers.begin(); it != this->AllUsers.end(); it++){
				if ((*it)->getNickname() == Nick){
					user->setBuffer(ERR_NICKNAMEINUSE(this->getHost(), Nick));
					return ;
				}
		}
		user->setNickname(cmd.Rest);
		user->setBuffer(RPL_NICK(user->getHostname(), user->getNickname()));
		if (user->getStatus() == 3 && user->getUsername() == "*")
			return ;
		user->setStatus(user->getStatus() + 1);
	}
	if (user->getStatus() == 4)
		return (user->setBuffer(RPL_WELCOME(this->getHost(), user->getUsername(), user->getHostname(), user->getNickname()))); //rpl_welcome;
	return ;
}

void	Server::CapInit(Command cmd, Users *user)
{
	if (cmd.CmdName == "CAP")
	{
		if (cmd.Rest == "LS")
		{
			if (user->getStatus() == 0)
				user->setStatus(1);
			user->setBuffer(RPL_CAP(user->getHostname()));
		}
	}
}

void	Server::RegisterUser(Command cmd, Users *user)
{
	if (user->getStatus() == 4 && user->getUsername() != "*")
		return user->setBuffer(ERR_ALREADYREGISTERED(this->getHost()));
	if (user->getStatus() < 2)
		return (user->setBuffer(ERR_NOTREGISTERED(this->getHost())));
	if (user->getStatus() >= 2)
	{
		std::vector<std::string>::iterator it = cmd.params.begin();
		std::string username = *it;
		it = cmd.params.end();
		it--;
		std::string realname = *it;
		user->setRealname(realname);
		user->setUsername(username);
		user->setBuffer(RPL_USER(user->getHostname(), user->getUsername()));
		if (user->getStatus() == 3 && user->getNickname() == "*")
			return ;
		user->setStatus(user->getStatus() + 1);
	}
	if (user->getStatus() == 4)
		return (user->setBuffer(RPL_WELCOME(this->getHost(), user->getUsername(), user->getHostname(), user->getNickname())));
	return ;
}

void	Server::SendPong(Command msg, Users *user)
{
	if (msg.Rest.empty())
		return user->setBuffer("");
	user->setBuffer(msg.Rest + "\r\n");
}

static Channel* CheckChannel(const std::vector<Channel*>& allChannels, const std::string& target) {
    for (std::vector<Channel*>::const_iterator it = allChannels.begin(); it != allChannels.end(); ++it) {
        if ((*it)->getName() == target)
            return *it;
    }
    return NULL;
}


void Server::c_topic(Command cmd, Users *user)
{
   std::string ChannelName = cmd.params[0];
   Channel *Channel= CheckChannel(AllChannels, ChannelName);
   if (Channel == NULL)
        return(user->setBuffer(ERR_NOSUCHCHANNEL(this->getHost(), ChannelName)));
	if (!Channel->UserIsInC(user))
		return(user->setBuffer(ERR_NOTONCHANNEL(this->getHost(), Channel->getName())));
	if (cmd.params.size() == 1)
	{
		if (Channel->getTopic().empty())
			return user->setBuffer(RPL_NOTOPIC(this->getHost(), user->getNickname(), Channel->getName()));
		return user->setBuffer(RPL_TOPIC(this->getHost(), user->getNickname(), Channel->getName(), Channel->getTopic()));
	}
	else
	{
		if (!Channel->getTopicf())
		{
			Channel->setTopic(cmd.params[1]);
			return Channel->brodcastMsg((RPL_TOPIC(this->getHost(), user->getNickname(), Channel->getName(), Channel->getTopic())), AllUsers);
		}
		else
		{
			if (!Channel->CheckifOP(user))
				return (user->setBuffer(ERR_CHANOPRIVSNEEDED(this->getHost(), Channel->getName())));
			else
			{
				Channel->setTopic(cmd.params[1]);
				return Channel->brodcastMsg((RPL_TOPIC(this->getHost(), user->getNickname(), Channel->getName(), Channel->getTopic())), AllUsers);
			}
		}
	}
}


std::vector<std::string> getTargets(std::string targets)
{
	std::vector<std::string> res;
	std::string tmp;
	if (targets.find(',') == std::string::npos)
		return (res.push_back(RSpaces(targets)), res);
	int i = 0;
	while (targets[i])
	{
		int j = 0;
		if (targets.find(',') != std::string::npos)
		{
			i = targets.find(',');
			tmp = targets.substr(j, i);
			if (!tmp.empty())
				res.push_back(tmp);
			targets = targets.substr(i + 1, targets.length());
			targets = RSpaces(targets);
			i = 0;
		}
		else
		{
			targets = RSpaces(targets);
			res.push_back(targets);
			break ;
		}
	}
	return res;
}

void	Server::c_privmsg(Command cmd, Users *user)
{
	std::vector<std::string> targets = getTargets(cmd.params[0]);
	if (targets.size() == 0)
		return ;
	for (std::vector<std::string>::iterator it = targets.begin();it != targets.end();it++)
	{
		if (OnlySpaces(*it))
		{
			user->setBuffer(ERR_NOSUCHNICK(this->getHost(), user->getNickname(), *it));
			continue;
		}
		if ((*it)[0] == '#')
		{
			if (getChannel(*it) == NULL)
			{
				user->setBuffer(ERR_NOSUCHCHANNEL(this->getHost(), *it));
				continue;
			}
			if (!getChannel(*it)->UserIsInC(user))
			{
				user->setBuffer(ERR_NOTONCHANNEL(this->getHost(), *it));
				continue;
			}
			getChannel(*it)->brodcastMsgPriv(RPL_PRIVMSG(user->getSrc(), *it, cmd.params[1]), AllUsers, user);
		}
		else
		{
			if (!getUserByNn(*it))
			{
				user->setBuffer(ERR_NOSUCHNICK(this->getHost(), user->getNickname(), *it));
				continue;
			}
			getUserByNn(*it)->setBuffer(RPL_PRIVMSG(user->getSrc(), *it, cmd.params[1]));
		}
	}
}

void	Server::join(Command cmd, Users *user)
{
	std::vector<std::string> chan_name = getTargets(cmd.params[0]);
	std::vector<std::string> key_lst;
	if (cmd.params.size() == 2)
		key_lst = getTargets(cmd.params[1]);
	for (std::vector<std::string>::iterator it = chan_name.begin(); it != chan_name.end(); ++it) {
		Channel *chan = getChannel(*it);
		if (!chan) {
			if (!CheckChannelName(*it))
			{
				user->setBuffer(ERR_BADCHANMASK(*it));
				continue;
			}
			chan = new Channel(*it);
			this->AddChanToServ(chan);
			chan->addUsertoC(user);
			chan->fliptoOperator(user);
			if (!key_lst.empty()) {
				std::string key = *(key_lst.begin());
				key_lst.erase(key_lst.begin());
				chan->setPassword(key);
				chan->setPasswordf(true);
				chan->ChangeMode();
			}
			user->setBuffer(RPL_JOIN(user->getNickname(), user->getUsername(), user->getHostname(), chan->getName()));
		}
		else {
			if (chan->UserIsInC(user))
			{
				user->setBuffer(ERR_USERONCHANNEL(this->getHost(), user->getNickname(), user->getNickname(), chan->getName()));
				continue;
			}
			if (chan->getLimitf())
			{
				if (chan->getLimit() >= chan->getNumberofUsers())
				{
					user->setBuffer(ERR_CHANNELISFULL(user->getNickname() + "!" + user->getUsername() + "@" + user->getHostname(), chan->getName()));
					continue;
				}
			}
			if (chan->getInvitef())
			{
				user->setBuffer(ERR_INVITEONLYCHAN(user->getNickname() + "!" + user->getUsername() + "@" + user->getHostname(), chan->getName()));
				continue;
			}
			if (chan->getPasswordf()) {
				if (!key_lst.empty())
				{
					std::string key = *(key_lst.begin());
					key_lst.erase(key_lst.begin());
					if (chan->getPassword() == key) {
						chan->brodcastMsg(RPL_JOIN(user->getNickname(), user->getUsername(), user->getHostname(), chan->getName()), AllUsers);
						chan->addUsertoC(user);
						chan->brodcastMsg(RPL_BOT_CWELCOME(user->getNickname(), chan->getName()), AllUsers);
						if (chan->getTopic().empty())
							user->setBuffer(RPL_NOTOPIC(this->getHost(), user->getNickname(), chan->getName()));
						else
							user->setBuffer(RPL_TOPIC(this->getHost(), user->getNickname(), chan->getName(), chan->getTopic()));
						user->setBuffer(RPL_NAMERPLY(this->getHost(), user->getNickname(), chan->getName(), chan->getAllUsersInChanList(AllUsers)));
						user->setBuffer(RPL_ENDOFNAMES(this->getHost(), user->getNickname(), chan->getName()));
						user->setBuffer(RPL_JOIN(user->getNickname(), user->getUsername(), user->getHostname(), chan->getName()));
					}
					else
						user->setBuffer(ERR_BADCHANNELKEY(user->getNickname() + "!" + user->getUsername() + "@" + user->getHostname(), chan->getName()));
				}
				else
					user->setBuffer(ERR_BADCHANNELKEY(user->getNickname() + "!" + user->getUsername() + "@" + user->getHostname(), chan->getName())); 
			}
			else {
				chan->brodcastMsg(RPL_JOIN(user->getNickname(), user->getUsername(), user->getHostname(), chan->getName()), AllUsers);
				chan->addUsertoC(user);
				chan->brodcastMsg(RPL_BOT_CWELCOME(user->getNickname(), chan->getName()), AllUsers);
				if (chan->getTopic().empty())
					user->setBuffer(RPL_NOTOPIC(this->getHost(), user->getNickname(), chan->getName()));
				else
					user->setBuffer(RPL_TOPIC(this->getHost(), user->getNickname(), chan->getName(), chan->getTopic()));
				user->setBuffer(RPL_NAMERPLY(this->getHost(), user->getNickname(), chan->getName(), chan->getAllUsersInChanList(AllUsers)));
				user->setBuffer(RPL_ENDOFNAMES(this->getHost(), user->getNickname(), chan->getName()));
				user->setBuffer(RPL_JOIN(user->getNickname(), user->getUsername(), user->getHostname(), chan->getName()));
			}

			}
		}	

	}

void	Server::c_kick(Command cmd, Users *user)
{
	for (std::vector<std::string>::iterator it = cmd.params.begin();it != cmd.params.end();it++)
		std::cout << *it << std::endl;
   std::string channel = cmd.params[0];
   std::vector<std::string> target = getTargets(cmd.params[1]);
   if (channel.empty())
       return(user->setBuffer(ERR_NEEDMOREPARAMS(user->getHostname(), cmd.CmdName)));
   Channel *cnl = getChannel(channel);
   if (cnl == NULL)
       return(user->setBuffer(ERR_NOSUCHCHANNEL(user->getHostname(), channel)));
   if (!cnl->UserIsInC(user))
       return(user->setBuffer(ERR_NOTONCHANNEL(user->getHostname(), channel)));
   if (!cnl->CheckifOP(user))
           return(user->setBuffer(ERR_CHANOPRIVSNEEDED(user->getHostname(), channel)));
   for (std::vector<std::string>::const_iterator it = target.begin(); it != target.end(); ++it) {
      Users *tar = getUserByNn(*it);
		if (tar == NULL)
		{
			user->setBuffer(ERR_NOSUCHNICK(this->getHost(), user->getNickname(), *it));
			continue;
		}
		if (tar->getNickname() == user->getNickname())
		{
			continue;
		}
      if (!cnl->UserIsInC(tar))
		{
         (user->setBuffer(ERR_USERNOTINCHANNEL(user->getHostname(),tar->getNickname(), channel)));
			continue; 
		}
      cnl->deleteUserfromC(tar);
		tar->setBuffer(RPL_KICK(user->getSrc(), cnl->getName(), cmd.params[2], tar->getNickname()));
		cnl->brodcastMsg(RPL_KICK(user->getSrc(), cnl->getName(), cmd.params[2], tar->getNickname()), AllUsers);
   }
}

void	Server::Whois(Users *user, Command cmd){
	std::string name = cmd.Rest;
	Users *u = getUserByNn(name);
	if (u == NULL)
		return user->setBuffer(ERR_NOSUCHNICK(this->getHost(), user->getNickname(), name));
	user->setBuffer(RPL_WHOISUSER(this->getHost(), user->getNickname(), name, u->getUsername(), u->getHostname(), u->getRealname()));
	user->setBuffer(RPL_ENDOFWHOIS(this->getHost(), user->getNickname(), name));
}


void Server::c_invite(Command cmd, Users *user)
{
   std::string channel = cmd.params[1];
   std::string target = cmd.params[0];

   if (user->getStatus() < 2)
	return (user->setBuffer(ERR_NOTREGISTERED(this->getHost())));
   Channel *cnl = getChannel(channel);
   if (!cnl)
       return(user->setBuffer(ERR_NOSUCHCHANNEL(this->getHost(), channel)));
   if (!cnl->UserIsInC(user))
       return(user->setBuffer(ERR_NOSUCHCHANNEL(this->getHost(), channel)));
   Users *invitee = getUserByNn(target);
   if (invitee == NULL)
       return user->setBuffer(ERR_NOSUCHNICK(this->getHost(), target, cnl->getName())); 
   if (cnl->UserIsInC(invitee))
       return(user->setBuffer(ERR_USERONCHANNEL(this->getHost(), user->getNickname(), target, cnl->getName())));
   if (cnl->getInvitef())
   {
      if (cnl->CheckifOP(user))
      {
			user->setBuffer(RPL_INVITING(this->getHost(), user->getNickname(), invitee->getNickname(), cnl->getName()));
			cnl->brodcastMsg(RPL_JOIN(invitee->getNickname(), invitee->getUsername(), invitee->getHostname(), cnl->getName()), AllUsers);
      	cnl->addUsertoC(invitee);
			cnl->brodcastMsg(RPL_BOT_CWELCOME(invitee->getNickname(), cnl->getName()), AllUsers);
			if (cnl->getTopic().empty())
					invitee->setBuffer(RPL_NOTOPIC(this->getHost(), invitee->getNickname(), cnl->getName()));
				else
					invitee->setBuffer(RPL_TOPIC(this->getHost(), invitee->getNickname(), cnl->getName(), cnl->getTopic()));
			invitee->setBuffer(RPL_NAMERPLY(this->getHost(), invitee->getNickname(), cnl->getName(), cnl->getAllUsersInChanList(AllUsers)));
			invitee->setBuffer(RPL_ENDOFNAMES(this->getHost(), invitee->getNickname(), cnl->getName()));
			invitee->setBuffer(RPL_JOIN(invitee->getNickname(), invitee->getUsername(), invitee->getHostname(), cnl->getName()));
         return ;
      }
      return(user->setBuffer(ERR_CHANOPRIVSNEEDED(this->getHost(), channel)));
   }
	user->setBuffer(RPL_INVITING(this->getHost(), user->getNickname(), invitee->getNickname(), cnl->getName()));
	cnl->brodcastMsg(RPL_JOIN(invitee->getNickname(), invitee->getUsername(), invitee->getHostname(), cnl->getName()), AllUsers);
	cnl->addUsertoC(invitee);
	cnl->brodcastMsg(RPL_BOT_CWELCOME(invitee->getNickname(), cnl->getName()), AllUsers);
	if (cnl->getTopic().empty())
			invitee->setBuffer(RPL_NOTOPIC(this->getHost(), invitee->getNickname(), cnl->getName()));
		else
			invitee->setBuffer(RPL_TOPIC(this->getHost(), invitee->getNickname(), cnl->getName(), cnl->getTopic()));
	invitee->setBuffer(RPL_NAMERPLY(this->getHost(), invitee->getNickname(), cnl->getName(), cnl->getAllUsersInChanList(AllUsers)));
	invitee->setBuffer(RPL_ENDOFNAMES(this->getHost(), invitee->getNickname(), cnl->getName()));
	invitee->setBuffer(RPL_JOIN(invitee->getNickname(), invitee->getUsername(), invitee->getHostname(), cnl->getName()));
}

std::vector<std::string> split(const std::string &str, const char *delimiter) {
    std::vector<std::string> tokens;
    char *cstr = new char[str.length() + 1];
    std::strcpy(cstr, str.c_str());

    char *token = std::strtok(cstr, delimiter);
    while (token != NULL) {
        tokens.push_back(std::string(token));
        token = std::strtok(NULL, delimiter);
    }

    delete[] cstr;
    return tokens;
}

void Server::c_part(Command cmd, Users* user)
{
    if (cmd.params[0].empty())
    {
        user->setBuffer(ERR_NEEDMOREPARAMS(this->getHost(), cmd.CmdName));
        return;
    }
    
    std::string reason = cmd.params[1];
    std::vector<std::string> channels = split(cmd.params[0], ",");
    
    for (std::vector<std::string>::const_iterator it = channels.begin(); it != channels.end(); ++it)
    {
        Channel *channel = getChannel(*it);
        
        if (channel == NULL)
        {
            user->setBuffer(ERR_NOSUCHCHANNEL(this->getHost(), *it));
            continue;
        }
        
        if (!channel->UserIsInC(user))
        {
            user->setBuffer(ERR_NOTONCHANNEL(this->getHost(), user->getNickname()));
            continue;
        }
			int opc = 0;
			for (std::vector<struct C_Users>::iterator i = channel->UserList.begin(); i != channel->UserList.end(); )
            {
                if (i->flag == 1)
                {
                    opc +=1; 
                }
				i++;
            }
        channel->deleteUserfromC(user);
        user->setBuffer(RPL_PART(user->getSrc(), *it, reason));
		if (opc <= 1)
		{
			channel->UserList[0].flag = 1;
			channel->UserList[0].user->setBuffer(RPL_BECOMINGOPERATOR(this->getHost(), channel->getName()));
		}
        if (channel->UserList.size() == 0)
        {
            for (std::vector<Channel*>::iterator i = AllChannels.begin(); i != AllChannels.end();i++ )
            {
                if ((*i)->getName() == *it)
                {
                    AllChannels.erase(i);
						  delete channel;
						  return ;
                }
            }
        }
			
        
        channel->brodcastMsg(RPL_PART(user->getSrc(), *it, reason), AllUsers);
    }
}

void Server::c_mode(Command cmd, Users *user) 
{
    std::string target = cmd.params[0];
    Channel *channel = getChannel(target);
    if (channel == NULL)
        return(user->setBuffer(ERR_NOSUCHCHANNEL(this->getHost(), target)));
    if (cmd.params.size() == 1)
    {
        return (user->setBuffer(RPL_CHANNELMODEIS(user->getSrc(), channel->getName(), user->getNickname(), channel->getMode())));
    }
    std::string modeString = cmd.params[1];
    std::string currentModes = channel->getMode();
    if (!channel->CheckifOP(user))
        return (user->setBuffer(ERR_NOPRIVILIGES(this->getHost())));
    switch (modeString[1])
    {
        case 'i':
            if (modeString[0] == '-')
                channel->setInvitef(false);
            else
                channel->setInvitef(true);
            break;
        case 't':
            if (modeString[0] == '-')
                channel->setTopicf(false);
            else
                channel->setTopicf(true);
            break;
        case 'l':
            if (modeString[0] == '-')
                channel->setLimitf(false);
            else
            {
                if (cmd.params.size() != 3)
                    return (user->setBuffer(ERR_NEEDMOREPARAMS(this->getHost(), "MODE")));
                if (!IfNumbers(cmd.params[2]))
                    return (user->setBuffer(ERR_INVALIDMODEPARAM(this->getHost(), user->getNickname(), channel->getName(), modeString[1], cmd.params[2])));
                channel->setLimitf(true);
                channel->setLimit(std::atoi(cmd.params[2].c_str()));
            }
            break;
        case 'k':
            if (modeString[0] == '-')
				{
					channel->ClearPass();
               channel->setPasswordf(false);
				}
            else
            {
                if (cmd.params.size() != 3)
                    return (user->setBuffer(ERR_NEEDMOREPARAMS(this->getHost(), "MODE")));
                if (!IfValid(cmd.params[2]))
                    return (user->setBuffer(ERR_INVALIDKEY(this->getHost(), user->getNickname(), channel->getName())));
                channel->setPasswordf(true);
                channel->setPassword(cmd.params[2]);
            }
            break;
        case 'o':
            if (cmd.params.size() != 3)
                return (user->setBuffer(ERR_NEEDMOREPARAMS(this->getHost(), "MODE")));
            if (!getUserByNn(cmd.params[2]))
                return (user->setBuffer(ERR_NOSUCHNICK(this->getHost(), user->getNickname(), channel->getName())));
            if (!channel->UserIsInC(getUserByNn(cmd.params[2])))
                return (user->setBuffer(ERR_USERNOTINCHANNEL(this->getHost(), user->getNickname(), channel->getName())));
            if (modeString[0] == '-')
            {
                if (!channel->CheckifOP(getUserByNn(cmd.params[2])))
                    return (user->setBuffer(ERR_CHANOPRIVSNEEDED(this->getHost(), channel->getName())));
                channel->fliptoOperator(getUserByNn(cmd.params[2]));
                getUserByNn(cmd.params[2])->setBuffer(RPL_NOTOPERATORANYMORE(this->getHost(), channel->getName()));
            }
            else
            {
                if (channel->CheckifOP(getUserByNn(cmd.params[2])))
                    return (user->setBuffer(ERR_ALREADYOPERATOR(this->getHost(), user->getNickname(), channel->getName(), cmd.params[3])));
                channel->fliptoOperator(getUserByNn(cmd.params[2]));
                getUserByNn(cmd.params[2])->setBuffer(RPL_BECOMINGOPERATOR(this->getHost(), channel->getName()));
            }
            break;
        default:
            user->setBuffer(ERR_UNKNOWNMODE(this->getHost(), modeString));
            break;
    }
    channel->ChangeMode();
    channel->brodcastMsg(RPL_CHANNELMODEIS(user->getSrc(), channel->getName(), user->getNickname(), channel->getMode()), AllUsers);
}