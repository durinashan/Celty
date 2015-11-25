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
		std::ifstream file;
		if(!this->FileExists(path.c_str())) {
			return false;
		}
		return true;
	}


	bool Configuration::FileExists(const char* path) {
		struct stat buffer;
  		return (stat(path, &buffer) == 0);
	}
}
