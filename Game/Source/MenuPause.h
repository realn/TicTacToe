#pragma once

#include "GUIMenuScreen.h"

namespace T3{
	namespace Menu{
		class CPause :
			public GUI::CMenuScreen
		{
		public:
			CPause(GUI::CMain& guiMain, const CB::Math::CVector2D& vScreen);
			virtual ~CPause();

			CB::Signals::CSlot<>	OnReturn;
			CB::Signals::CSlot<>	OnNewGame;
			CB::Signals::CSlot<>	OnMainMenu;
			CB::Signals::CSlot<>	OnExit;

		private:
			void	EventReturn();
			void	EventNewGame();
			void	EventMainMenu();
			void	EventExit();
		};
	}
}