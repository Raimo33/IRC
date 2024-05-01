/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 17:00:28 by craimond          #+#    #+#             */
/*   Updated: 2024/05/01 22:58:02 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "echo_server.hpp"

void	check_args(int argc, char **argv);
int		setup_server(char **argv);
int		connect_with_client(int server_socket);
void	listen_and_echo(int client_socket);
void	clean_exit(int server_socket, int client_socket);

int main(int argc, char **argv)
{
	int server_socket;
	int client_socket;

	check_args(argc, argv);
	server_socket = setup_server(argv);
	client_socket = connect_with_client(server_socket);	
	listen_and_echo(client_socket);
	clean_exit(server_socket, client_socket);
}

void	check_args(int argc, char **argv)
{
	if (argc != 2)
	{
		cerr << "Usage: " << argv[0] << " <port>" << endl;
		exit(EXIT_FAILURE);
	}
}

int	setup_server(char **argv)
{
	int					port;
	int 				server_socket;
	struct sockaddr_in	server_addr;

	port = atoi(argv[1]);
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port); //cambia la endianess della porta per essere uguale a quella del server
	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_socket, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	return (server_socket);
}

int	connect_with_client(int server_socket)
{
	struct sockaddr_in	client_addr;
	socklen_t			client_addr_len;
	string				client_ip;
	int					client_port;
	int					client_socket;

	client_addr_len = sizeof(client_addr);
	cout << "Waiting for connection..." << endl;
	client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
	client_ip = inet_ntoa(client_addr.sin_addr);
	client_port = ntohs(client_addr.sin_port); //cambia la endianess della porta per essere uguale a quella del client
	cout << "Connection established with " << client_ip << ":" << client_port << endl;
	return (client_socket);
}

void	listen_and_echo(int client_socket)
{
	char	buffer[BUFFER_SIZE];
	int		bytes_received;

	while (true)
	{
		memset(buffer, 0, BUFFER_SIZE);
		bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0); //read
		if (bytes_received <= 0)
			break;
		cout << "Received: '" << buffer << "', echoing back..." << endl;
		if (send(client_socket, buffer, bytes_received, 0) < 0) //write
			break;
	}
	perror("Exiting");
}

void	clean_exit(int server_socket, int client_socket)
{
	cout << "Closing connection..." << endl;
	shutdown(client_socket, SHUT_RDWR);
	close(client_socket);
	close(server_socket);
}