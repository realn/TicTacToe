#include "GUIManager.h"

namespace T3{
	namespace GUI{

		const CB::CString GUI_SHADER = L"GUIShader";

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

			CB::Collection::CList<CVertexElement> els;
			els.Add(CVertexElement(0, L"vinput.vPosition", VertexType::Float, 3, 0));
			els.Add(CVertexElement(0, L"vinput.vTexCoord", VertexType::Float, 2, els.Last().GetSize()));

			this->m_pVertexDeclaration = pDevice->CreateVertexDeclaration(this->m_pVShader, els);
		}

		void	CManager::PushScreen(CB::CRefPtr<IScreen> pScreen){
			this->m_Screens.Insert(0, pScreen);
		}

		void	CManager::PopScreen(){
			if(!this->m_Screens.IsEmpty()){
				this->m_Screens.Remove(0);
			}
		}

		void	CManager::Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice){
			pDevice->SetShader(this->m_pVShader);
			pDevice->SetShader(this->m_pFShader);
			pDevice->SetState(this->m_pBlendState);
			pDevice->SetVertexDeclaration(this->m_pVertexDeclaration);

			this->m_pVShader->SetUniform(L"mModelViewProj", 
				CB::Math::CMatrix::GetOrtho(2.0f, 2.0f, -1.0f, 1.0f)
				);

			for(uint32 i = 0; i < this->m_Screens.GetLength(); i++){
				this->m_Screens[i]->Render(pDevice);
			}

			pDevice->FreeVertexDeclaration();
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