/***************************************************************************************
 * Project 1 in course Computer Communications and Networks, 2014/2015
 * Author: Lukáš Rendvanský
 * Email: xrendv00@stud.fit.vutbr.cz
 * Date: 18.3.2015
 * File: connection.class.cpp
 * Description: Implementation of connection class - source code.
 ***************************************************************************************/

#include "connection.class.hpp"

long Connection::GetPort() {
	return this->port;
}

int Connection::GetSocket() {
	return this->soc;
}

void ServerConnection::CreateSocket() {

	if ((this->soc = socket(PF_INET, SOCK_STREAM,0)) < 0) { // create socket
		throw ConnectionException("Socket creation failed.",Exception::INVALID_SOCKET);
	}

	this->socketAddressIn.sin_family = PF_INET; // set protocol family to Internet
	this->socketAddressIn.sin_port = htons(this->port); // set port no.
	this->socketAddressIn.sin_addr.s_addr  = INADDR_ANY; // set IP addr to any interface

}

void ServerConnection::Bind() {

	// bind socket
	if (bind(this->soc, (struct sockaddr*) &this->socketAddressIn, sizeof(this->socketAddressIn)) < 0 ) {
		throw ConnectionException("Binding of socket failed.",Exception::BIND_FAILED);
	}

	this->sinlen = (socklen_t*) malloc(sizeof(socklen_t*));
	if(this->sinlen != NULL) {
		*this->sinlen = sizeof(this->socketAddressIn);
	} else {
		throw ConnectionException("Binding of socket failed.",Exception::BIND_FAILED);
	}
}

int ServerConnection::GetSocketId() {
	return this->socketId;
}

void ServerConnection::Listen() {
	if (listen(this->soc, 128)) { 
		throw ConnectionException("Listening on port failed.",Exception::LISTEN_FAILED);
	}
}

void ServerConnection::Accept() {
	if ((this->socketId = accept(this->soc, (struct sockaddr *) &this->socketAddressIn, this->sinlen)) < 0 ) {
		throw ConnectionException("Accepting the request failed.",Exception::ACCEPT_FAILED);
	}
}

std::string ServerConnection::ReadRequest() {

	char *buffer;
	std::string request;

	int32_t length = 0;

	alarm(20); // Set timeout to 20 seconds

	// Get request length
	if ( read(this->socketId, &length, sizeof(length)) < 0 ) {  /* send message to server */
		throw ConnectionException("Sending request failed.",Exception::SEND_REQUEST_FAILED);
	}

	length = ntohl(length);

	buffer = (char*) calloc((length + 1) * sizeof(char), sizeof(char));

	// Get request message
	if ((buffer == NULL) || (read(this->socketId, buffer, length) <0)) {  /* read message from client */
		throw ConnectionException("Unable to read client request.",Exception::READ_REQUEST_FAILED);
	}

	alarm(0); // Clear timeout and continue in work

	request = std::string(buffer);
	free(buffer);

	return request;
}

void ServerConnection::SendResponse(std::string value) {

	int32_t length = htonl(value.length());

	if (write(this->socketId, &length, sizeof(length)) < 0 ) {  /* send message to server */
		throw ConnectionException("Sending request failed.",Exception::SEND_REQUEST_FAILED);
	}

	if (write(this->socketId, value.c_str(), value.length()) < 0 ) {  /* echo message back */
		throw ConnectionException("Unable to send response.",Exception::SEND_RESPONSE_FAILED);
	}

}

void ServerConnection::Close() {
	if (close(this->socketId) < 0) { 
		throw ConnectionException("Unable to close connection.",Exception::CLOSE_CONNECTION);
	} 
}

void ServerConnection::CloseServer() {
	if (close(this->soc) < 0) { 
		throw ConnectionException("Unable to close connection.",Exception::CLOSE_SERVER);
	}
}

void ClientConnection::SetHostName(std::string value) {
	this->hostname = value;
}

void ClientConnection::CreateSocket() {
	if ( (this->soc = socket(PF_INET, SOCK_STREAM, 0 ) ) < 0) { // create socket
		throw ConnectionException("Socket creation failed.",Exception::INVALID_SOCKET);
	}

	if ( (hptr =  gethostbyname(this->hostname.c_str()) ) == NULL){
		throw ConnectionException("Socket creation failed.",Exception::INVALID_SOCKET);
	}

	this->socketAddressIn.sin_family = PF_INET; //set protocol family to Internet
	this->socketAddressIn.sin_port = htons(this->port); // set port no.
	memcpy(&this->socketAddressIn.sin_addr,this->hptr->h_addr,this->hptr->h_length);
}

void ClientConnection::Connect() {
	if (connect (this->soc, (struct sockaddr *)&this->socketAddressIn, sizeof(this->socketAddressIn) ) < 0 ){
		throw ConnectionException("Connection to server failed.",Exception::CONNECT_FAILED);
	}
}

void ClientConnection::SendRequest(std::string value) {

	int32_t length = htonl(value.length());

	if ( write(this->soc, &length, sizeof(length)) < 0 ) { // send message to server
		throw ConnectionException("Sending request failed.",Exception::SEND_REQUEST_FAILED);
	}

	if ( write(this->soc, value.c_str(), value.length()) < 0 ) { // send message to server
		throw ConnectionException("Sending request failed.",Exception::SEND_REQUEST_FAILED);
	}
}

std::string ClientConnection::ReadResponse() {
	
	char *buffer;
	std::string response;

	int32_t length = 0;

	alarm(20); // Set timeout to 20 seconds

	// Get request length
	if ( read(this->soc, &length, sizeof(length)) < 0 ) { // send message to server
		throw ConnectionException("Sending request failed.",Exception::SEND_REQUEST_FAILED);
	}

	length = ntohl(length);

	buffer = (char*) calloc((length + 1) * sizeof(char), sizeof(char));

	// Get request message
	if ((buffer == NULL) || (read(this->soc, buffer, length) < 0)) { // read message from server
		throw ConnectionException("Reading response failed.",Exception::READ_RESPONSE_FAILED);
	}

	alarm(0); // Clear timeout and continue in work

	response = std::string(buffer);
	free(buffer);

	return response;
}

void ClientConnection::Close() {
	if (close(this->soc) < 0) { 
		throw ConnectionException("Unable to close connection.",Exception::CLOSE_CLIENT);
	}
}
