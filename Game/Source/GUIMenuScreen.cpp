#include "GUIMenuScreen.h"

namespace T3{
	namespace GUI{
		CMenuScreen::CMenuScreen(CMain& Main, const CB::Math::CVector2D& vSize, const uint32 uItemPerPage) :
			CScreen(Main, vSize),
			m_uItemsPerPage(uItemPerPage)
		{

		}

		const bool	CMenuScreen::AddItem(CScreen::ItemPtr pItem){
			if(CScreen::AddItem(pItem)){
				uint32 uIndex = this->m_Items.GetLength();
				float32 fYStep = this->m_vSize.Y / (float32)(this->m_uItemsPerPage + 1);
				float32 fYSize = this->m_vSize.Y / (float32)this->m_uItemsPerPage;

				pItem->SetRect(CB::Math::CRectangleF32(0.0f, fYStep * (float32)uIndex, this->m_vSize.X, fYSize));

				return true;
			}
			return false;
		}
	}
}