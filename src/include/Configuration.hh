/*! \file Configuration.hh
	Celty config file parser
*/
#pragma once
#include <map>
#include <string>
namespace Celty {
	typedef	std::map<std::string, std::string> cfg_map;
	class Configuration {
	private:
		Configuration(void);
		static Configuration* instance;

		bool FileExists(const char* path);

	public:
		cfg_map ActiveConfig;

		static Configuration* GetInstance(void);

		~Configuration();

		bool LoadConfig(std::string path);

	};
}
