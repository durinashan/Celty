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
	std::mutex Configuration::_mtlock;
	std::shared_ptr<Configuration> Configuration::instance = nullptr;
	std::shared_ptr<Configuration>& Configuration::GetInstance(void) {
		if(!Configuration::instance) {
			std::lock_guard<std::mutex> lock(Configuration::_mtlock);
			if(!Configuration::instance)
				Configuration::instance.reset(new Configuration());
		}
		return Configuration::instance;
	}

	Configuration::Configuration(void) {

	}

	Configuration::~Configuration(void) {

	}

	bool Configuration::LoadConfig(std::string path) {
		if(!this->FileExists(path.c_str())) {
			return false;
		}
		std::ifstream file((this->cfg_path = path));
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

	void Configuration::ReloadConfiguration(void) {
		this->ActiveConfig.clear();
		this->LoadConfig(this->cfg_path);
	}

	bool Configuration::SettingEnabled(std::string key) {
		if(this->ActiveConfig.find(key) != this->ActiveConfig.end()) {
			if(this->ActiveConfig[key] == "yes") {
				return true;
			}
		}
		return false;
	}


}
