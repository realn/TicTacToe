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

		void	CScreen::AddControl(CB::CRefPtr<IControl> pControl){
			if(!CB::Collection::Contains(this->m_Controls, pControl)){
				this->m_Controls.Add(pControl);
				pControl->SetParentNull();
			}
		}

		void	CScreen::Update(const float32 fTD){
			for(uint32 i = 0; i < this->m_Controls.GetLength(); i++){
				this->m_Controls[i]->Update(fTD);
			}
		}

		void	CScreen::Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice){
			auto mMatrix = CB::Math::CMatrix::GetOrtho(0.0f, this->m_vSize.X, 0.0f, this->m_vSize.Y, -1.0f, 1.0f);

			this->m_pDevice->GetShader(CB::Graphic::ShaderType::Vertex)->SetUniform(L"mModelViewProj", mMatrix);
			for(uint32 i = 0; i < this->m_Controls.GetLength(); i++){
				this->m_Controls[i]->Render(pDevice);
			}
		}
	}
}