#include "MenuMain.h"

#include <Signals_Method.h>

namespace T3{
	namespace Menu{
		CMain::CMain(GUI::CMain& Main, const CB::Math::CVector2D& vSize) :
			GUI::CMenuScreen(Main, L"Tic Tac Toe", vSize, 6)
		{
			this->AddMenuButton(L"New Game", CB::Signals::CMethod<CMain>(this, &CMain::EventNewGame));
			this->AddMenuButton(L"Options", CB::Signals::CMethod<CMain>(this, &CMain::EventOptions));
			this->AddMenuButton(L"Exit", CB::Signals::CMethod<CMain>(this, &CMain::EventExit));
		}

		CMain::~CMain(){
		}

		void	CMain::EventNewGame(){
			this->OnNewGame();
		}

		void	CMain::EventOptions(){
		}

		void	CMain::EventExit(){
			this->OnExit();
		}
	}
}