#include "GUIMain.h"

namespace T3{
	namespace GUI{
		CMain::CMain(CB::CRefPtr<CB::Graphic::IDevice> pDevice, CTextRenderer& TextRenderer) :
			m_pDevice(pDevice),
			m_TextRender(TextRenderer)
		{
		}

		void	CMain::PushScreen(CB::CRefPtr<CScreen> pScreen){
			this->AddItem(pScreen);
		}

		void	CMain::PopScreen(){
			this->m_Items.Remove();
		}
	}
}