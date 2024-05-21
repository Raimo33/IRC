/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:23:51 by craimond          #+#    #+#             */
/*   Updated: 2024/05/21 19:41:37 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Server.hpp"
#include "irc/SystemCalls.hpp"
#include "irc/Client.hpp"
#include "irc/Channel.hpp"
#include "irc/SystemCalls.hpp"
#include "irc/EventHandler.hpp"
#include "irc/Hasher.hpp"
#include "irc/ReplyCodes.hpp"
#include "irc/Standards.hpp"
#include "irc/Exceptions.hpp"

using namespace std;
using namespace irc;

Server::Server(const uint16_t port_no, const string &password) :
	_port(port_no),
	_pwd_hash(Hasher(password).hexdigest()),
	_socket(socket_p(AF_INET, SOCK_STREAM, 0)),
	_event_handler(EventHandler())
{
	struct sockaddr_in server_addr;
	pollfd server_poll_fd;

	memset(&server_addr, 0, sizeof(server_addr));
	configureNonBlocking(_socket);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(_port);
	bind_p(_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
	listen_p(_socket, 5);
	cout << "Server waiting for connections on port: " << _port << endl;
	//TODO printare l'IP del server
	server_poll_fd.fd = _socket;
	server_poll_fd.events = POLLIN;
	_pollfds.push_back(server_poll_fd);
}

Server::Server(const Server &copy) :
	_port(copy._port),
	_pwd_hash(copy._pwd_hash),
	_clients(copy._clients),
	_channels(copy._channels),
	_pollfds(copy._pollfds),
	_socket(copy._socket),
	_event_handler(copy._event_handler) {}

Server::~Server(void) {}

void	Server::run(void)
{
	while (true)
	{
		poll_p(&_pollfds[0], _pollfds.size(), -1);
		for (size_t i = 0; i < _pollfds.size(); i++)
		{
			if (_pollfds[i].revents & POLLIN)
			{
				//new connection
				if (_pollfds[i].fd == _socket)
				{
					Client				*client;
					struct sockaddr_in	client_addr;
					socklen_t			client_addr_len;
					int 				client_socket;

					// TCP connection
					client_addr_len = sizeof(client_addr);
					client_socket = accept_p(_socket, (struct sockaddr *)&client_addr, &client_addr_len);
					configureNonBlocking(client_socket);

					const string client_ip_addr = string(inet_ntoa(client_addr.sin_addr));
					uint16_t client_port = ntohs(client_addr.sin_port);

					client = new Client(this, client_socket, client_ip_addr, client_port);
					addClient(client);
					client->setIsConnected(true);

					pollfd client_poll_fd;

					client_poll_fd.fd = client_socket;
					client_poll_fd.events = POLLIN;
					addPollfd(client_poll_fd);
					cout << "Connection established with " << client->getIpAddr() << endl;
				}
				else
				{
					//client already exists
					Client *client = NULL;

					for (map<string, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
					{
						if (it->second->getSocket() == _pollfds[i].fd)
						{
							client = it->second;
							break;
						}
					}
					handleClient(client, &i);
				}
			}
		}
	}
}

uint16_t	Server::getPort(void) const
{
	return _port;
}

const string	&Server::getPwdHash(void) const
{
	return _pwd_hash;
}

const map<string, Client *>	&Server::getClients(void) const
{
	return _clients;
}

void	Server::setClients(const map<string, Client *> &clients)
{
	_clients = clients;
}

const Client &Server::getClient(const string &nickname) const
{
	if (_clients.find(nickname) == _clients.end())
		throw ClientNotFoundException();
	return *(_clients.at(nickname));
}

void Server::addClient(Client *client)
{
	if (_clients.find(client->getUsername()) != _clients.end())
		throw ClientAlreadyExistsException();
	_clients[client->getUsername()] = client;
}

void Server::removeClient(const Client &client)
{
	if (!_clients.at(client.getUsername()))
		throw ClientNotFoundException();
	_clients.erase(client.getUsername());
}

const map<string, Channel *>	&Server::getChannels(void) const
{
	return _channels;
}

void	Server::setChannels(const map<string, Channel *> &channels)
{
	_channels = channels;
}

const Channel	&Server::getChannel(const string &name) const
{
	if (_channels.find(name) == _channels.end())
		throw ChannelNotFoundException();
	return *(_channels.at(name));
}

void	Server::addChannel(Channel *channel)
{
	if (_channels.find(channel->getName()) != _channels.end())
		throw ChannelAlreadyExistsException();
	_channels[channel->getName()] = channel;
}

void	Server::removeChannel(const Channel &channel)
{
	_channels.erase(channel.getName());
}

const vector<pollfd>	&Server::getPollfds(void)
{
	return _pollfds;
}

void	Server::setPollfds(const vector<pollfd> &pollfds)
{
	_pollfds = pollfds;
}

void	Server::addPollfd(const pollfd pollfd)
{
	_pollfds.push_back(pollfd);
}

void	Server::removePollfd(const pollfd pollfd)
{
	for (size_t i = 0; i < _pollfds.size(); i++)
	{
		if (_pollfds[i].fd == pollfd.fd)
		{
			_pollfds.erase(_pollfds.begin() + i);
			break;
		}
	}
}

void Server::removePollfd(const int socket)
{
	for (size_t i = 0; i < _pollfds.size(); i++)
	{
		if (_pollfds[i].fd == socket)
		{
			_pollfds.erase(_pollfds.begin() + i);
			break;
		}
	}
}

int	Server::getSocket(void) const
{
	return _socket;
}

void Server::handleClient(Client *client, size_t *i)
{
	char buffer[BUFFER_SIZE];

	if (!client)
		throw ClientNotFoundException();

	try
	{
		int bytes_read = recv(client->getSocket(), buffer, sizeof(buffer), 0);
		if (bytes_read > 0)
		{
			buffer[bytes_read - 1] = '\0';
			_event_handler.setClient(client);
			_event_handler.processInput(buffer); // se tutto va bene esegue il comando
			_event_handler.setClient(NULL);
		}
		else if (bytes_read <= 0)
		{
			// Connection closed
			removeClient(*client);
			(*i)--;
			// Error
			if (bytes_read < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
				throw SystemErrorException(strerror(errno));
		}
	}
	catch (const FatalErrorException &e)
	{
		throw;
	}
	catch (const IRCException &e)
	{
		//TODO send error message to client
		cerr << e.what() << endl;
	}
	catch (const exception &e)
	{
		//TODO send error message to client
		cerr << e.what() << endl;
	}
}

void Server::disconnectClient(Client *client)
{
	int socket = client->getSocket();

	shutdown_p(socket, SHUT_RDWR);
	close_p(socket);
	removePollfd(socket);
	removeClient(*client);
	delete client;
}

void Server::configureNonBlocking(const int socket) const
{
	int flags;

	flags = fcntl(socket, F_GETFL);
	if (flags == -1)
		throw runtime_error(strerror(errno));
	fcntl(socket, F_SETFL, flags | O_NONBLOCK);
	if (flags == -1)
		throw runtime_error(strerror(errno));
}

const map<uint16_t, string> Server::initReplyCodes(void)
{
	map<uint16_t, string>	m;
	//https://github.com/williamkapke/irc-replies/blob/master/replies.json

	m[1]	= "Welcome";
	m[2]	= "Your host";
	m[3]	= "Created";
	m[4]	= "My info";
	m[5]	= "Support";
	m[10]	= "Bounce";
	m[15]	= "Map";
	m[17]	= "Map end";
	m[18]	= "Map start";
	m[20]	= "Hello";
	m[42]	= "Your ID";
	m[43]	= "Save nick";
	m[200]	= "Trace link";
	m[201]	= "Trace connecting";
	m[202]	= "Trace handshake";
	m[203]	= "Trace unknown";
	m[204]	= "Trace operator";
	m[205]	= "Trace user";
	m[206]	= "Trace server";
	m[207]	= "Trace service";
	m[208]	= "Trace new type";
	m[209]	= "Trace class";
	m[211]	= "Stats link info";
	m[212]	= "Stats commands";
	m[213]	= "Stats C line";
	m[214]	= "Stats N line";
	m[215]	= "Stats I line";
	m[216]	= "Stats K line";
	m[217]	= "Stats Q line";
	m[218]	= "Stats Y line";
	m[219]	= "End of stats";
	m[221]	= "User mode is";
	m[231]	= "Service info";
	m[232]	= "End of services";
	m[233]	= "Service";
	m[234]	= "Service list";
	m[235]	= "Service list end";
	m[239]	= "Stats I auth";
	m[240]	= "Stats V line";
	m[241]	= "Stats L line";
	m[242]	= "Stats uptime";
	m[243]	= "Stats O line";
	m[244]	= "Stats H line";
	m[245]	= "Stats S line";
	m[246]	= "Stats ping";
	m[247]	= "Stats B line";
	m[248]	= "Stats define";
	m[249]	= "Stats debug";
	m[250]	= "Stats D line";
	m[251]	= "Luser client";
	m[252]	= "Luser operator";
	m[253]	= "Luser unknown";
	m[254]	= "Luser channels";
	m[255]	= "Luser me";
	m[256]	= "Admin me";
	m[257]	= "Admin loc1";
	m[258]	= "Admin loc2";
	m[259]	= "Admin email";
	m[261]	= "Trace log";
	m[262]	= "Trace end";
	m[263]	= "Try again";
	m[265]	= "Local users";
	m[266]	= "Global users";
	m[300]	= "None";
	m[301]	= "Away";
	m[302]	= "User host";
	m[303]	= "Is on";
	m[304]	= "Text";
	m[305]	= "Unaway";
	m[306]	= "Now away";
	m[311]	= "Whois user"; // Dynamic message: <nick> <user> <host> * :<real name>
	m[312]	= "Whois server"; // Dynamic message: <nick> <server> :<server info>
	m[313]	= "Whois operator"; // Dynamic message: <nick> :is an IRC operator
	m[314]	= "Who was user"; // Dynamic message: <nick> <user> <host> * :<real name>
	m[315]	= "End of who"; // Dynamic message: <name> :End of WHO list
	m[316]	= "Whois chanop"; // Dynamic message: <nick> :is a channel operator
	m[317]	= "Whois idle"; // Dynamic message: <nick> <integer> :seconds idle
	m[318]	= "End of whois"; // Dynamic message: <nick> :End of WHOIS list
	m[319]	= "Whois channels"; // Dynamic message: <nick> :<channels>
	m[321]	= "List start"; // Dynamic message: Channel :Users  Name
	m[322]	= "List"; // Dynamic message: <channel> <# visible> :<topic>
	m[323]	= "List end"; // Dynamic message: :End of LIST
	m[324]	= "Channel mode is"; // Dynamic message: <channel> <modes>
	m[325]	= "Unique op is"; // Dynamic message: <channel> <nickname>
	m[331]	= "No topic"; // Dynamic message: <channel> :No topic is set
	m[332]	= "Topic"; // Dynamic message: <channel> :<topic>
	m[333]	= "Topic who time"; // Dynamic message: <channel> <nickname> <set time>
	m[341]	= "Inviting"; // Dynamic message: <channel> <nick>
	m[342]	= "Summoning"; // Dynamic message: <user> :Summoning user to IRC
	m[344]	= "Reop list"; // Dynamic message: <channel> <nickname>
	m[345]	= "End of reop list"; // Dynamic message: <channel> :End of reop list
	m[346]	= "Invite list"; // Dynamic message: <channel> <invitemask>
	m[347]	= "End of invite list"; // Dynamic message: <channel> :End of invite list
	m[348]	= "Except list"; // Dynamic message: <channel> <exceptionmask>
	m[349]	= "End of except list"; // Dynamic message: <channel> :End of exception list
	m[351]	= "Version"; // Dynamic message: <version>.<debuglevel> <server> :<comments>
	m[352]	= "Who reply"; // Dynamic message: <channel> <user> <host> <server> <nick> <H|G>[*][@|+] :<hopcount> <real name>
	m[353]	= "Name reply"; // Dynamic message: <channel> :<nick> <nick> <nick>...
	m[361]	= "Kill done"; // Dynamic message: <nickname> :<comment>
	m[362]	= "Closing"; // Dynamic message: <server> :<comment>
	m[363]	= "Close end"; // Dynamic message: <server> :<comment>
	m[364]	= "Links"; // Dynamic message: <mask> <server> :<hopcount> <server info>
	m[365]	= "End of links"; // Dynamic message: <mask> :End of LINKS list
	m[366]	= "End of names"; // Dynamic message: <channel> :End of NAMES list
	m[367]	= "Ban list"; // Dynamic message: <channel> <banmask>
	m[368]	= "End of ban list"; // Dynamic message: <channel> :End of channel ban list
	m[369]	= "End of who was"; // Dynamic message: <nick> :End of WHOWAS
	m[371]	= "Info"; // Dynamic message: :<string>
	m[372]	= "Motd"; // Dynamic message: :- <text>
	m[373]	= "Info start"; // Dynamic message: :<server> info
	m[374]	= "End of info"; // Dynamic message: :End of INFO list
	m[375]	= "Motd start"; // Dynamic message: :- <server> Message of the day -
	m[376]	= "End of motd"; // Dynamic message: :End of MOTD command
	m[381]	= "You are oper"; // Dynamic message: :You are now an IRC operator
	m[382]	= "Rehashing"; // Dynamic message: :Rehashing
	m[383]	= "You are service"; // Dynamic message: :You are service
	m[384]	= "My port is"; // Dynamic message: <port> :<nick>
	m[385]	= "Not oper anymore"; // Dynamic message: :You are no longer an IRC operator
	m[391]	= "Time"; // Dynamic message: <server> :<time string>
	m[392]	= "Users start"; // Dynamic message: :UserID   Terminal  Host
	m[393]	= "Users"; // Dynamic message: :<username>
	m[394]	= "End of users"; // Dynamic message: :End of users
	m[395]	= "No users"; // Dynamic message: :Nobody logged in
	m[401]	= "No such nick"; // Dynamic message: <nickname> :No such nick/channel
	m[402]	= "No such server"; // Dynamic message: <server name> :No such server
	m[403]	= "No such channel"; // Dynamic message: <channel name> :No such channel
	m[404]	= "Cannot send to chan"; // Dynamic message: <channel name> :Cannot send to channel
	m[405]	= "Too many channels"; // Dynamic message: <channel name> :You have joined too many channels
	m[406]	= "Was no such nick"; // Dynamic message: <nickname> :There was no such nickname
	m[407]	= "Too many targets"; // Dynamic message: <target> :Duplicate recipients. No message delivered
	m[408]	= "No such service"; // Dynamic message: <service name> :No such service
	m[409]	= "No origin"; // Dynamic message: :No origin specified
	m[411]	= "No recipient"; // Dynamic message: :No recipient given (<command>)
	m[412]	= "No text to send"; // Dynamic message: :No text to send
	m[413]	= "No top level"; // Dynamic message: <mask> :No toplevel domain specified
	m[414]	= "Wild top level"; // Dynamic message: <mask> :Wildcard in toplevel domain
	m[415]	= "Bad mask"; // Dynamic message: <mask> :Bad Server/host mask
	m[416]	= "Too many matches"; // Dynamic message: <mask> :Too many matches
	m[421]	= "Unknown command"; // Dynamic message: <command> :Unknown command
	m[422]	= "No motd"; // Dynamic message: :MOTD File is missing
	m[423]	= "No admin info"; // Dynamic message: <server> :No administrative info available
	m[424]	= "File error"; // Dynamic message: :File error doing <file op> on <file>
	m[431]	= "No nickname given"; // Dynamic message: :No nickname given
	m[432]	= "Erroneous nickname"; // Dynamic message: <nick> :Erroneous nickname
	m[433]	= "Nickname in use"; // Dynamic message: <nick> :Nickname is already in use
	m[434]	= "Service name in use"; // Dynamic message: <nick> :Service name already in use
	m[435]	= "Service confused"; // Dynamic message: <nick> :Service is confused
	m[436]	= "Nick collision"; // Dynamic message: <nick> :Nickname collision KILL
	m[437]	= "Unavailable resource"; // Dynamic message: <nick/channel> :Nick/channel is temporarily unavailable
	m[441]	= "User not in channel"; // Dynamic message: <nick> <channel> :They aren't on that channel
	m[442]	= "Not on channel"; // Dynamic message: <channel> :You're not on that channel
	m[443]	= "User on channel"; // Dynamic message: <nick> <channel> :is already on channel
	m[444]	= "No login"; // Dynamic message: <user> :User not logged in
	m[445]	= "Summon disabled"; // Dynamic message: :SUMMON has been disabled
	m[446]	= "Users disabled"; // Dynamic message: :USERS has been disabled
	m[451]	= "Not registered"; // Dynamic message: :You have not registered
	m[461]	= "Need more params"; // Dynamic message: <command> :Not enough parameters
	m[462]	= "Already registered"; // Dynamic message: :Unauthorized command (already registered)
	m[463]	= "No perm for host"; // Dynamic message: :Your host isn't among the privileged
	m[464]	= "Password mismatch"; // Dynamic message: :Password incorrect
	m[465]	= "You're banned creep"; // Dynamic message: :You are banned from this server
	m[466]	= "You will be banned"; // Dynamic message: :You will be banned from this server
	m[467]	= "Key set"; // Dynamic message: <channel> :Channel key already set
	m[471]	= "Channel is full"; // Dynamic message: <channel> :Cannot join channel (+l)
	m[472]	= "Unknown mode"; // Dynamic message: <char> :is unknown mode char to me
	m[473]	= "Invite only chan"; // Dynamic message: <channel> :Cannot join channel (+i)
	m[474]	= "Banned from chan"; // Dynamic message: <channel> :Cannot join channel (+b)
	m[475]	= "Bad channel key"; // Dynamic message: <channel> :Cannot join channel (+k)
	m[476]	= "Bad chan mask"; // Dynamic message: <channel> :Bad Channel Mask
	m[477]	= "No chan modes"; // Dynamic message: <channel> :Channel doesn't support modes
	m[478]	= "Ban list full"; // Dynamic message: <channel> <char> :Channel list is full
	m[481]	= "No privileges"; // Dynamic message: :Permission Denied- You're not an IRC operator
	m[482]	= "Chanop privileges needed"; // Dynamic message: <channel> :You're not channel operator
	m[483]	= "Can't kill server"; // Dynamic message: :You can't kill a server!
	m[484]	= "Restricted"; // Dynamic message: :Your connection is restricted!
	m[485]	= "Unique op privileges needed"; // Dynamic message: :You're not the original channel operator
	m[491]	= "No oper host"; // Dynamic message: :No O-lines for your host
	m[492]	= "No service host"; // Dynamic message: :No S-lines for your host
	m[499]	= "Stats K line"; // Dynamic message: <host> :<reason>
	m[501]	= "Umode unknown flag"; // Dynamic message: :Unknown MODE flag
	m[502]	= "Users don't match"; // Dynamic message: :Cannot change mode for other users
	m[708]	= "Etrace full"; // Dynamic message: <user> <server> :<info>
	m[759]	= "Etrace end"; // Dynamic message: <user> <server> :End of ETRACE

    return m;
}

const map<uint16_t, string>	Server::reply_codes = initReplyCodes();