/* \file ModuleLoader.cc
	Module loading implementation
*/
#include <iostream>

#include <ModuleLoader.hh>
#include <algorithm>

#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <sys/types.h>

#include <cstdlib>
#include <cstring>

#include <celty-config.hh>

namespace Celty {
std::mutex ModuleLoader::_mtlock;
std::shared_ptr<ModuleLoader> ModuleLoader::instance = nullptr;
std::string ModuleLoader::ending = DEFAULT_MODULEEXT;

ModuleLoader::ModuleLoader(void) {}

ModuleLoader::~ModuleLoader(void) { this->UnloadAll(); }

std::shared_ptr<ModuleLoader> &ModuleLoader::GetInstance(void) {
	if (!ModuleLoader::instance) {
		std::lock_guard<std::mutex> lock(ModuleLoader::_mtlock);
		if (!ModuleLoader::instance)
			ModuleLoader::instance.reset(new ModuleLoader());
	}
	return ModuleLoader::instance;
}

bool ModuleLoader::LoadModule(std::string module) {
	module_t *mod = new module_t;
	mod->handle = dlopen((module).c_str(), RTLD_LAZY);
	if (!mod->handle) {
		std::cout << "Error loading module '" << module << "'. " << dlerror() << std::endl;
		delete mod;
		return false;
	}
	mod->ctor = (init_t *)dlsym(mod->handle, "Init");
	mod->dtor = (tear_t *)dlsym(mod->handle, "Tear");
	mod->name = (name_t *)dlsym(mod->handle, "Name");
	if (mod->ctor == nullptr || mod->dtor == nullptr || mod->name == nullptr) {
		std::cout << "Error loading module '" << module << "'. I don't have the needed function pointers... "
				  << dlerror() << std::endl;
		dlclose(mod->handle);
		delete mod;
		return false;
	}
	mod->modu = mod->ctor();
	if (mod->modu == nullptr) {
		std::cout << "Error: unable to construct module '" << module << "'" << std::endl;
		dlclose(mod->handle);
		delete mod;
		return false;
	}
	this->LoadedModules[module] = mod;
	return true;
}
bool ModuleLoader::UnloadModule(std::string module) {
	module_t *mod = this->LoadedModules[module];
	mod->dtor(mod->modu);
	dlclose(mod->handle);
	delete mod;
	this->LoadedModules.erase(module);
	return true;
}
bool ModuleLoader::ReloadModule(std::string module) {
	if (this->LoadedModules.find(module) == this->LoadedModules.end())
		return false;
	/*
		Seeing as we are just reloading the module, I'm
		not going to reallocate the module_t.
	*/
	module_t *mod = this->LoadedModules[module];
	mod->dtor(mod->modu);
	dlclose(mod->handle);
	/* Flush the memory */
	memset(mod, 0, sizeof(module_t));
	/* Once that is done, we can load again */
	/*
		It's not quite duplicate code due to the fact that
		if it fails we need to remove the module for the
		loaded list
	*/
	mod->handle = dlopen((module).c_str(), RTLD_LAZY);
	if (!mod->handle) {
		std::cout << "Error loading module '" << module << "'. " << dlerror() << std::endl;
		delete mod;
		this->LoadedModules.erase(module);
		return false;
	}
	mod->ctor = (init_t *)dlsym(mod->handle, "Init");
	mod->dtor = (tear_t *)dlsym(mod->handle, "Tear");
	mod->name = (name_t *)dlsym(mod->handle, "Name");
	if (mod->ctor == nullptr || mod->dtor == nullptr || mod->name == nullptr) {
		std::cout << "Error loading module '" << module << "'. I don't have the needed function pointers... "
				  << dlerror() << std::endl;
		dlclose(mod->handle);
		delete mod;
		this->LoadedModules.erase(module);
		return false;
	}
	mod->modu = mod->ctor();
	if (mod->modu == nullptr) {
		std::cout << "Error: unable to construct module '" << module << "'" << std::endl;
		dlclose(mod->handle);
		delete mod;
		this->LoadedModules.erase(module);
		return false;
	}
	return true;
}

bool ModuleLoader::LoadAll(std::string dir) {
	std::vector<std::string> files;
	if (!this->GetDirContents(dir, files))
		return false;
	files.erase(std::remove_if(files.begin(), files.end(),
							   [](std::string name) {
								   if (name == ".." || name == ".")
									   return true;
								   if (name.length() < ModuleLoader::ending.length() + 1)
									   return true;
								   return (0 !=
										   name.compare(name.length() - ModuleLoader::ending.length(),
														ModuleLoader::ending.length(), ModuleLoader::ending));
							   }),
				files.end());
	for (auto &module : files) {
		if (!this->LoadModule(dir + "/" + module))
			return false;
	}
	return true;
}
bool ModuleLoader::UnloadAll(void) {
	for (auto &module : this->LoadedModules) {
		this->UnloadModule(module.first);
	}
	return true;
}
bool ModuleLoader::ReloadAll(void) {
	for (auto &module : this->LoadedModules) {
		module.second->modu->Halt();
		this->ReloadModule(module.first);
	}
	return true;
}

bool ModuleLoader::GetDirContents(std::string dir, std::vector<std::string> &files) {
	DIR *dp;
	struct dirent *de;
	if ((dp = opendir(dir.c_str())) == NULL)
		return false;
	while ((de = readdir(dp)) != NULL) {
		files.emplace_back(std::string(de->d_name));
	}
	closedir(dp);
	return true;
}

int ModuleLoader::GetLoadedModuleCount(void) { return this->LoadedModules.size(); }

void ModuleLoader::Foreach(std::function<void(Module *)> const &lambda) {
	for (auto &module : this->LoadedModules) {
		lambda(module.second->modu);
	}
}
}
