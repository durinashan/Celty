/*! \file dummy.cc
	A test module
*/
#include <iostream>
#include <Module.hh>

using Celty::Module;
using Celty::cfg_map;

MODULE("DummyMod")

Module::Module(void) { /* Dummy Loaded */ }

Module::~Module(void) { /* Dummy Unloaded */ }

void Module::AnnounceSettings(cfg_map &settings) {
	/* Settings */
	UNUSED(settings);
}

void Module::Run(void) { /* Use to spin up any worker threads */ }

void Module::Halt(void) { /* Used to join said threads */ }
