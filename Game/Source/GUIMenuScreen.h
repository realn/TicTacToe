#pragma once

#include "GUIScreen.h"

namespace T3{
	namespace GUI{
		

		class CMenuScreen :
			public CScreen
		{
		protected:
			const uint32	m_uItemsPerPage;

		public:
			CMenuScreen(CMain& Main, const CB::Math::CVector2D& vSize, const uint32 uItemsPerPage);

			virtual const bool	AddItem(CScreen::ItemPtr pItem) override;
		};
	}
}