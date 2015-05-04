/****************************************************************************************
 * Project 1 in course Computer Communications and Networks, 2014/2015
 * Author: Lukáš Rendvanský
 * Email: xrendv00@stud.fit.vutbr.cz
 * Date: 18.3.2015
 * File: searcher.class.hpp
 * Description: Implementation of searcher class - header file.
 ***************************************************************************************/

#ifndef SEARCH_H
#define SEARCH_H

#include <string>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <pwd.h>

class Searcher {

	public:
		Searcher(std::string pattern) {
			this->pattern = pattern;
			this->login = false;
			this->uid = false;
			this->gid = false;
			this->name = false;
			this->home = false;
			this->shell = false;
		}

		enum searchBy {
			LOGIN = 0,
			UID = 1
		};

		int GetSearchBy();

		void ParsePattern();

		int GetNumberOfSearchs();

		std::vector<char> GetSeachValues();

		std::vector<std::string> Search(std::string value);

		std::vector<std::string> Search(unsigned int value);

	private:

		std::string pattern;

		enum States {
			BEGINING = 0,
			SEARCHS = 1,
			NUMBER = 2
		};

		int searchBy;

		std::vector<char> searchValues;

		int numberOfSearchs;

		bool login;
		bool uid;
		bool gid;
		bool name;
		bool home;
		bool shell;

};

#endif /* SEARCH_H */