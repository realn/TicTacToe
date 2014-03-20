#pragma once

#include "GUIMenuScreen.h"

namespace T3{
	namespace Menu{

		class CMain :
			public GUI::CMenuScreen
		{
		public:
			CMain(GUI::CMain& guiMain, const CB::Math::CVector2D& vSize);
			virtual ~CMain();

			CB::Signals::CSlot<>	OnNewGame;
			CB::Signals::CSlot<>	OnExit;

		private:
			void	EventNewGame();
			void	EventOptions();
			void	EventExit();
		};
	}
}