#include "MenuPause.h"
#include <Signals_Method.h>

namespace T3{
	namespace Menu{
		CPause::CPause(GUI::CMain& guiMain, const CB::Math::CVector2D& vSize) :
			GUI::CMenuScreen(guiMain, L"Pause", vSize, 6)
		{
			this->AddMenuButton(L"Return", CB::Signals::CMethod<CPause>(this, &CPause::EventReturn));
			this->AddMenuButton(L"New Game", CB::Signals::CMethod<CPause>(this, &CPause::EventNewGame));
			this->AddMenuButton(L"Return to Main Menu", CB::Signals::CMethod<CPause>(this, &CPause::EventMainMenu));
			this->AddMenuButton(L"Exit Game", CB::Signals::CMethod<CPause>(this, &CPause::EventExit));
		}

		CPause::~CPause(){
		}

		void	CPause::EventReturn(){
			this->OnReturn();
		}

		void	CPause::EventNewGame(){
			this->OnNewGame();
		}

		void	CPause::EventMainMenu(){
			this->OnMainMenu();
		}

		void	CPause::EventExit(){
			this->OnExit();
		}
	}
}