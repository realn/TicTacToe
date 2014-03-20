#include "GUIMain.h"

namespace T3{
	namespace GUI{
		const CB::CString	GUI_SHADER = L"GUIShader";
		const CB::CString	SHADER_CONST_COLOR = L"vColor";
		const CB::CString	SHADER_CONST_PROJ = L"mProj";

		CMain::CMain(CB::CRefPtr<CB::Graphic::IDevice> pDevice, CTextRenderer& TextRenderer, CShaderManager& ShdMng) :
			m_pDevice(pDevice),
			m_TextRender(TextRenderer),
			m_pVertexShader(ShdMng.Load(GUI_SHADER, CB::Graphic::ShaderType::Vertex)),
			m_pFragmentShader(ShdMng.Load(GUI_SHADER, CB::Graphic::ShaderType::Fragment)),
			m_Background(pDevice, ShdMng),
			m_uTransitMode(TransitionMode::None)
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

		CMain::~CMain(){
		}

		void	CMain::PushScreen(CB::CRefPtr<CScreen> pScreen, const bool bTransition){
			if(!bTransition){
				this->AddItem(pScreen);
			}
			else{
				this->m_pTransitScreen = pScreen;
				this->m_pTransitScreen->SetTransitionMode(TransitionMode::Show);
				if(!this->m_Items.IsEmpty()){
					this->m_Items.Last()->SetTransitionMode(TransitionMode::Hide);
				}
				this->m_uTransitMode = TransitionMode::Show;
			}
		}

		void	CMain::PopScreen(const bool bTransition){
			if(!bTransition){
				this->m_Items.Remove();
			}
			else if(!this->m_Items.IsEmpty()){
				this->m_pTransitScreen = this->m_Items.Remove();
				this->m_pTransitScreen->SetTransitionMode(TransitionMode::Hide);
				if(!this->m_Items.IsEmpty()){
					this->m_Items.Last()->SetTransitionMode(TransitionMode::Show);
				}
				this->m_uTransitMode = TransitionMode::Hide;
			}
		}

		void	CMain::Update(const float32 fTD){
			if(this->m_uTransitMode == TransitionMode::Show){
				if(!this->m_Items.IsEmpty() && this->m_Items.Last()->GetTransitionMode() == TransitionMode::Hide){
					this->m_Items.Last()->Update(fTD);
				}
				else if(this->m_pTransitScreen->GetTransitionMode() == TransitionMode::Show){
					this->m_pTransitScreen->Update(fTD);
				}
				else{
					this->AddItem(this->m_pTransitScreen);
					this->m_pTransitScreen.Release();
					this->m_uTransitMode = TransitionMode::None;
					this->m_Items.Last()->Update(fTD);
				}
			}
			else if(this->m_uTransitMode == TransitionMode::Hide){
				if(this->m_pTransitScreen->GetTransitionMode() == TransitionMode::Hide){
					this->m_pTransitScreen->Update(fTD);
				}
				else if(!this->m_Items.IsEmpty() && this->m_Items.Last()->GetTransitionMode() == TransitionMode::Show){
					this->m_Items.Last()->Update(fTD);
				}
				else{
					this->m_pTransitScreen.Release();
					this->m_uTransitMode = TransitionMode::None;
					if(!this->m_Items.IsEmpty()){
						this->m_Items.Last()->Update(fTD);
					}
				}
			}
			else{
				if(this->m_Items.IsEmpty())
					return;

				this->m_Items.Last()->Update( fTD );
			}
		}

		void	CMain::Render(){
			this->SetUpRender();

			if(this->m_uTransitMode == TransitionMode::Show){
				if(!this->m_Items.IsEmpty() && this->m_Items.Last()->GetTransitionMode() == TransitionMode::Hide){
					this->m_Items.Last()->Render();
				}
				else{
					this->m_pTransitScreen->Render();
				}
			}
			else if(this->m_uTransitMode == TransitionMode::Hide){
				if(this->m_pTransitScreen->GetTransitionMode() == TransitionMode::Hide){
					this->m_pTransitScreen->Render();
				}
				else if(!this->m_Items.IsEmpty()){
					this->m_Items.Last()->Render();
				}
			}
			else if(!this->m_Items.IsEmpty()){
				this->m_Items.Last()->Render();
			}

			this->FreeRender();
		}

		CB::CRefPtr<CB::Graphic::IDevice>	CMain::GetDevice(){
			return this->m_pDevice;
		}

		CTextRenderer&	CMain::GetTextRenderer(){
			return this->m_TextRender;
		}

		CFullscreenQuad&	CMain::GetBackground(){
			return this->m_Background;
		}

		const bool	CMain::IsInTransition() const{
			return this->m_uTransitMode != TransitionMode::None;
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

		void	CMain::SetProjection(const CB::Math::CMatrix& Matrix){
			this->m_pVertexShader->SetUniform(SHADER_CONST_PROJ, Matrix);
		}

		void	CMain::SetCursorPos(const CB::Math::CVector2D& vPosition){
			this->m_vCursorPos = vPosition;
		}

		const CB::Math::CVector2D&	CMain::GetCursorPos() const{
			return this->m_vCursorPos;
		}

		void	CMain::ProcessEvent(const CEvent& Event){
			if(Event.Type == EventType::MouseMove){
				this->SetCursorPos(Event.Position);
			}

			CB::Collection::CList<CB::CRefPtr<GUI::CScreen>> screens(this->m_Items);
			for(uint32 i = 0; i < screens.GetLength(); i++){
				screens[i]->ProcessEvent(Event);
			}
		}
	}
}