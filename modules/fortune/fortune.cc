/*! \file dummy.cc
	A test module
*/
#include <iostream>
#include <Module.hh>
#include <syslog.h>
#include <random>
#include <chrono>

using Celty::Module;
using Celty::cfg_map;

MODULE("Fortune")

static bool fortune = false;

static char *fortunes[] = {
	" The problem with America is stupidity. I'm not saying there should be a capital punishment for stupidity, but "
	"why don't we just take the safety labels off of everything and let the problem solve itself?",
	"I haven't lost my mind; I have a tape back-up somewhere.",
	"The problem with troubleshooting is that the trouble shoots back.", "I script in C, because fuck you."};

Module::Module(void) {}

Module::~Module(void) {}

void Module::AnnounceSettings(cfg_map &settings) {
	if (settings.find("EnableFortunes") != settings.end()) {
		if (settings["EnableFortunes"] == "yes") {
			fortune = true;
		}
	}
}

void Module::Run(void) {
	if (fortune) {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::mt19937 mtwisat(seed);
		char *f = fortunes[mtwisat() % 4];
		syslog(LOG_DEBUG, f);
		std::cout << "[*] " << f << std::endl;
	}
}

void Module::Halt(void) {}
