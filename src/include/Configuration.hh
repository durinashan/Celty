/*! \file Configuration.hh
	Celty config file parser
*/
#pragma once
#include <map>
#include <string>
#include <mutex>
#include <memory>
namespace Celty {
typedef std::map<std::string, std::string> cfg_map;
class Configuration {
	private:
	Configuration(void);
	static std::shared_ptr<Configuration> instance;

	bool FileExists(const char *path);
	std::string cfg_path;
	static std::mutex _mtlock;

	public:
	cfg_map ActiveConfig;

	static std::shared_ptr<Configuration> &GetInstance(void);

	~Configuration();

	bool LoadConfig(std::string path);

	void ReloadConfiguration(void);

	bool SettingEnabled(std::string key);
};
}
