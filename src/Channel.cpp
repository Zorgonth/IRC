#include "../include/Channel.hpp"

Channel::Channel(std::string name){
	Name = name;
	Password = "random";
	Topic = "";
	limit = -1;
	Mode = "-iltk";
	Limitf  = false;
	Topicf = false;
	Invitef = false;
	Passf = false;
}

Channel::Channel(const	Channel &cp){
	if (this != &cp){
		setName(cp.getName());
		setPassword(cp.getPassword());
		setTopic(cp.getTopic());
		setLimit(cp.getLimit());
	}
}

Channel	&Channel::operator=(const Channel &og){
	if (this != &og){
		setName(og.getName());
		setPassword(og.getPassword());
		setTopic(og.getTopic());
		setLimit(og.getLimit());
	}
	return (*this);
}

Channel::~Channel(){}

void	Channel::setName(std::string name){
	this->Name = name;
}
void	Channel::setPassword(std::string password){
	this->Password = password;
}
void	Channel::setTopic(std::string topic){
	this->Topic = topic;
}
void	Channel::setLimit(int limit){
	this->limit = limit;
}
void	Channel::setQuote(std::string Quote){
	this->Quote = Quote;
}
void	Channel::setLimitf(bool limitf){
	this->Limitf = limitf;
}
void	Channel::setTopicf(bool topicf){
	this->Topicf = topicf;
}
void	Channel::setInvitef(bool	invitef){
	this->Invitef = invitef;
}
void	Channel::setPasswordf(bool passwordf){
	this->Passf = passwordf;
}
void	Channel::setKickf(bool Kickf){
	this->Kickf = Kickf;
}
std::string	Channel::getName() const{
	return (this->Name);
}
std::string	Channel::getPassword() const{
	return (this->Password);
}
std::string	Channel::getTopic() const{
	return (this->Topic);
}
std::string Channel::getQuote() const{
	return (this->Quote);
}

std::string Channel::getMode() const{
	return (this->Mode);
}

int		Channel::getLimit() const{
	int	lim = this->limit;
	return (lim);
}

bool	Channel::getLimitf() const
{
	return this->Limitf;
}

bool Channel::getInvitef() const
{
	return this->Invitef;
}

bool	Channel::getTopicf() const{
	return this->Topicf;
}
bool	Channel::getPasswordf() const{
	return this->Passf;
}
bool	Channel::getKickf() const{
	return this->Kickf;
}

int	Channel::getNumberofUsers(){
	return UserList.size();
}

std::string Channel::getAllUsersInChanList(std::vector<Users *> AllUsers)
{
	std::string list = "";
	for (std::vector<Users *>::iterator it = AllUsers.begin();it != AllUsers.end();it++)
	{
		for (std::vector<struct C_Users>::iterator t = UserList.begin();t != UserList.end();t++)
		{
			if ((*it)->getNickname() == t->user->getNickname())
			{
						list += (*t).user->getNickname();
							list += " ";
			}
		}
	}
	return list;
}

void	Channel::ClearPass()
{
	this->Password.clear();
}

std::vector<Users *> Channel::getAllUsersInChan(std::vector<Users *> AllUsers)
{
	std::vector<Users *> InChan;
	for (std::vector<Users *>::iterator it = AllUsers.begin();it != AllUsers.end();it++)
	{
		for (std::vector<struct C_Users>::iterator t = UserList.begin();t != UserList.end();t++)
		{
			if ((*it)->getNickname() == t->user->getNickname())
				InChan.push_back(*it);
		}
	}
	return InChan;
}

void	Channel::addUsertoC(Users *user){
	for (std::vector<struct C_Users>::iterator it = this->UserList.begin(); it != this->UserList.end(); it++){
		if ((*it).user->getNickname() == user->getNickname()){
			return ;
		}
	}
	C_Users tmp;
	tmp.user = user;
	tmp.flag = 0;
	this->UserList.push_back(tmp);
}

void	Channel::deleteUserfromC(Users *goner){
	for (std::vector<struct C_Users>::iterator it = this->UserList.begin(); it != this->UserList.end(); it++){
		if ((*it).user->getNickname()== goner->getNickname()){
			this->UserList.erase(it);
			return ;
		}
	}
}

bool	Channel::UserIsInC(Users *user)
{
	for (std::vector<struct C_Users>::iterator it = UserList.begin();it != UserList.end();it++)
	{
		if (user == (*it).user)
			return true;
	}
	return false;
}

void	Channel::fliptoOperator(Users	*user){
	std::string N = user->getNickname();
	for (std::vector<struct C_Users>::iterator it = this->UserList.begin(); it != this->UserList.end(); it++){
		if ((*it).user->getNickname() == N){
			if ((*it).flag == 1)
				(*it).flag = 0;
			else
				(*it).flag = 1;
		}
	}
}

bool	Channel::CheckifOP(Users *user){
	std::string N = user->getNickname();
	for (std::vector<struct C_Users>::iterator it = this->UserList.begin(); it != this->UserList.end(); it++){
		if ((*it).user->getNickname() == N){
			if ((*it).flag == 1)
				return true;
		}
	}
	return false;
}

void	Channel::brodcastMsg(std::string msg, std::vector<Users *> users)
{
	std::vector<Users *> InChan = getAllUsersInChan(users);
	for (std::vector<Users *>::iterator it = InChan.begin();it != InChan.end();it++)
		(*it)->setBuffer(msg);
}

void	Channel::brodcastMsgPriv(std::string msg, std::vector<Users *> users, Users *user)
{
	std::vector<Users *> InChan = getAllUsersInChan(users);
	for (std::vector<Users *>::iterator it = InChan.begin();it != InChan.end();it++)
	{
		if ((*it) == user)
			continue;
		(*it)->setBuffer(msg);
	}
}

void	Channel::ChangeMode()
{
	std::string Mode1 = "+";
	std::string Mode2 = "-";

	if (getInvitef() == false)
		Mode2 += "i";
	else
		Mode1 += "i";
	if (getLimitf() == false)
		Mode2 += "l";
	else
		Mode1 += "l";
	if (getPasswordf() == false)
		Mode2 += "k";
	else
		Mode1 += "k";
	if (getTopicf() == false)
		Mode2 += "t";
	else
		Mode1 += "t";
	if (Mode1.length() == 1)
	{
		Mode = Mode2;
		return ;
	}
	if (Mode2.length() == 1)
	{
		Mode = Mode1;
		return;
	}
	Mode = Mode1 + Mode2;
}