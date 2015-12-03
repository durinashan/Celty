/*! \file swarm.cc
	A memcached backed swarm module to keep information constant
*/
#include <libmemcached/memcached.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <Module.hh>

using Celty::Module;
using Celty::cfg_map;

static memcached_st* memc = nullptr;
static std::string connstr;

MODULE("Swarm")

Module::Module(void) {

}

Module::~Module(void) {
	if(memc != nullptr)
		memcached_free(memc);
}

void Module::AnnounceSettings(cfg_map& settings) {
	if(settings.find("SwarmMemcachedHost") == settings.end())
		std::cerr << "[!] Unable to load Swarm, no memcached host." << std::endl;
	connstr = "--SERVER=" + settings["SwarmMemcachedHost"] + ":" + settings["SwarmMemcachedPort"];
}

void Module::Run(void) {
	memc = memcached(connstr.c_str(), connstr.size());
}

void Module::Halt(void) {

}

