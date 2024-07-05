#include "../include/Server.hpp"
#include "../include/Users.hpp"

void  Server::init()
{
   char  name[1024];

   if (gethostname(name, sizeof(name)) == -1){
      std::cerr << RED <<"name problem" << strerror(errno) << DEFAULT <<std::endl;
      exit (EXIT_FAILURE);
   }
   setHost(name);
   ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
   if (ServerSocket == -1) {
      std::cerr << "Error: socket: " << strerror(errno) << std::endl;
      exit(EXIT_FAILURE);
   }
   int opt = 1;
   if (setsockopt(this->ServerSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))
         == -1) {
      std::cerr << "Error: setsockopt: " << strerror(errno) << std::endl;
      close(this->ServerSocket);
      exit(EXIT_FAILURE);
   }
   if (fcntl(this->ServerSocket, F_SETFL, O_NONBLOCK) == -1) {
      std::cerr << "Error: fcntl: " << strerror(errno) << std::endl;
      close(this->ServerSocket);
      exit(EXIT_FAILURE);
   }
   struct sockaddr_in serverAdr;
   std::memset(&serverAdr, 0, sizeof(serverAdr));
   serverAdr.sin_family = AF_INET;
   serverAdr.sin_addr.s_addr = INADDR_ANY;
   serverAdr.sin_port = htons(getPort());
   if (bind(this->ServerSocket, (struct sockaddr *)&serverAdr, sizeof(serverAdr))
         == -1) {
      std::cerr << "Error: bind: " << strerror(errno) << std::endl;
      close(this->ServerSocket);
      exit(EXIT_FAILURE);        
   }
   
   if (listen(this->ServerSocket, 128) == -1) {
      std::cerr << "Error: listen: " << strerror(errno) << std::endl;
      close(this->ServerSocket);
      exit(EXIT_FAILURE);
   }
   
   struct pollfd sfd;
   std::memset(&sfd, 0, sizeof(sfd));
   sfd.fd = this->ServerSocket;
   sfd.events = POLLIN;
   addPfds(sfd);
}

void  Server::start()
{
   int   activity;
   Users *user;
   while (1)
   {
      activity = poll(&(this->Fds[0]), this->Fds.size(), -1);
      if (activity < 0) {
         std::cout << "Error: poll: " << strerror(errno) << std::endl;
         exit(EXIT_FAILURE);
      }

      for (size_t i = 0; i < this->Fds.size(); ++i) {
         if (this->Fds[i].revents & POLLIN) {
            if (this->Fds[i].fd == this->ServerSocket) {
               if (addNewClient() == -1)
                  continue ;
            }
            else {
               user = getUserByFd(this->Fds[i].fd);
               if (user)
                  handleMsg(user);
            }
         }
         if (i < this->Fds.size() && (this->Fds[i].revents & POLLOUT))
            send_2usr(this->Fds[i].fd); 
      }
   }
}

void	Server::send_2usr(int fd) 
{
	if (fd == this->ServerSocket || fd == -1)
		return ;
	Users *user = getUserByFd(fd);
	std::string msg = user->getBuffer();
	if (msg.empty())
		return ;
      if (send(fd, msg.c_str(), msg.size(), 0) != (long)msg.size())
		   std::cerr << "Error: send: did not send all data" << std::endl;
      else
	      std::cout << GREEN << "SENT: " << DEFAULT << msg << std::endl;
	user->clearBuffer();
}

int   Server::addNewClient()
{
	struct sockaddr_in clientAdr;
	socklen_t clientAddrSize = sizeof(clientAdr);
	int clientSocket = accept(this->ServerSocket, (struct sockaddr *)&clientAdr, &clientAddrSize);
	if (clientSocket == -1) {
		std::cerr << "Error: accept: " << strerror(errno) << std::endl;
		return -1;
	}
	else
		std::cout << BLUE << "NEW CONNECTION" << DEFAULT << std::endl;

	if (fcntl(this->ServerSocket, F_SETFL, O_NONBLOCK) == -1) {
		std::cerr << "Error: fcntl: " << strerror(errno) << std::endl;
		return -1;
	}

	char name[1000];
	if (getnameinfo((struct sockaddr*)&clientAdr, clientAddrSize, name, sizeof(name), 0, 0, NI_NUMERICHOST) != 0) {
		std::cerr << "Error: getnameinfo: " << strerror(errno) << std::endl;
		return -1;
	}
	
	pollfd new_client_fd;
	std::memset(&new_client_fd, 0, sizeof(new_client_fd));
   new_client_fd.fd = clientSocket;
   new_client_fd.events = POLLIN | POLLOUT;
   addPfds(new_client_fd);
	
	Users *user = new Users(std::string(name), clientSocket);
	if (!user) {
		std::cerr << "Error: failed to create user for the connected client." << std::endl;
		close(clientSocket);
		removePfds(new_client_fd);
		return -1;
	}
   addUser(user);
	return 0;
}
