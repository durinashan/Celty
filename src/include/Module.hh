/*! \file Module.hh
	Celty module interface
*/
#pragma once

#define UNUSED(X) X = X

#define MODULE(NAME)                         \
	extern "C" Celty::Module* Init(void) {   \
		return new Celty::Module();          \
	}                                        \
	extern "C" void Tear(Celty::Module* m) { \
		if(m != nullptr) delete m;           \
	}                                        \
	extern "C" const char* Name(void) {      \
		return NAME;                         \
	}                                        \

namespace Celty {
	class Module {
	public:
		Module(void);
		virtual ~Module(void);

		// TODO: Add Module interface
	};
}
