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

#include "Users.hpp"

class Users;

struct	C_Users{
	Users *user;
	int	flag;
};

class Channel
{
	private:
		int						limit;
		bool						Limitf;
		bool						Topicf;
		bool						Invitef;
		bool						Kickf;
		bool						Passf;
		std::string				Mode;
		std::string				Name;
		std::string				Password;
		std::string				Topic;
		std::string				Quote;
	public:
		Channel(std::string name);
		Channel(const Channel &cp);
		~Channel();
		Channel	&operator=(const Channel &og);

		void	setName(std::string name);
		void	setPassword(std::string password);
		void	setTopic(std::string topic);
		void	setLimit(int	limit);
		void	setLimitf(bool	limitf);
		void	setPasswordf(bool	passwordf);
		void	setInvitef(bool	invitef);
		void	setTopicf(bool	topicf);
		void	setQuote(std::string Quote);
		void	setKickf(bool	Kickf);
		
		std::vector<struct C_Users>	UserList;
		std::string	getName() const;
		std::string	getPassword() const;
		std::string	getTopic() const;
		std::string	getQuote() const;
		std::string	getMode() const;
		int	getLimit() const;
		bool	getPasswordf() const;
		bool	getKickf() const;
		bool	getLimitf() const;
		bool	getTopicf() const;
		bool	getInvitef() const;
		std::vector<Users *> getAllUsersInChan(std::vector<Users *> Allusers);
		int	getNumberofUsers();
		std::string getAllUsersInChanList(std::vector<Users *> AllUsers);

		void	ClearPass();
		void	ChangeMode();
		void	addUsertoC(Users *user);
		bool	UserIsInC(Users *user);
		void	deleteUserfromC(Users	*user);
		void	fliptoOperator(Users	*user);
		bool	CheckifOP(Users *user);
		void	brodcastMsg(std::string msg, std::vector<Users *> users);
		void	brodcastMsgPriv(std::string msg, std::vector<Users *> users, Users *user);
};