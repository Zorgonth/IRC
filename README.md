# IRC

ft_irc is an IRC (Internet Relay Chat) server implementation written in C++98. It provides a platform for real-time text-based communication over TCP/IP between multiple clients. The server supports essential IRC functionalities such as user authentication, channel management, private messaging, and operator privileges.

## DESCRIPTION

IRC (Internet Relay Chat) - an application-level Protocol for exchanging messages in real-time. Designed primarily for group communication, it also allows you to communicate via private messages and exchange data, including files. IRC uses the TCP transport protocol and cryptographic TLS (optional). IRC began to gain particular popularity after Operation "Desert Storm" (1991), when messages from all over the world were collected in one place and broadcast on-line to IRC. Due to the technical simplicity of implementation, the IRC protocol was used in the organization of botnets as a means of transmitting control commands to the computers participating in the botnet from the owner.

## USAGE

This IRC was made to be compatible with the KVIrc Client, So you would need to install it on your machine to use the full capacities of the code!
After making the project and compiling all the provided files, You will have to excute the program using this command:
```
./ircserv <password> <port>
```
Now you that you started the program, you can start up Kvirc and connect to the server using the ip adress of the host with the port.
Afer successfully connecting to the program You will have to go through the registration steps before using any commands, and you shall do so using these following steps in order:
```
PASS <password>
```
make sure it's the same password that you set up while launching the program else it won't let you continue the registration.
```
NICK <nickname>
```
Set up your preferred nickname (The nicknames are unique so you can't have the same nickname as someone else on the server!).
```
USER <username> 0 * <realname>
```
Set up your username and realname(these names aren't unique since multiple people can have the same realname).
The last two commands in the registartion proccess are interchangable so you can do one before the other.

### COMMANDS

After registring successefully Now you can use Any of these commands:
- PRIVMSG
```
PRIVMSG <nickname> <message>
```
PRIVMSG is basically a private message command, you just have enter the nickname of the recepient and if that user exists he shall receive your message, It can also be used to send a message to an existing channel.
This Messge can be sent to multiple user/channels at the same time by using this syntax:
```
PRIVMSG <nickname>,#<channel>.. <message>
```
- JOIN
```
JOIN #<channel> <key>
```
JOIN is a command that can either create channels or join alredy existing ones, since channel names are unique you can't create a channel with a name that already exists so instead you will be trying to join the channel that way.
These channels can have a key, So you can pass it as a second paramater to the command, although it is not necessary to do so if you wanna create a channel wihtout a key.
This command can also be used to create/join multiple channels at the same time(Usage is the same as PRIVMSG).
- INVITE
```
INVITE #<nickname> <channel>
```
INVITE is a command that lets you invite another user to an already existing channel.
- PART
```
PART #<channel> <reason>
```
PART is a command that lets you leave a channel, You can also provide a reason for leaving and it will be brodcasted to everyone else in that channel.
- TOPIC
```
TOPIC #<channel>
```
TOPIC let's you know the topic of a certain channel, yet it also let's you change the topic for a channel if you have the right privileges:
```
TOPIC #<channel> <new topic>
```
- KICK
```
KICK #<channel> <nickname>,<nickname>... <reason>
```
KICK let's you kick a user from a certain channel only if you have certain privileges on that channel. You can kick multiple users at the same time.
Just like PART you can either provide a reasno for kicking the users which everyone will see in that channel, or a default one will be provided.
- WHOIS
```
WHOIS <nickname>
```
This command let's you know all the information that can be provided about a certain user of your choosing.
- MDOE
```
MODE #<channel> <mode> <extra paramater if needed>
```
MODE let's you change some special modes of a certain channel, but can only be accessed if you are an Operator in that channel.
Each mode has to start with a '+' or a '-', it's treated as a switch for that mode. 
Here's a list of each mode and it's exlpanation:
  - i: This mode let's you either make the channel invite only/or accessible to anyone without needing an invite.
  - k: This mode let's you either set up/change the key for the channel or let's you delete it.
  - t: This mode let's you make the topic change command accessible by non Operators in that channel, Or it can make it so only Operators who can make a change to it.
  - l: This mode let's you either set a limit to the number of users in a channel (you have to feed it an extra paramater which should be numerical), Or let's you remove that limit.
  - o: This mode let's you either make another user an Operator in that channel (you have to feed it an extra paramater which should be a nickname of a suer), Or turn an Operator in that channel to a normal user again.
- BOT
```
BOT <cmd>
```
BOT is a special command that let's you use our beloved bot [KNIGHT] which allows you to use certain commands that only he can preform.
Here's a list fo all those commands:
  - HELPC: This command lists all the commands available on the server for you.
  - BOTC: This command lists all the bot commands.
  - UL: Gives you a list of every user on the server.
  - CL: Gives you a list of every channel on the server.
  - d20: This command throws a dice of 20, Test you luck.
  - Eightball: you can ask the bot any question you have and he shall reply with his wisdom (provide the question in the same command).
  - WHOISOP: This command let's you know who are the Operators in a certain channel (you have to feed it the name of channel), But only if you are on that channel.
