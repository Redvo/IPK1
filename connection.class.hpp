/****************************************************************************************
 * Project 1 in course Computer Communications and Networks, 2014/2015
 * Author: Lukáš Rendvanský
 * Email: xrendv00@stud.fit.vutbr.cz
 * Date: 18.3.2015
 * File: connection.class.hpp
 * Description: Implementation of connection class - header file.
 ***************************************************************************************/

#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include "exception.class.hpp"

class Connection {

public:
	Connection(long port) {
		this->port = port;
	}

	virtual ~Connection() { }

	long GetPort();

	int GetSocket();

protected:

	long port;
	int soc;
	socklen_t *sinlen;
	struct sockaddr_in socketAddressIn;

};

class ServerConnection: public Connection {

public:
	ServerConnection(long port): Connection(port) {	}

	virtual ~ServerConnection() { }

	int GetSocketId();

	void CreateSocket();

	void Bind();

	void Listen();

	void Accept();

	std::string ReadRequest();

	void SendResponse(std::string value);

	void Close();

	void CloseServer();

private:

	int socketId;

};

class ClientConnection: public Connection {

public:
	ClientConnection(long port): Connection(port) {	}

	virtual ~ClientConnection() { }

	void SetHostName(std::string value);

	std::string GetHostName();

	void CreateSocket();

	void Connect();

	void SendRequest(std::string value);

	std::string ReadResponse();

	void Close();

private:

	struct hostent *hptr;

	std::string hostname;

};


#endif /* CONNECTION_H */