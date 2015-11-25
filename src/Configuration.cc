/*!\file Configuration.cc
	Configuration file parser
*/
#include <Configuration.hh>
#include <celty-config.hh>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>

namespace Celty {
	Configuration* Configuration::instance = nullptr;
	Configuration* Configuration::GetInstance(void) {
		return (Configuration::instance != nullptr) ? Configuration::instance : (Configuration::instance = new Configuration);
	}

	Configuration::Configuration(void) {

	}

	Configuration::~Configuration(void) {

	}

	bool Configuration::LoadConfig(std::string path) {
		if(!this->FileExists(path.c_str())) {
			return false;
		}
		std::ifstream file(path);
		std::string line;
		while(std::getline(file, line)) {
			if(line[0] == '#')
				continue;
			size_t pos = line.find('=');
			std::string key = line.substr(0,pos);
			std::string value = line.substr(pos+1, line.size());
			if(!key.empty() && !value.empty())
				this->ActiveConfig.insert(std::pair<std::string, std::string>(key, value));
		}
		return true;
	}


	bool Configuration::FileExists(const char* path) {
		struct stat buffer;
  		return (stat(path, &buffer) == 0);
	}
}
