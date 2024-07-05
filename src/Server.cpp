#include "../include/Server.hpp"
#include "../include/Replies.hpp"

Server::Server(){}

Server::Server(int port, std::string password)
{
	Port = port;
	Password = password;
	this->setStatus(true);
	std::cout << GREEN << "-------STARTING---SERVER-------" << DEFAULT << std::endl;
}

Server::~Server()
{
	for (std::vector<Channel *>::iterator it = AllChannels.begin();it != AllChannels.end();it++)
		delete *it;
	for (std::vector<Users *>::iterator it = AllUsers.begin();it != AllUsers.end();it++)
		delete *it;
	AllChannels.clear();
	AllUsers.clear();
}

Server::Server(const Server &cp)
{
	*this = cp;
}

Server &Server::operator=(const Server &cp)
{
	if (this != &cp)
		*this = cp;
	return *this;
}

void Server::setHost(std::string name)
{
	Host = name;
}

std::string Server::getHost() const
{
	return this->Host;
}

bool	Server::getStatus() const
{
	return ServerStatus;
}

void	Server::setStatus(bool flag)
{
	ServerStatus = flag;
}

int	Server::getPort() const
{
	return this->Port;
}

void	Server::addPfds(struct pollfd sfd) 
{
	this->Fds.push_back(sfd);
}

void	Server::setBytesReceived(int BytesReceived)
{
	this->BytesReceived = BytesReceived;
}

void	Server::AddChanToServ(Channel *channel)
{
	AllChannels.push_back(channel);
}

ssize_t Server::getBytesReceived()
{
	return this->BytesReceived;
}

void	Server::setAllUsers(std::vector<Users *> user){
	AllUsers = user;
}

void	Server::setAllChannels(std::vector<Channel *> vec){
	AllChannels = vec;
}

std::vector<Users *> Server::getVectorU() const{
	return this->AllUsers;
}

std::vector<Channel *> Server::getVectorCh() const{
	return this->AllChannels;
}

Channel *Server::getChannel(const std::string cname) {
	for (std::vector<Channel *>::iterator it = this->AllChannels.begin();
			it != this->AllChannels.end(); ++it) {
		if ((*it)->getName() == cname)
			return *it;
	}
	return NULL;
}

Users *Server::getUserByFd(int fd) {
	for (std::vector<Users *>::iterator it = this->AllUsers.begin(); 
			it != this->AllUsers.end(); ++it) {
		if ((*it)->getSocket() == fd)
			return *it;
	}
	return NULL;
}


void	Server::removePfds(struct pollfd sfd)
{
	for (std::vector<struct pollfd>::iterator it = this->Fds.begin(); it != this->Fds.end(); ++it) {
		if ((*it).fd == sfd.fd && ((*it).events == sfd.events)) {
			this->Fds.erase(it);
			return ;
		}
	}
}

Users *Server::getUserByNn(std::string nname)
{
	for (std::vector<Users *>::iterator it = this->AllUsers.begin();it != this->AllUsers.end();it++) {
		if ((*it)->getNickname() == nname)
			return *it;
	}
	return NULL;
}


void  Server::addUser(Users *user)
{
  this->AllUsers.push_back(user);
}

void	Server::removeUserFromServer(Users *user)
{
	for (std::vector<pollfd>::iterator it = this->Fds.begin(); it != this->Fds.end(); it++)
	{
		if ((*it).fd == this->ServerSocket)
			continue ;
		if ((*it).fd == user->getSocket()) 
		{
			this->Fds.erase(it);
			break ;
		}
	}
	for (std::vector<Users *>::iterator it = this->AllUsers.begin(); it != this->AllUsers.end(); ++it) {
		if ((*it) == user) 
		{
			this->AllUsers.erase(it);
			break ;
		}
	}
	delete user;}

void Server::handleMsg(Users *user)
{
	setBytesReceived(recv(user->getSocket(), this->buffer, sizeof(this->buffer), 0));
	if (getBytesReceived() <= 0) {
		if (getBytesReceived() == 0)
			std::cout << "Connection closed." << std::endl;
		else
			std::cerr << "Error: recv: " << strerror(errno) << std::endl;
		removeUserFromServer(user);
	}
	else {
		std::string msg(buffer, getBytesReceived());
		std::memset(this->buffer, '\0', sizeof(this->buffer));
		user->setCmd(msg);
		if (!Check(user->getCmd(), "\r\n"))
			return ;
		std::vector<std::string> vec = MySplit(user->getCmd(), "\r\n");
		std::string tmp = user->getCmd().substr(user->getCmd().rfind("\r\n") + 2);
		user->clearCmd();
		user->setCmd(tmp);
		for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it) {
			Command cmd = parse(*it);
			std::cout <<"Recv"<< *it << std::endl;
			if (!CheckCmd(cmd, user ,this->getHost()))
				executeCmd(cmd, user);
		}
	}
}

void Server::executeCmd(Command msg, Users *user)
{
	if (user->getStatus() != 4 && (msg.CmdName != "PING" && msg.CmdName != "CAP" && msg.CmdName != "NICK" && msg.CmdName != "USER" && msg.CmdName != "PASS"))
		return user->setBuffer(ERR_NOTREGISTERED(getHost()));
	if (msg.CmdName == "CAP")
		CapInit(msg, user);
	else if (msg.CmdName == "PASS")
			AddPtoUser(msg, user);
	else if (msg.CmdName == "NICK")
			AddNicktoUser(msg, user);
	else if (msg.CmdName == "USER")
			RegisterUser(msg, user);
	else if (msg.CmdName == "PING")
			SendPong(msg, user);
	else if (msg.CmdName == "BOT")
		Knight->executeBot(msg, user, this->AllUsers, this->AllChannels);
	else if (msg.CmdName == "JOIN")
		join(msg, user);
	else if (msg.CmdName == "PART")
		c_part(msg, user);
	else if (msg.CmdName == "KICK")
		c_kick(msg, user);
  	else if (msg.CmdName == "INVITE")
		c_invite(msg, user);
	else if (msg.CmdName == "TOPIC")
		c_topic(msg, user);
	else if (msg.CmdName == "MODE")
		c_mode(msg, user);
	else if (msg.CmdName == "PRIVMSG")
		c_privmsg(msg, user);
	else if(msg.CmdName == "WHOIS")
		Whois(user, msg);
}
