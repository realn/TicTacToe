#include "GUIScreen.h"
#include "GUIMain.h"

namespace T3{
	namespace GUI{
		CScreen::CScreen(CB::CRefPtr<CMain> pMain, const CB::Math::CVector2D& vSize) :
			Manage::IItem<CMain>(pMain),
			m_vSize(vSize)
		{

		}
	}
}