/*! \file Statistician.cc
	Statistics collection machinery
*/
#include <Statistician.hh>
namespace Celty {
std::mutex Statistician::_mtlock;
std::shared_ptr<Statistician> Statistician::instance = nullptr;

std::shared_ptr<Statistician> &Statistician::GetInstance(void) {
	if (!Statistician::instance) {
		std::lock_guard<std::mutex> lock(Statistician::_mtlock);
		if (!Statistician::instance)
			Statistician::instance.reset(new Statistician());
	}
	return Statistician::instance;
}

Statistician::Statistician(void) {}

Statistician::~Statistician(void) {}

void Statistician::LoadClientList(std::string list) {}
}
