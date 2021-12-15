//
// Created by ayserjamshidi on 10/28/21.
//

#ifndef PROJECT_3_NETWORKED_SPELL_CHECKER_AYSERJAMSHIDI_OPEN_LISTENER_SOCKET_CPP
#define PROJECT_3_NETWORKED_SPELL_CHECKER_AYSERJAMSHIDI_OPEN_LISTENER_SOCKET_CPP

#include <cstring> // bzero
#include <netinet/in.h> // sockaddr_in, networking definitions, etc...

int OpenListenerSocket(int port, int backlog_num) {
	int listenfd, optval = 1;
	sockaddr_in server{};

	// Create a socket descriptor
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;

	// Prevents "Address already in use" error from bind
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *) &optval, sizeof(int)) < 0)
		return -1;

	// Listenfd will be an endpoint for all requests to port on any IP address for this host
	bzero((char *) &server, sizeof(server));
	server.sin_family = AF_INET; // Internet Protocol
	server.sin_addr.s_addr = htonl(INADDR_ANY); // Address to accept any incoming messages
	server.sin_port = htons((unsigned short) port); // Port number...

	// Attaches the socket IP:PORT to server
	if (bind(listenfd, (struct sockaddr *) &server, sizeof(server)) != 0)
		return -1;

	// https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-listen
	// Make it a listening socket ready to accept connection requests
	if (listen(listenfd, backlog_num) < 0)
		return -1;

	return listenfd;
}
#endif //PROJECT_3_NETWORKED_SPELL_CHECKER_AYSERJAMSHIDI_OPEN_LISTENER_SOCKET_CPP
