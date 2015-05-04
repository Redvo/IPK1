/****************************************************************************************
 * Project 1 in course Computer Communications and Networks, 2014/2015
 * Author: Lukáš Rendvanský
 * Email: xrendv00@stud.fit.vutbr.cz
 * Date: 18.3.2015
 * File: server.cpp
 * Description: Implementation of server-side application - source code.
 ***************************************************************************************/

#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "arguments.class.hpp"
#include "connection.class.hpp"
#include "searcher.class.hpp"
#include "exception.class.hpp"

using namespace std;

ServerConnection *globalConnection = NULL;

const string helpMessage = "\nProject 1 in course Computer Communications and Networks, 2014/2015\n\
Author: Lukas Rendvansky (xrendv00@stud.fit.vutbr.cz)\n\n\
Description: Server side implementation. This program searchs for specific \
lines in /etc/passwd file by conditions specified in requests from client. \
It returns the appropriate line from file or indicates client that the line was not found.\n\n\
Usage: ./server -p PORT\n\
  PORT - Port number on which server will run, in range of 1025 to 65535\n";

// Macro to get rid of unused parameter warnings
#define UNUSED(x) (void)(x)

void AlarmHandler(int sig) {
	UNUSED(sig);
	signal(SIGALRM, SIG_IGN); // Ignore this signal while we handle it
	throw TimeoutException("Witing for message timed out.", Exception::TIMEOUT);
}

void CloseHandler(int sig) {
	UNUSED(sig);
	signal(SIGINT, SIG_IGN); // Ignore this signal while we handle it
	if(globalConnection != NULL) {
		globalConnection->CloseServer();
	}
	exit(0);
}

int main(int argc, char** argv)
{
	// Set AlarmHandler() function to handle SIGALRM signal for timeout
	signal(SIGALRM, AlarmHandler);
	signal(SIGINT, CloseHandler);

	pid_t pid;

	// Create new object and try to parse client specific arguments 
	ServerArguments args(argc,argv);
	try {
		args.ParseArguments();
	} catch(WrongArgumentsException ex) {
		cerr << ex.getMessage() << endl;
		exit(ex.getCode());
	}	

	// Print help if requested
	if(args.IsHelpSet()) {
		cout << helpMessage << endl;
		return 0;
	}

	// Create new connection object with port number specified in arguments
	ServerConnection connection(args.GetPortNumber());
	globalConnection = &connection;
	try {
		connection.CreateSocket(); // Create new socket for server
		connection.Bind(); // Bind this socket
		connection.Listen(); // Listen on socket
	} catch (ConnectionException ex){
		cerr << ex.getMessage() << endl;
		exit(ex.getCode());
	}

	// Infinite loop for accepting and handling new connections from clients
	while(1) {

		try {
			connection.Accept(); // Accept new connection
		// Catch all possible errors 
		} catch(ConnectionException ex) {
			continue;
		}

		// For new process for each client (concurrent server)
		if ((pid = fork()) == -1) { // Handle error

			try {
			connection.Close(); // Close client connection on error
			}catch(...) {
				cout << "here" << endl;
			}

		} else if(pid > 0) { // Parrent stuff

			/*
			─────────────────────────────────────
			──────█▀▄─▄▀▄─▀█▀─█─█─▀─█▀▄─▄▀▀▀─────
			──────█─█─█─█──█──█▀█─█─█─█─█─▀█─────
			──────▀─▀──▀───▀──▀─▀─▀─▀─▀──▀▀──────
			─────────────────────────────────────
			───────────────▀█▀─▄▀▄───────────────
			────────────────█──█─█───────────────
			────────────────▀───▀────────────────
			─────────────────────────────────────
			─────█▀▀▄─█▀▀█───█──█─█▀▀─█▀▀█─█▀▀───
			─────█──█─█──█───█▀▀█─█▀▀─█▄▄▀─█▀▀───
			─────▀▀▀──▀▀▀▀───▀──▀─▀▀▀─▀─▀▀─▀▀▀───
			─────────────────────────────────────
			─────────▄███████████▄▄──────────────
			──────▄██▀──────────▀▀██▄────────────
			────▄█▀────────────────▀██───────────
			──▄█▀────────────────────▀█▄─────────
			─█▀──██──────────────██───▀██────────
			█▀──────────────────────────██───────
			█──███████████████████───────█───────
			█────────────────────────────█───────
			█────────────────────────────█───────
			█────────────────────────────█───────
			█────────────────────────────█───────
			█────────────────────────────█───────
			█▄───────────────────────────█───────
			▀█▄─────────────────────────██───────
			─▀█▄───────────────────────██────────
			──▀█▄────────────────────▄█▀─────────
			───▀█▄──────────────────██───────────
			─────▀█▄──────────────▄█▀────────────
			───────▀█▄▄▄──────▄▄▄███████▄▄───────
			────────███████████████───▀██████▄───
			─────▄███▀▀────────▀███▄──────█─███──
			───▄███▄─────▄▄▄▄────███────▄▄████▀──
			─▄███▓▓█─────█▓▓█───████████████▀────
			─▀▀██▀▀▀▀▀▀▀▀▀▀███████████────█──────
			────█─▄▄▄▄▄▄▄▄█▀█▓▓─────██────█──────
			────█─█───────█─█─▓▓────██────█──────
			────█▄█───────█▄█──▓▓▓▓▓███▄▄▄█──────
			────────────────────────██──────────
			────────────────────────██───▄███▄───
			────────────────────────██─▄██▓▓▓██──
			───────────────▄██████████─█▓▓▓█▓▓██▄
			─────────────▄██▀───▀▀███──█▓▓▓██▓▓▓█
			─▄███████▄──███───▄▄████───██▓▓████▓█
			▄██▀──▀▀█████████████▀▀─────██▓▓▓▓███
			██▀─────────██──────────────██▓██▓███
			██──────────███──────────────█████─██
			██───────────███──────────────█─██──█
			██────────────██─────────────────█───
			██─────────────██────────────────────
			██─────────────███───────────────────
			██──────────────███▄▄────────────────
			███──────────────▀▀███───────────────
			─███─────────────────────────────────
			──███────────────────────────────────
			─────────────────────────────────────
			*/

			continue;

		} else if(pid == 0) { // Child stuff - handle client connection

			// sleep(10); // DEBUG	

			// Read client request and create new object which represents
			// abstraction over pwd.h search functions
			Searcher searcher(connection.ReadRequest());

			try {
			
				// Parse client patter - determine whether search for 
				// UIDs or logins, which values client requires and how
				// many UIDs or logins will client send
				searcher.ParsePattern();

				// Tell client that first request has been successfuly parsed and
				// connection is fully established
				connection.SendResponse("ESTABLISHED");

				while(1) {		    
					
					string request;

					// Read request from client and close connection on timeout
					request = connection.ReadRequest();

					// If client wants to end connection, close it
					if(request == "TERMINATE") {
						connection.Close();
						break;
					} else {

						// Create vector of return messages
						std::vector<std::string> response;

						// If we search for logins...
						if(searcher.GetSearchBy() == Searcher::LOGIN) {

							// Fill response by search result
							response = searcher.Search(request);

							// If search result is for some reason empty, 
							// send EMPTY message to client
							if(response.empty()) {
								connection.SendResponse("EMPTY");

							// Otherwise send results one by one (in case of multiple founds)
							// to client and always wait for NEXT message before sending next one
							} else {
								for (std::vector<std::string>::iterator iterator = response.begin();
										iterator != response.end(); ++iterator) {
									connection.SendResponse(*iterator);
									if(connection.ReadRequest() == "NEXT") {
										continue;
									} else {
										break;
									}
								}

								// Send END message if there is nothing more to send and
								// wait for next request from client
								connection.SendResponse("END");
							}

						// If we search for UIDs
						} else if (searcher.GetSearchBy() == Searcher::UID) {

							// Cast string value of UID to integer
							int uid = atoi(request.c_str());

							// Fill response by search result
							response = searcher.Search(uid);

							// If search result is for some reason empty, 
							// send EMPTY message to client
							if(response.empty()) {
								connection.SendResponse("EMPTY");

							// Otherwise send results one by one (in case of multiple founds)
							// to client and always wait for NEXT message before sending next one	
							} else {
								for (std::vector<std::string>::iterator iterator = response.begin(); iterator != response.end(); ++iterator) {
									connection.SendResponse(*iterator);
									if(connection.ReadRequest() == "NEXT") {
										continue;
									} else {
										break;
									}
								}

								// Send END message if there is nothing more to send and
								// wait for next request from client
								connection.SendResponse("END");
							}

						}

						
					}

				}

				// Close connection after loop end (TERMINATE message from client)
				// Wont be successful if client closes connection first
				connection.Close();
	
				exit(0);

			} catch (SearcherException ex) {
				exit(ex.getCode());
			} catch(TimeoutException ex) {
				exit(ex.getCode());
			} catch(ConnectionException ex) {
				exit(ex.getCode());
			} catch(...) {
				exit(-1);
			}

		}

	}

	// Close server
	try {
		connection.CloseServer();
	} catch(ConnectionException ex) {
		exit(ex.getCode());
	}

	exit(0);

}

/* End of file server.cpp */
