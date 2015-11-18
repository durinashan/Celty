/*! \file dummy.cc
	A test module
*/
#include <iostream>
#include <Module.hh>
using Celty::Module;

MODULE("DummyMod")

Module::Module(void) {
	std::cout << "[!] Dummy Module Loaded" << std::endl;
}

Module::~Module(void) {
	std::cout << "[!] Dummy Module Unloaded" << std::endl;
}
