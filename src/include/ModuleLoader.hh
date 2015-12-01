/*! \file ModuleLoader.hh
	Module loading and unloading
*/
#pragma once

#include <Module.hh>

#include <map>
#include <string>
#include <vector>
#include <functional>
#include <mutex>
#include <memory>

namespace Celty {
	class ModuleLoader {
	private:
		typedef Celty::Module* init_t(void);
		typedef void tear_t(Celty::Module*);
		typedef const char* name_t(void);

		struct module_t {
			init_t* ctor;
			tear_t* dtor;
			void* handle;
			name_t* name;
			Module* modu;
		};

		static std::shared_ptr<ModuleLoader> instance;
		static std::string ending;

		static std::mutex _mtlock;

		ModuleLoader(void);

		bool GetDirContents(std::string dir, std::vector<std::string> &files);

	public:
		std::map<std::string, module_t*> LoadedModules;
		~ModuleLoader(void);

		static std::shared_ptr<ModuleLoader>& GetInstance(void);

		bool LoadModule(std::string module);
		bool UnloadModule(std::string module);
		bool ReloadModule(std::string module);

		bool LoadAll(std::string dir);
		bool UnloadAll(void);
		bool ReloadAll(void);

		void Foreach(std::function<void(Module*)> const& lambda);

		int GetLoadedModuleCount(void);

	};
}
