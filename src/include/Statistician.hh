/*! \file Statistician.hh
	Statistics collection machinery
*/
#pragma once
namespace Celty {
	class Statistician {
	private:
		Statistician(void);

		static Statistician* instance;
	public:
		~Statistician(void);

		static Statistician* GetInstance(void);
	};
}
