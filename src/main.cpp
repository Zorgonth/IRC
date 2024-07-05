#include "../include/Lib.hpp"
#include "../include/Server.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << RED << "Please enter all needed Arguments!" << DEFAULT << std::endl;
		std::cerr << "How to use : ./IRC <port> <password>" << std::endl;
		return 1;
	}
	if (PortChecker(argv[1]) == 1)
		return 1;
	Server server(atoi(argv[1]), argv[2]);
	server.init();
	server.start();
	return 0;
}