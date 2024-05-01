/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 17:00:28 by craimond          #+#    #+#             */
/*   Updated: 2024/05/01 18:55:03 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "echo_server.hpp"


# define BUFFER_SIZE 1024

int main(int argc, char **argv)
{
	(void)argc;
	if (argc != 2)
	{
		cerr << "Usage: " << argv[0] << " <port>" << endl;
		exit(EXIT_FAILURE);
	}

	int port = atoi(argv[1]);

	//socket
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);

	//bind
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port); //cambia la endianess della porta per essere uguale a quella del server

	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	//listen
	if (listen(server_socket, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	//establish connection
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);

	cout << "Waiting for connection..." << endl;
	int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);

	std::string client_ip = inet_ntoa(client_addr.sin_addr);
	int client_port = ntohs(client_addr.sin_port); //cambia la endianess della porta per essere uguale a quella del client

	cout << "Connection established with " << client_ip << ":" << client_port << endl;

	//receive and echo back
	char buffer[BUFFER_SIZE];

	while (true)
	{
		memset(buffer, 0, BUFFER_SIZE);
		//receive
		int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0); //read
		
		if (bytes_received < 0)
		{
			perror("recv failed");
			break;
		}
		else if (bytes_received == 0)
		{
			cout << "Connection terminated by client: " << client_ip << ":" << client_port << endl; 
			break;	
		}
		cout << "Received: " << buffer;
		//echo back
		if (send(client_socket, buffer, bytes_received, 0) < 0) //write
		{
			perror("send failed");
			break;
		}
	}

	cout << "Closing connection..." << endl;
	shutdown(client_socket, SHUT_RDWR);
	close(server_socket);
}