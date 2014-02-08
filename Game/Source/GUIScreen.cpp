#include "GUIScreen.h"

namespace T3{
	namespace GUI{
		CScreen::CScreen(CB::CRefPtr<CB::Graphic::IDevice> pDevice, const CB::Math::CVector2D& vSize) :
			m_pDevice(pDevice)
		{

		}

		void	CScreen::Update(const float32 fTD){
		}
	}
}