#include "../include/Users.hpp"

Users::Users(std::string name, int socket)
{
	Hostname = name;
	Nickname = "*";
	Username = "*";
	Realname = "*";
	Mode = "*";
	Socket = socket;
	Status = 0;
}

Users::~Users()
{
	close(Socket);
}

std::string Users::getNickname() const
{
	return this->Nickname;	
}

std::string	Users::getCmd() const
{
	return this->cmd;
}

std::string Users::getBuffer() const
{
	return this->buffer;	
}

std::string	Users::getUsername() const{
	return (this->Username);
}

std::string	Users::getHostname() const{
	return (this->Hostname);
}

std::string	Users::getRealname() const{
	return (this->Realname);
}
std::string	Users::getPassword() const{
	return (this->Password);
}

std::string	Users::getMode() const{
	return (this->Mode);
}

int	Users::getSocket() const{
	int	sock = this->Socket;
		return (sock);
}

int	Users::getStatus() const{
	int	stat = this->Status;
	return (stat);
}

std::string Users::getSrc()
{
	std::string src = this->getNickname() + "!" + this->getUsername() + "@" + this->getHostname();
	return (src);
}

void	Users::setCmd(std::string msg)
{
	this->cmd += msg;
}

void	Users::setNickname(std::string nickname){
	this->Nickname = nickname;
}
void	Users::setUsername(std::string username){
	this->Username = username;
}
void	Users::setHostname(std::string hostname){
	this->Hostname = hostname;
}
void	Users::setRealname(std::string realname){
	this->Realname = realname;
}
void	Users::setPassword(std::string pass){
	this->Password = pass;
}
void	Users::setMode(std::string mode){
	this->Mode = mode;
}
void	Users::setSocket(int	socket){
	this->Socket = socket;
}
void	Users::setStatus(int status){
	this->Status = status;
}

void	Users::setBuffer(std::string msg)
{
	this->buffer += msg;
}

Users::Users(const Users &cp){
	if (this != &cp){
		setHostname(cp.getHostname());
		setNickname(cp.getNickname());
		setUsername(cp.getUsername());
		setRealname(cp.getRealname());
		setMode(cp.getMode());
		setSocket(cp.getSocket());
		setStatus(cp.getStatus());
	}
	std::cout << "copy constructor has been called" << std::endl;
}

Users &Users::operator=(const Users &cp){
	if (this != &cp)
	{
		setHostname(cp.getHostname());
		setNickname(cp.getNickname());
		setUsername(cp.getUsername());
		setRealname(cp.getRealname());
		setMode(cp.getMode());
		setSocket(cp.getSocket());
		setStatus(cp.getStatus());
	}
	std::cout << "copy assignment constructor has been called" << std::endl;
	return (*this);
}

void	Users::clearCmd()
{
	this->cmd.clear();
}

void	Users::clearBuffer()
{
	this->buffer.clear();
}