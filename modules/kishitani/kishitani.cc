/*! \file kishitani.cc
	Kishitani web-ui connector
	https://github.com/XAMPP/Kishitani
*/
#include <pqxx/pqxx>
#include <iostream>
#include <Module.hh>

using Celty::Module;
using Celty::cfg_map;

static bool HasDB = false;
static pqxx::connection* con = nullptr;

MODULE("Kishitani")

Module::Module(void) {

}

Module::~Module(void) {
	if(con == nullptr)
		delete con;
}

void Module::AnnounceSettings(cfg_map& settings) {
	if(settings.find("KishitaniDB") == settings.end()) {
		std::cerr << "[!] Unable to load Kishitani backend, no DB settings" << std::endl;
	} else {
		HasDB = true;

	}
}

void Module::Run(void) {

}

void Module::Halt(void) {

}

