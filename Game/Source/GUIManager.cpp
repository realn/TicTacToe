#include "GUIManager.h"

namespace T3{
	namespace GUI{

		const CB::CString GUI_SHADER = L"GUIShader.cg";

		CManager::CManager(CB::CRefPtr<CB::Graphic::IDevice> pDevice, CShaderManager& ShdMng) :
			m_pVShader(ShdMng.Load(GUI_SHADER, CB::Graphic::ShaderType::Vertex)),
			m_pFShader(ShdMng.Load(GUI_SHADER, CB::Graphic::ShaderType::Fragment))
		{
			using namespace CB::Graphic;
			CBlendStateDesc desc(
				true,
				CBlendInstDesc(BlendOption::SourceAlpha, BlendOperation::Add, BlendOption::OneMinusSourceAlpha),
				CBlendInstDesc(BlendOption::SourceAlpha, BlendOperation::Add, BlendOption::OneMinusSourceAlpha),
				0xFF);

			this->m_pBlendState = pDevice->CreateState(desc).Cast<IDeviceState>();
		}

		void	CManager::PushScreen(CB::CRefPtr<IScreen> pScreen){
			this->m_Screens.Add(pScreen);
		}

		void	CManager::PopScreen(){
			if(!this->m_Screens.IsEmpty()){
				uint32 uIndex = this->m_Screens.GetLength() - 1;
				this->m_Screens.Remove(uIndex);
			}
		}

		void	CManager::Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice){
			pDevice->SetShader(this->m_pVShader);
			pDevice->SetShader(this->m_pFShader);
			pDevice->SetState(this->m_pBlendState);

			for(uint32 i = this->m_Screens.GetLength(); i > 0; i++){
				this->m_Screens[i]->Render(*this);
			}

			pDevice->FreeState(CB::Graphic::DeviceStateType::Blend);
			pDevice->FreeShader(CB::Graphic::ShaderType::Vertex);
			pDevice->FreeShader(CB::Graphic::ShaderType::Fragment);
		}

		void	CManager::Update(const float32 fTD){
			for(uint32 i = 0; i < this->m_Screens.GetLength(); i++){
				this->m_Screens[i]->Update(fTD);
			}
		}
	}
}