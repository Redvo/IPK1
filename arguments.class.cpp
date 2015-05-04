/****************************************************************************************
 * Project 1 in course Computer Communications and Networks, 2014/2015
 * Author: Lukáš Rendvanský
 * Email: xrendv00@stud.fit.vutbr.cz
 * Date: 18.3.2015
 * File: arguments.class.cpp
 * Description: Class storing arguments configuration - both client and server sides.
 ***************************************************************************************/

#include "arguments.class.hpp"

/************************************************************
******************** COMMON ARGUMENTS ***********************
************************************************************/

int Arguments::GetArgumentCount() {
	return this->argumentCount;
}

std::vector<std::string> Arguments::GetArguments() {
	return this->arguments;
}

void Arguments::PrintArguments() {
	for (std::vector<std::string>::iterator iterator = this->arguments.begin(); iterator != this->arguments.end(); ++iterator) {
    	std::cout << *iterator << std::endl;
	}	
}

bool Arguments::IsHelpSet() {
	return this->helpSet;
}

void Arguments::SetHelp(bool value) {
	this->helpSet = value;
}

void Arguments::SetPortNumber(long value) {
	this->portNumber = value;
}

long Arguments::GetPortNumber() {
	return this->portNumber;
}

/************************************************************
******************** SERVER ARGUMENTS ***********************
************************************************************/

void ServerArguments::ParseArguments() {

	int argCount = this->GetArgumentCount();	
	std::vector<std::string> arguments = this->GetArguments();

	long portNumber = 0;
	char* error;

	if(argCount == 2 && arguments.front() == "--help") {

			this->SetHelp(true);
			return;

	} else if(argCount == 3 && arguments.front() == "-p") {

			portNumber = std::strtol(arguments.back().c_str(),&error,10);

			if(*error != '\0' || portNumber < MIN_PORT_NUMBER || portNumber > MAX_PORT_NUMBER) {

				throw WrongArgumentsException("Invalid port number. Run with --help for detailed instructions.", 
					Exception::WRONG_ARGUMENT);

			}

			this->SetPortNumber(portNumber);
			return;

	} else {

		throw WrongArgumentsException("Invalid arguments. Run with --help for detailed instructions.",
			Exception::WRONG_ARGUMENT);

	}

	return;
	
}	

/************************************************************
******************** CLIENT ARGUMENTS ***********************
************************************************************/

void ClientArguments::SetServerName(std::string value) {
	this->serverName = value;
}

std::string ClientArguments::GetServerName() {
	return this->serverName;
}

std::vector<std::string> ClientArguments::GetSearchArray() {
	return this->searchArray;
}

void ClientArguments::PrintSearchArray() {
	for (std::vector<std::string>::iterator iterator = this->searchArray.begin(); iterator != this->searchArray.end(); ++iterator) {
    	std::cout << *iterator << std::endl;
	}
}

void ClientArguments::TruncateSearchArray() {
	this->searchArray.clear();
}

void ClientArguments::AddToSearchArray(std::string value) {
	std::locale l;
	for (std::string::size_type i=0; i<value.length(); ++i) {
    	value[i] = std::tolower(value[i],l);
	}

	if (std::find(this->searchArray.begin(), this->searchArray.end(), value) == this->searchArray.end()) {
		this->searchArray.push_back(value);
	}
}

std::vector<char> ClientArguments::GetSearchValues() {
	return this->searchValues;
}

void ClientArguments::PrintSearchValues() {
	for (std::vector<char>::iterator iterator = this->searchValues.begin(); iterator != this->searchValues.end(); ++iterator) {
    	std::cout << *iterator << std::endl;
	}
}

void ClientArguments::TruncateSearchValues() {
	this->searchValues.clear();
}

void ClientArguments::AddToSearchValues(char value) {
	if (std::find(this->searchValues.begin(), this->searchValues.end(), value) == this->searchValues.end()) {
		this->searchValues.push_back(value);
	}
}

int ClientArguments::GetSearchBy() {
	return this->searchBy;
}

void ClientArguments::SetSearchBy(int value) {
	this->searchBy = value;
}

void ClientArguments::ParseArguments() {

	int argCount = this->GetArgumentCount();	
	std::vector<std::string> arguments = this->GetArguments();

	long portNumber = 0;
	char *error;
	std::string serverName = "";

	bool portSet = false;
	bool serverSet = false;
	bool searchSet = false;

	if(argCount == 2 && arguments.front() == "--help") {

			this->SetHelp(true);
			return;

	} else {

		for(std::vector<std::string>::iterator iterator = arguments.begin(); iterator != arguments.end(); ++iterator) {

			// Set port number (-p portNumber)
			if(*iterator == "-p" && ((iterator+1) != arguments.end())) {

				if(portSet) {
					throw WrongArgumentsException("Error: cannot set multiple ports. Run with --help for detailed instructions.",
						Exception::WRONG_ARGUMENT);
				} else {
					portSet = true;
				}

				portNumber = std::strtol((*(++iterator)).c_str(),&error,10);

				if(*error != '\0' || portNumber < MIN_PORT_NUMBER || portNumber > MAX_PORT_NUMBER) {

					throw WrongArgumentsException("Invalid port number. Run with --help for detailed instructions.",
						Exception::WRONG_ARGUMENT);
				
				}

				this->SetPortNumber(portNumber);

			// Set server name (-h serverName)
			} else if(*iterator == "-h" && ((iterator+1) != arguments.end())) {

				if(serverSet) {
					throw WrongArgumentsException("Error: cannot set multiple server names. Run with --help for detailed instructions.",
						Exception::WRONG_ARGUMENT);
				} else {
					serverSet = true;
				}

				iterator++;
				this->SetServerName(std::string(*(iterator)));

			// Set search by login (-l login...)
			} else if(*iterator == "-l" && ((iterator+1) != arguments.end())) {

				searchSet = true;

				this->SetSearchBy(this->LOGIN);

				this->TruncateSearchArray();

				if((*(iterator+1))[0] == '-') {
					throw WrongArgumentsException("Error: no search LOGIN specified. Run with --help for detailed instructions.",
						Exception::WRONG_ARGUMENT);
				}

				iterator++;

				while(iterator != arguments.end()) {

					if((*(iterator))[0] == '-') {
						break;
					}


					this->AddToSearchArray(std::string(*(iterator)));

					iterator++;	

				}

				iterator--;

			// Set search by UID (-u uid...)
			} else if(*iterator == "-u" && ((iterator+1) != arguments.end())) {

				searchSet = true;

				char *err;

				this->SetSearchBy(this->UID);

				this->TruncateSearchArray();

				if((*(iterator+1))[0] == '-') {
					throw WrongArgumentsException("Error: no search UID specified. Run with --help for detailed instructions.",
						Exception::WRONG_ARGUMENT);
				}

				iterator++;

				while(iterator != arguments.end()) {

					if((*(iterator))[0] == '-') {
						break;
					}

					strtol((*iterator).c_str(),&err,10);
					if(*err != '\0') {
						throw WrongArgumentsException("Error: UID must be decimal number. Run with --help for detailed instructions.",
							Exception::WRONG_ARGUMENT);
					}

					this->AddToSearchArray(std::string(*(iterator)));

					iterator++;	

				}

				iterator--;

			// Set searched values (L,U,G,N,H,S)
			} else if((*iterator)[0] == '-') { 

				for(unsigned int i=1; i < (*iterator).size(); i++) {
					switch((*iterator)[i]) {
						case 'L':
						case 'U':
						case 'G':
						case 'N':
						case 'H':
						case 'S': {
							this->AddToSearchValues(std::string(*(iterator))[i]);
						} break;
						default: {
							throw WrongArgumentsException("Invalid arguments. Run with --help for detailed instructions.",
								Exception::WRONG_ARGUMENT);
						}
					}
				}

			// Something unknown?!
			} else {
				throw WrongArgumentsException("Invalid arguments. Run with --help for detailed instructions.",
					Exception::WRONG_ARGUMENT);
			}

		}

		if(!serverSet || !portSet || !searchSet) {
			throw WrongArgumentsException("Invalid arguments. Run with --help for detailed instructions.",
				Exception::WRONG_ARGUMENT);
		}

	}

	return;

}

