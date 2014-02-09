#include "GUIScreen.h"

namespace T3{
	namespace GUI{
		CScreen::CScreen(CB::CRefPtr<CB::Graphic::IDevice> pDevice, const CB::Math::CVector2D& vSize) :
			m_pDevice(pDevice),
			m_vSize(vSize)
		{

		}

		CB::CRefPtr<CB::Graphic::IDevice>	CScreen::GetDevice() const{
			return this->m_pDevice;
		}

		const CB::Math::CVector2D	CScreen::GetSize() const{
			return this->m_vSize;
		}

		IControlList&	CScreen::GetControls() const{
			return this->m_Controls;
		}

		void	CScreen::Update(const float32 fTD){
			for(uint32 i = 0; i < this->m_Controls.GetLength(); i++){
				this->m_Controls[i]->Update(fTD);
			}
		}

		void	CScreen::Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice){
			for(uint32 i = 0; i < this->m_Controls.GetLength(); i++){
				this->m_Controls[i]->Render(pDevice);
			}
		}
	}
}