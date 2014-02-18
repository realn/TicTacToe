#include "GUIMain.h"

namespace T3{
	namespace GUI{
		const CB::CString	GUI_SHADER = L"GUIShader";
		const CB::CString	SHADER_CONST_COLOR = L"vColor";

		CMain::CMain(CB::CRefPtr<CB::Graphic::IDevice> pDevice, CTextRenderer& TextRenderer, CShaderManager& ShdMng) :
			m_pDevice(pDevice),
			m_TextRender(TextRenderer),
			m_pVertexShader(ShdMng.Load(GUI_SHADER, CB::Graphic::ShaderType::Vertex)),
			m_pFragmentShader(ShdMng.Load(GUI_SHADER, CB::Graphic::ShaderType::Fragment))
		{
			using namespace CB::Graphic;

			{
				CB::Collection::CList<CVertexElement> vEls;
				vEls.Add(CVertexElement(0, L"input.vPosition", VertexType::Float, 3, 0));
				vEls.Add(CVertexElement(1, L"input.vTexCoord", VertexType::Float, 2, 0));
				this->m_pVertexDeclaration = this->m_pDevice->CreateVertexDeclaration(this->m_pVertexShader, vEls);
			}

			{
				CBlendInstDesc inst(BlendOption::SourceAlpha, BlendOperation::Add, BlendOption::OneMinusSourceAlpha);
				CBlendStateDesc desc(true, inst, inst, 0xFF);
				this->m_pBState = this->m_pDevice->CreateState(desc).Cast<IDeviceState>();
			}
		}

		void	CMain::PushScreen(CB::CRefPtr<CScreen> pScreen){
			this->AddItem(pScreen);
		}

		void	CMain::PopScreen(){
			this->m_Items.Remove();
		}

		void	CMain::Render(){
			this->SetUpRender();

			for(uint32 i = 0; i < this->m_Items.GetLength(); i++){
				this->m_Items[i]->Render();
			}

			this->FreeRender();
		}

		void	CMain::Update(const float32 fTD){
			for(uint32 i = 0; i < this->m_Items.GetLength(); i++){
				this->m_Items[i]->Update(fTD);
			}
		}

		CB::CRefPtr<CB::Graphic::IDevice>	CMain::GetDevice(){
			return this->m_pDevice;
		}

		CTextRenderer&	CMain::GetTextRenderer(){
			return this->m_TextRender;
		}

		void	CMain::SetUpRender(){
			this->m_pDevice->SetVertexDeclaration(this->m_pVertexDeclaration);
			this->m_pDevice->SetShader(this->m_pVertexShader);
			this->m_pDevice->SetShader(this->m_pFragmentShader);
			this->m_pDevice->SetState(this->m_pBState);

			this->SetColor(CB::Math::CColor(1.0f));
		}

		void	CMain::FreeRender(){
			this->m_pDevice->FreeShader(CB::Graphic::ShaderType::Vertex);
			this->m_pDevice->FreeShader(CB::Graphic::ShaderType::Fragment);
			this->m_pDevice->FreeState(CB::Graphic::DeviceStateType::Blend);
			this->m_pDevice->FreeVertexDeclaration();
		}

		void	CMain::SetColor(const CB::Math::CColor& Color){
			this->m_pFragmentShader->SetUniform(SHADER_CONST_COLOR, Color.ToVector4D());
		}
	}
}