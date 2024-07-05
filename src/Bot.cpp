#include "../include/Bot.hpp"
#include "../include/Replies.hpp"
#include "../include/Server.hpp"

Bot::Bot(std::string name){
	name = "The knight";
}

Bot::~Bot() {}

Bot::Bot(const Bot &cp){
	if (this != &cp){
		setBotName(cp.getBotName());
	}
}

Bot &Bot::operator=(const Bot &og){
	if (this != &og){
		setBotName(og.getBotName());
	}
	return (*this);
}

void	Bot::setBotName(std::string name){
	this->name = name;
}
std::string	Bot::getBotName() const{
	return (this->name);
}

void	Bot::HelpCommands(Users *user){
	std::string Help = "You ask and you shall receive, here is the list of commands:\n\n"
	"KNIGHT TOPIC:   <Channel> <TOPIC>,                                    The TOPIC command serves the purpose of giving you the possibility of changing the Topic, or see the topic set for the channel\n"
	"KNIGHT KICK:    <Channel> <user>,                                        The KICK command ejects a user from the Channel.\n"
	"KNIGHT INVITE:  <NickName> <Channel>,                              The INVITE command hands you the power of inviting your peers to the Channel.\n"
	"KNIGHT PRIVMSG: <target>   <message>,                               It is quite self explanatory, this command is used to send a private message to someone.\n\n"
	"KNIGHT MODE: this is used to change Channel modes, for this you would need to be the OP, here are the modes available:\n"
	"KNIGHT +i: MODE #Channel <+/-i>                                         Set or remove the invite only for the channel.\n"
	"KNIGHT +t: MODE #Channel <+/-t>                                         Set or remove the Topic restrictions for the OP.\n"
	"KNIGHT +k: MODE #Channel <+/-K> <PassWord>                 Set or remove Channel Password.\n"
	"KNIGHT +l: MODE #Channel <+/-l> <int>                              Set or remove the limit of users in one Channel.\n"
	"KNIGHT +o: MODE #Channel <+/-o> <NickName>                Set or remove the OP privilages to a user.\n\n"
	"KNIGHT As mentioned before, some of these commands require you to be the OP, you can't use KICK and MODE if you are a user, unless some restrtictions have been softened for you.";
	user->setBuffer(RPL_BOT_HC(Help));
}

void	Bot::BotCommands(Users *user){
	std::string Help = "KNIGHT If you would like to know what i can do, here is a list of my capabilities: \n"
	"KNIGHT User list: (write <BOT UL>) gives you all the users that are in the same server as you.\n"
	"KNIGHT Channel list: (write <BOT CL>), shows you all the available Channels created by you or other users.\n"
	"KNIGHT EightBall: (write <BOT Eightball>), you summon my capabilities of second sight by advising you wether you should, or should not act upon what you ask me.\n"
	"KNIGHT d20: (write <BOT d20>), permits you to test your luck out of 20.\n";
	user->setBuffer(RPL_BOT_BC(Help));
}

void	Bot::UserList(Users *user, std::vector<Users *>	&AllUsers){
	std::string Help = "KNIGHT Here is the list for every user in the server:";
	for (std::vector <Users *>::iterator it = AllUsers.begin(); it != AllUsers.end(); it++){
		Help += (*it)->getNickname();
		Help += " / ";
	}
		user->setBuffer(RPL_BOT_UL(Help));
}

void	Bot::ChannelList(Users *user, std::vector<Channel *> &AllChannels){
	std::string everychannel;
	if (AllChannels.empty()){
		everychannel = "KNIGHT There are no Channels available currently, feel free to create one!";
	}
	else{
		everychannel = "KNIGHT Here is the list for every channel in the server: ";
		for (std::vector<Channel *>::iterator it = AllChannels.begin(); it != AllChannels.end(); it++){
			everychannel += (*it)->getName();
			everychannel += " / ";
	}
	}
	user->setBuffer(RPL_BOT_CL(everychannel));
}

Channel	*Bot::getChan(std::string name, std::vector<Channel *> AllChannels)
{
	for (std::vector<Channel *>::iterator it = AllChannels.begin();it != AllChannels.end(); ++it) {
		if ((*it)->getName() == name)
			return *it;
	}
	return NULL;
}

void	Bot::WhoisOperator(Users *user, Command &cmd, std::vector<Channel *> &AllChannels){
	std::string channel;
	if (AllChannels.empty()){
		std::string t = " :KNIGHT There are no Channels available currently, feel free to create one!";
		return user->setBuffer(RPL_BOT_WHOISOP(t));
	}
	else{
		std::string name = cmd.params[1];
		Channel *tmp = getChan(name, AllChannels);
		if (tmp)
		{
			if (!(tmp)->UserIsInC(user))
				{
					std::string n = (tmp)->getName();
					return user->setBuffer(ERR_NOTONCHANNEL(user->getHostname(), n));
				}
				channel = " :KNIGHT Here is the list of operators in this channel: ";
				for (std::vector<C_Users>::iterator i = (tmp)->UserList.begin(); i != (tmp)->UserList.end(); i++){
					if ((i)->flag == 1)
					{
						channel += (i)->user->getNickname();
						channel += " / ";
					}
				}
			user->setBuffer(RPL_BOT_WHOISOP(channel));
		}
		else
			return user->setBuffer(ERR_NOSUCHCHANNEL(user->getHostname(), name));
	}
}

void	Bot::EightBall(Users *user){
    static const char* responses[] = {
   	"KNIGHT It appears so.",
   	"KNIGHT You'll learn from this.",
   	"KNIGHT It may not go as planned.",
   	"KNIGHT Think further about it.",
   	"KNIGHT I wouldn't know.",
   	"KNIGHT I shouldn't say.",
   	"KNIGHT It is favorable.",
   	"KNIGHT Perhaps another time.",
   	"KNIGHT Guaranteed.",
   	"KNIGHT The time is right.",
   	"KNIGHT Don't let that distract you.",
   	"KNIGHT You have other things to worry about.",
   	"KNIGHT Very well.",
   	"KNIGHT There is a chance.",
   	"KNIGHT It could be done.",
   	"KNIGHT I would not do that.",
   	"KNIGHT Doubtful.",
   	"KNIGHT No doubt in me.",
   	"KNIGHT Maybe in another timeline.",
   	"KNIGHT It would mean farewell.",
   	"KNIGHT It would bring novelty.",
   	"KNIGHT Focus, and ask again when you know more."};

    const int num_responses = sizeof(responses) / sizeof(responses[0]);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, num_responses - 1);
    int random_index = dis(gen);
    std::string randomResponse = responses[random_index];
    user->setBuffer(RPL_BOT_EB(randomResponse));
}

void	Bot::d20(Users *user){
	std::random_device rd;
   std::mt19937 gen(rd());
	std::string str = "KNIGHT You have summoned the d20...";
   std::uniform_int_distribution<> dis(1, 20);
	int dice = dis(gen);
	str += std::to_string(dice);
	user->setBuffer(RPL_BOT_D20(str));
}

void	Bot::executeBot(Command cmd, Users *user, std::vector<Users *>users, std::vector<Channel *>Channels){
	if (cmd.Rest.find("Eightball") != std::string::npos)
	{
		int i = cmd.Rest.find(' ');
		cmd.Rest = cmd.Rest.substr(0, i);
	}
	if (cmd.Rest == "HELPC")
		HelpCommands(user);
	else if (cmd.Rest == "UL")
		UserList(user, users);
	else if (cmd.Rest == "d20")
		d20(user);
	else if (cmd.Rest == "Eightball")
		EightBall(user);
	else if (cmd.Rest == "CL")
		ChannelList(user, Channels);
	else if (cmd.Rest == "BOTC")
		BotCommands(user);
	else if (!cmd.params.empty() && cmd.params[0] == "WHOISOP")
		WhoisOperator(user, cmd, Channels);
	else
		user->setBuffer(ERR_UNKNOWNCOMMAND(user->getHostname(), cmd.Rest, user->getNickname()));
}
