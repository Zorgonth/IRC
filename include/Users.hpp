#pragma once

#include "Lib.hpp"
#include <unistd.h>

class	Users
{
	private:
		std::string Nickname;
		std::string	Username;
		std::string Hostname;
		std::string Realname;
		std::string	Password;
		std::string Mode;
		int			Socket;
		int			Status;
		std::string buffer;
		std::string	cmd;
		//myb buffer
	public:
		Users(std::string hostname, int socket);
		~Users();
		Users(const Users &cp);
		Users &operator=(const Users &cp);

		std::string getNickname() const;
		std::string getUsername() const;
		std::string getHostname() const;
		std::string getRealname() const;
		std::string getPassword() const;
		std::string getMode() const;
		int			getSocket() const;
		int			getStatus() const;
		std::string		getBuffer() const;
		std::string		getCmd() const;
		std::string 	getSrc();
		
		void setNickname(std::string nickname);
		void setUsername(std::string username);
		void setHostname(std::string hostname);
		void setRealname(std::string realname);
		void setPassword(std::string pass);
		void setMode(std::string mode);
		void setSocket(int socket);
		void setStatus(int status);
		void setBuffer(std::string msg);
		void setCmd(std::string msg);

		void	clearBuffer();
		void	clearCmd();
};