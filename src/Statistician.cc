/*! \file Statistician.cc
	Statistics collection machinery
*/
#include <Statistician.hh>
namespace Celty {
	Statistician* Statistician::instance = nullptr;

	Statistician* Statistician::GetInstance(void) {
		return (Statistician::instance != nullptr) ? Statistician::instance : (Statistician::instance = new Statistician());
	}

	Statistician::Statistician(void) {

	}

	Statistician::~Statistician(void) {

	}
}
