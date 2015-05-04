/****************************************************************************************
 * Project 1 in course Computer Communications and Networks, 2014/2015
 * Author: Lukáš Rendvanský
 * Email: xrendv00@stud.fit.vutbr.cz
 * Date: 18.3.2015
 * File: arguments.class.hpp
 * Description: Class storing arguments configuration - header file.
 ***************************************************************************************/

#ifndef ARG_CLASS
#define ARG_CLASS

#include <vector>	
#include <string>
#include <iostream>
#include <algorithm>

#include "exception.class.hpp"

#define MIN_PORT_NUMBER 1025
#define MAX_PORT_NUMBER 65535

class Arguments {

	public:
		explicit Arguments(int argc, char** argv) {
			this->argumentCount = argc;
			this->arguments.insert(arguments.end(), argv+1, argv+argc);
			this->helpSet = false;
		}

		~Arguments() {}

		int GetArgumentCount();

		std::vector<std::string> GetArguments();

 		void PrintArguments();

 		bool IsHelpSet();

		long GetPortNumber();

	private:
		int argumentCount;
		std::vector<std::string> arguments;
		bool helpSet;
		long portNumber;

	protected:
		void SetPortNumber(long value);

		void SetHelp(bool value);

};

class ClientArguments: public Arguments {

	public:
		explicit ClientArguments(int argc, char** argv) : Arguments(argc,argv) {};

		virtual ~ClientArguments() {}

		enum SearchBy {
			LOGIN = 0, 
			UID = 1
		};

		void ParseArguments();

		std::string GetServerName();

		std::vector<std::string> GetSearchArray();

		void PrintSearchArray();

		std::vector<char> GetSearchValues();

		void PrintSearchValues();

		int GetSearchBy();

	private:
		std::string serverName;
		std::vector<std::string> searchArray;
		std::vector<char> searchValues;
		int searchBy;

		void SetServerName(std::string value);

		void TruncateSearchArray();

		void AddToSearchArray(std::string value);

		void TruncateSearchValues();

		void AddToSearchValues(char value);

		void SetSearchBy(int value);

};

class ServerArguments: public Arguments {

	public:
		explicit ServerArguments(int argc, char** argv) : Arguments(argc,argv) {};

		virtual ~ServerArguments() {}

		void ParseArguments();

};

#endif /* ARG_CLASS */