#include "GUIItem.h"
#include "GUIScreen.h"
#include "GUIMain.h"
#include "TextRenderer.h"

namespace T3{
	namespace GUI{
		IItem::IItem(CB::CRefPtr<CScreen> pScreen) :
			Manage::IItem<CScreen>(pScreen)
		{
		}

		CTextRenderer&	IItem::GetTR(){
			return this->m_pParent->GetParent()->GetTextRenderer();
		}
	}
}