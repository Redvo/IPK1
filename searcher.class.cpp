/****************************************************************************************
 * Project 1 in course Computer Communications and Networks, 2014/2015
 * Author: Lukáš Rendvanský
 * Email: xrendv00@stud.fit.vutbr.cz
 * Date: 18.3.2015
 * File: searcher.class.cpp
 * Description: Implementation of searcher class - source code.
 ***************************************************************************************/

#include "exception.class.hpp"
#include "searcher.class.hpp"

void Searcher::ParsePattern() {

	std::string buffer;
	int state = Searcher::BEGINING;

	for(std::string::size_type i = 0; i < this->pattern.size(); ++i) {
	    switch(state) {
	    	case Searcher::BEGINING: {
	    		this->searchBy = this->pattern[i] - '0';
	    		i += 2;
	    		state = Searcher::SEARCHS;
	    	} break;
	    	case Searcher::SEARCHS: {
	    		if(this->pattern[i] == ':') {
	    			state = Searcher::NUMBER;
	    		} else {
	    			switch(this->pattern[i]) {
	    				case 'L': {this->login = true; this->searchValues.push_back(this->pattern[i]);} break;
	    				case 'U': {this->uid = true; this->searchValues.push_back(this->pattern[i]);} break;
	    				case 'G': {this->gid = true; this->searchValues.push_back(this->pattern[i]);}  break;
	    				case 'N': {this->name = true; this->searchValues.push_back(this->pattern[i]);}  break;
	    				case 'H': {this->home = true; this->searchValues.push_back(this->pattern[i]);}  break;
	    				case 'S': {this->shell = true; this->searchValues.push_back(this->pattern[i]);}  break;
	    				case '-': {
							this->login = true; this->searchValues.push_back('L');
	    					this->uid = true; this->searchValues.push_back('U');
	    					this->gid = true; this->searchValues.push_back('G');
	    					this->name = true; this->searchValues.push_back('N');
	    					this->home = true; this->searchValues.push_back('H');
	    					this->shell = true; this->searchValues.push_back('S');
	    				} break;
	    				default: continue;
	    			}
	    			i++;
	    		}
	    	} break;
	    	case Searcher::NUMBER: {
	    		this->numberOfSearchs = atoi(this->pattern.substr(i).c_str());
	    	} break;
	    	default: continue;
	    }
	}


}

int Searcher::GetSearchBy() {

	return this->searchBy;

}

int Searcher::GetNumberOfSearchs() {

	return this->numberOfSearchs;

}

std::vector<char> Searcher::GetSeachValues() {

	return this->searchValues;

}

std::vector<std::string> Searcher::Search(std::string value) {

	std::vector<std::string> returnVector;
	struct passwd *pwd;

	setpwent();

	while((pwd = getpwent()) != NULL) {
		if(std::string(pwd->pw_name) == value) {
			if(!this->searchValues.empty()) {
				std::string returnSearch = "";
				for (std::vector<char>::iterator iterator = this->searchValues.begin(); iterator != this->searchValues.end(); ++iterator) {
			    	switch(*iterator) {
			    		case 'L': {returnSearch += std::string(pwd->pw_name); returnSearch += " ";} break;
	    				case 'U': {returnSearch += static_cast<std::ostringstream*>( &(std::ostringstream() << pwd->pw_uid) )->str(); returnSearch += " "; } break;
	    				case 'G': {returnSearch += static_cast<std::ostringstream*>( &(std::ostringstream() << pwd->pw_gid) )->str(); returnSearch += " "; } break;
	    				case 'N': {returnSearch += std::string(pwd->pw_gecos); returnSearch += " ";} break;
	    				case 'H': {returnSearch += std::string(pwd->pw_dir); returnSearch += " ";} break;
	    				case 'S': {returnSearch += std::string(pwd->pw_shell); returnSearch += " ";} break;
	    				default: continue;
			    	}
				}
				returnSearch += "\n";
				returnVector.push_back(returnSearch);
			}
		}
	}

	endpwent();

	return returnVector;
	
}

std::vector<std::string> Searcher::Search(unsigned int value) {

	std::vector<std::string> returnVector;
	struct passwd *pwd;

	setpwent();

	while((pwd = getpwent()) != NULL) {
		if(pwd->pw_uid == value) {
			if(!this->searchValues.empty()) {
				std::string returnSearch = "";
				for (std::vector<char>::iterator iterator = this->searchValues.begin(); iterator != this->searchValues.end(); ++iterator) {
			    	switch(*iterator) {
			    		case 'L': {returnSearch += std::string(pwd->pw_name); returnSearch += " ";} break;
	    				case 'U': {returnSearch += static_cast<std::ostringstream*>( &(std::ostringstream() << pwd->pw_uid) )->str(); returnSearch += " ";} break;
	    				case 'G': {returnSearch += static_cast<std::ostringstream*>( &(std::ostringstream() << pwd->pw_gid) )->str(); returnSearch += " ";} break;
	    				case 'N': {returnSearch += std::string(pwd->pw_gecos); returnSearch += " ";} break;
	    				case 'H': {returnSearch += std::string(pwd->pw_dir); returnSearch += " ";} break;
	    				case 'S': {returnSearch += std::string(pwd->pw_shell); returnSearch += " ";} break;
    					default: continue;
		    		}
				}
				returnSearch += "\n";
				returnVector.push_back(returnSearch);
			}
		}
	}

	endpwent();

	return returnVector;
	
}