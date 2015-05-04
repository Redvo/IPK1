/****************************************************************************************
 * Project 1 in course Computer Communications and Networks, 2014/2015
 * Author: Lukáš Rendvanský
 * Email: xrendv00@stud.fit.vutbr.cz
 * Date: 18.3.2015
 * File: client.cpp
 * Description: Implementation of client-side application - source code.
 ***************************************************************************************/

#include <string>
#include <iostream>
#include <sstream>
#include <signal.h>
#include "arguments.class.hpp"
#include "connection.class.hpp"
#include "exception.class.hpp"

using namespace std;

const string helpMessage = "\nProject 1 in course Computer Communications and Networks, 2014/2015\n\
Author: Lukas Rendvansky (xrendv00@stud.fit.vutbr.cz)\n\n\
Description: Client side implementation. This program requests for specific \
lines from /etc/passwd file on server machine. Conditions of search are specified in requests send to server. \
Server returns the appropriate line from file or it indicates that the requested line was not found.\n\n\
Usage: ./server -h HOSTNAME -p PORT -l LOGIN [...] | -u UID [...] [-L -U -G -N -H -S]\n\
  -h HOSTNAME - Hostname of server as IP address or domain name.\n\
  -p PORT - Port number on which server is running, in range of 1025 to 65535\n\
  -l LOGIN [...] - Logins we want to search for, separated by space.\n\
  -u UID [...] - UIDs we want to seach for, separated by space.\n\
  If parameters -u and -l are combined, considered is only last one.\n\
  -L - Return login.\n\
  -U - Return ID.\n\
  -G - Return group ID.\n\
  -N - Return name and full gecos.\n\
  -H - Return home directory.\n\
  -S - Return default shell.\n\
  If none of search condition is specified, server returns full info.\n";

// Macro to get rid of unused parameter warnings
#define UNUSED(x) (void)(x)

void AlarmHandler(int sig) {
	UNUSED(sig);
	signal(SIGALRM, SIG_IGN); // Ignore this signal while we handle it
	throw TimeoutException("Connection to server timed out.", Exception::TIMEOUT);
}

int main(int argc, char** argv)
{

	// Set AlarmHandler() function to handle SIGALRM signal for timeout
	signal(SIGALRM, AlarmHandler);

	// Create new object and try to parse client specific arguments 
	ClientArguments args(argc,argv);
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

	// Create new connection object with port number and server address (domain name or IP)
	// specified in arguments
	ClientConnection connection(args.GetPortNumber());
	connection.SetHostName(args.GetServerName());

	try {

		// Create new socket and try to connect to server
		connection.CreateSocket();
		connection.Connect();

		// Construction of establishing request (described in documentation)

		string search = static_cast<ostringstream*>( &(ostringstream() << args.GetSearchBy()) )->str();
		string count = static_cast<ostringstream*>( &(ostringstream() << args.GetSearchArray().size()))->str(); 
		string request;
		string response;
		request = search;
		request += "::";

		if(args.GetSearchValues().empty()) {

			request += "-::";

		} else {

			std::vector<char> values = args.GetSearchValues();

			for (std::vector<char>::iterator iterator = values.begin(); iterator != values.end(); ++iterator) {
		    	request += *iterator;
		    	request += ":";
			}

			request += ":";

		}

		request += count;

		// End of construction of establishing request, send it to server
		connection.SendRequest(request);

		// Read if server was able to successfuly parse establishing request
		// and close connection with appropriate message if not.
		if((response = connection.ReadResponse()) != "ESTABLISHED") {
			cerr << response << endl;
			connection.Close();
			exit(Exception::INVALID_RESPONSE);
		}

		// Create vector of all logins / uids we want to search for
		std::vector<string> searchs = args.GetSearchArray();

		// Loop through the vector and do with each value...
		for (std::vector<string>::iterator iterator = searchs.begin(); 
			iterator != searchs.end(); ++iterator) {
	    
			// ... send value to server as request ...
	    	connection.SendRequest(*iterator);

	 		// ... read response ...   	
	    	string response = connection.ReadResponse();

	    	// ... if response is OK ...
	    	if(response != "EMPTY") {

	    		// ... write response to client console ...
	    		cout << response;

	    		// ... and send request for NEXT value
	    		connection.SendRequest("NEXT");

	    		// Read response for next value
	    		response = connection.ReadResponse();

	    		// If server tells by END message, that there is nothing else to read,
	    		// continue to next login/uid and repeat this process
				while(response != "END") {
					// If there are multiple values, request and print them until
					// END message is not received
					cout << response;
					connection.SendRequest("NEXT");
					response = connection.ReadResponse();
				}

			// ... if UID or LOGIN was not found, print error message
			} else {

				if(args.GetSearchBy() == ClientArguments::LOGIN) {
					cerr << "Unknown login " << *iterator << endl;
				} else if(args.GetSearchBy() == ClientArguments::UID) {
					cerr << "Unknown UID " << *iterator << endl;
				}

			}

		}
		
		// If we got all we came for, send TERMINATE message and close connection.
		connection.SendRequest("TERMINATE");
		connection.Close();
		
	// Handle all possible errors
	} catch (ConnectionException ex){
		cerr << ex.getMessage() << endl;
		exit(ex.getCode());
	} catch (TimeoutException ex) {
		cerr << ex.getMessage() << endl;
		exit(ex.getCode());
	}

	return 0;
}