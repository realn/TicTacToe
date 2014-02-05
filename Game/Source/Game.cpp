#include "Game.h"
#include <Timer.h>
#include <Signals_Method.h>
#include "TextureManager.h"
#include <IO_Path.h>
#include <IO_File.h>
#include <IO_TextReader.h>

namespace T3{
	CGame::CGame(const CB::Collection::ICountable<CB::CString>& strArgs, CGameEnv& Env, CGameConfig& Config) :
		m_State(GameState::MainMenu),
		m_Env(Env),
		m_Config(Config),
		m_TextureManager(Env.GetDevice(), Config.AssetsDir),
		m_ShaderManager(Env.GetDevice(), Config.AssetsDir, CB::Graphic::ShaderVersion::ShaderModel_2),
		m_uField(CLevel::FieldType::Cross)
	{

		{
			auto pWindow = this->m_Env.GetWindow();
			pWindow->OnClose += CB::Signals::CMethod<CGame, const bool, CB::CRefPtr<CB::Window::IWindow>>(this, &CGame::WindowClose);
		}

		this->m_Env.OnMouseMove += CB::Signals::CMethod<CGame, void, const CB::Math::CVector2D&>(this, &CGame::EventMouseMove);
		this->m_Env.OnMouseDown += CB::Signals::CMethod<CGame, void, const CB::Math::CVector2D&, const CB::Window::VirtualKey>(this, &CGame::EventMouseDown);
		
		this->m_pCursor = new CGameCursor(this->m_Env.GetDevice(), this->m_Config.Resolution, this->m_TextureManager, this->m_ShaderManager);

		this->m_pLevel = new CLevel(this->m_Env.GetDevice(), this->m_TextureManager, this->m_ShaderManager); 
		this->m_pLevel->SetSceneSize(CB::Math::CVector2D(1.0f * this->m_Env.GetAspectRatio(), 1.0f));
		this->m_pLevel->SetGridPos(CB::Math::CVector3D((this->m_Env.GetAspectRatio() - 1.0f)/2.0f, 0.0f));

		this->m_pBackGround = new CBGGridModel(this->m_Env.GetDevice(), this->m_TextureManager, this->m_ShaderManager);
		this->m_pBackGround->SetScreenSize(CB::Math::CVector2D(1.0f * this->m_Env.GetAspectRatio(), 1.0f));
		this->m_pBackGround->SetGridPos(CB::Math::CVector3D((this->m_Env.GetAspectRatio() - 1.0f)/2.0f, 0.0f));
	}

	CGame::~CGame(){
		auto pWindow = this->m_Env.GetWindow();
		pWindow->OnClose.Clear(this);
		m_Env.OnMouseDown.Clear(this);
		m_Env.OnMouseMove.Clear(this);
	}

	const GameResult	CGame::MainLoop(){
		CB::CTimer	timer;

		do{
			this->Render();

			timer.Update();
			this->m_Env.Update();

			this->Update(timer.GetTimeDelta());

			this->m_Env.GetDevice()->Swap();
		}
		while(this->m_State != GameState::Shutdown);
		
		return GameResult::Shutdown;
	}

	const bool	CGame::WindowClose(CB::CRefPtr<CB::Window::IWindow> pWindow){
		this->m_State = GameState::Shutdown;

		return true;
	}

	void	CGame::EventMouseMove(const CB::Math::CVector2D& Position){
		this->m_pCursor->SetPos(Position);
		this->m_pLevel->SetMousePos(Position);
	}

	void	CGame::EventMouseDown(const CB::Math::CVector2D& Position, const CB::Window::VirtualKey Button){
		if(Button == CB::Window::VirtualKey::LBUTTON){
			this->m_pLevel->PutField(Position, m_uField);
			this->m_uField = this->m_uField == CLevel::FieldType::Cross ? CLevel::FieldType::Circle : CLevel::FieldType::Cross;
		}
	}

	void	CGame::Render(){
		auto pDev = this->m_Env.GetDevice();

		pDev->Clear(CB::Math::CColor(0.5f, 0.6f, 0.3f));
		pDev->BeginRender();

		this->m_pBackGround->Render(pDev);
		this->m_pLevel->Render(pDev);
		this->m_pCursor->Render(pDev);

		pDev->EndRender();
	}

	void	CGame::Update(const float32 fTD){
		this->m_pLevel->Update(fTD);
	}
}