#pragma once

#include <sstream>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>

#include "Lib.hpp"
#include "Users.hpp"
#include "Channel.hpp"
#include "Bot.hpp"

class Users;
class Bot;
class Channel;
class Server
{
	private:
		int	Port;
		std::string Password;
		bool			ServerStatus;
		std::string	Host;
		char		buffer[1024];
		std::vector<struct pollfd>	Fds;
		ssize_t	BytesReceived;
		int		ServerSocket;
		std::vector<Users *>	AllUsers;
		std::vector<Channel *> AllChannels;
		Server();
		Bot	*Knight;
	public:
		Server(int port, std::string password);
		~Server();
		Server(const Server &cp);
		Server &operator=(const Server &cp);
	
		void init();
		void start();
		void stop();
		void	addPfds(struct pollfd sfd);
		void	addUser(Users *user);
		int	addNewClient();
		void	send_2usr(int fd);
		void	removePfds(struct pollfd sfd);
		void	removeUserFromServer(Users *user);
		void	handleMsg(Users *user);
		void	executeCmd(Command msg, Users *user);
		void	AddChanToServ(Channel *channel);
		void	Whois(Users *user, Command cmd);
	
	//Setters
		void	setHost(std::string name);
		void	setStatus(bool flag);
		void	setBytesReceived(int BytesReceived);
		void	setAllUsers(std::vector<Users *> user);
		void	setAllChannels(std::vector<Channel *> vec);

	//Getters
		std::string getHost() const;
		bool	getStatus() const;
		int	getPort() const;
		Users*	getUserByFd(int fd);
		ssize_t getBytesReceived();
		std::vector<Users *> getVectorU() const;
		std::vector<Channel *> getVectorCh() const;
		Users *getUserByNn(std::string nname);
		Channel *getChannel(const std::string cname);
	
	//Commands
		void	AddPtoUser(Command cmd, Users *user);
		void	AddNicktoUser(Command cmd, Users *user);
		void	CapInit(Command cmd, Users *user);
		void	RegisterUser(Command cmd, Users *user);
		void	SendPong(Command cmd, Users *user);
		void	c_topic(Command cmd, Users *user);
		void	c_privmsg(Command cmd, Users *user);
		void	join(Command, Users *user);
		void	c_kick(Command cmd, Users *user);
		void	c_invite(Command cmd, Users *user);
		void	c_part(Command cmd, Users* user);
		void	c_mode(Command cmd, Users *user);
};