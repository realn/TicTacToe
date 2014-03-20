#include "Game.h"
#include <Timer.h>
#include <Signals_Method.h>
#include <IO_Path.h>
#include <IO_File.h>
#include <IO_TextReader.h>

#include "GUIScreen.h"
#include "GUIMenuScreen.h"
#include "GUITextItem.h"
#include "GUITextButton.h"

#include "MenuMain.h"
#include "MenuPause.h"

namespace T3{
	const float32	GUI_SCREEN_H = 10.0f;

	CGame::CGame(const CB::Collection::ICountable<CB::CString>& strArgs, CGameEnv& Env, CGameConfig& Config) :
		m_State(GameState::MainMenu),
		m_Env(Env),
		m_Config(Config),
		m_TextureManager(Env.GetDevice(), Config.AssetsDir),
		m_ShaderManager(Env.GetDevice(), Config.AssetsDir, CB::Graphic::ShaderVersion::ShaderModel_2),
		m_uField(CLevel::FieldType::Cross),
		m_Text(Env.GetDevice(), m_ShaderManager, m_Config.AssetsDir),
		m_GUIMain(Env.GetDevice(), m_Text, m_ShaderManager)
	{
		{
			auto pWindow = this->m_Env.GetWindow();
			pWindow->OnClose += CB::Signals::CMethod<CGame, const bool, CB::CRefPtr<CB::Window::IWindow>>(this, &CGame::WindowClose);
		}

		this->m_Env.OnMouseMove += CB::Signals::CMethod<CGame, void, const CB::Math::CVector2D&>(this, &CGame::EventMouseMove);
		this->m_Env.OnMouseDown += CB::Signals::CMethod<CGame, void, const CB::Math::CVector2D&, const CB::Window::VirtualKey>(this, &CGame::EventMouseDown);
		this->m_Env.OnKeyDown += CB::Signals::CMethod<CGame, void, const CB::Window::VirtualKey>(this, &CGame::EventKeyDown);
		
		this->m_pCursor = new CGameCursor(this->m_Env.GetDevice(), this->m_Config.Resolution, this->m_TextureManager, this->m_ShaderManager);

		this->m_pLevel = new CLevel(this->m_Env.GetDevice(), this->m_TextureManager, this->m_ShaderManager); 
		this->m_pLevel->SetSceneSize(CB::Math::CVector2D(1.0f * this->m_Env.GetAspectRatio(), 1.0f));
		this->m_pLevel->SetGridPos(CB::Math::CVector3D((this->m_Env.GetAspectRatio() - 1.0f)/2.0f, 0.0f));

		this->m_pBackGround = new CBGGridModel(this->m_Env.GetDevice(), this->m_TextureManager, this->m_ShaderManager);
		this->m_pBackGround->SetScreenSize(CB::Math::CVector2D(1.0f * this->m_Env.GetAspectRatio(), 1.0f));
		this->m_pBackGround->SetGridPos(CB::Math::CVector3D((this->m_Env.GetAspectRatio() - 1.0f)/2.0f, 0.0f));

		CB::Math::CVector2D guiSize(this->m_Env.GetAspectRatio() * GUI_SCREEN_H, GUI_SCREEN_H);
		CB::CRefPtr<Menu::CMain> pScreen = new Menu::CMain(this->m_GUIMain, guiSize);
		pScreen->OnNewGame += CB::Signals::CMethod<CGame>(this, &CGame::MenuNewGame);
		pScreen->OnExit += CB::Signals::CMethod<CGame>(this, &CGame::MenuExit);
		this->m_GUIMain.PushScreen(pScreen.Cast<GUI::CScreen>(), true);
	}

	CGame::~CGame(){
		auto pWindow = this->m_Env.GetWindow();
		pWindow->OnClose.Clear(this);
		m_Env.OnMouseDown.Clear(this);
		m_Env.OnMouseMove.Clear(this);
	}

	void	CGame::MenuNewGame(){
		this->m_State = GameState::Level;
		this->m_GUIMain.PopScreen(true);
	}

	void	CGame::MenuExit(){
		this->m_State = GameState::Exit;
		this->m_GUIMain.PopScreen(true);
	}

	void	CGame::MenuReturn(){
		this->m_State = GameState::ReturnToLevel;
		this->m_GUIMain.PopScreen(true);
	}

	void	CGame::MenuToMain(){
		this->m_State = GameState::ReturnToMain;
		this->m_GUIMain.PopScreen(true);
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
		this->m_GUIMain.SetCursorPos(Position);
		
		GUI::CEvent event;
		event.Type = GUI::EventType::MouseMove;
		event.Position = Position;
		this->m_GUIMain.ProcessEvent(event);
	}

	void	CGame::EventMouseDown(const CB::Math::CVector2D& Position, const CB::Window::VirtualKey Button){
		if(Button == CB::Window::VirtualKey::LBUTTON){
			if(this->m_State == GameState::Level){
				this->m_pLevel->PutField(Position, m_uField);
				this->m_uField = this->m_uField == CLevel::FieldType::Cross ? CLevel::FieldType::Circle : CLevel::FieldType::Cross;
			}

			GUI::CEvent event;
			event.Type = GUI::EventType::MouseClick;
			event.Key = Button;
			this->m_GUIMain.ProcessEvent(event);
		}
	}

	void	CGame::EventKeyDown(const CB::Window::VirtualKey uKey){
		if(this->m_State == GameState::Level){
			switch (uKey)
			{
			case CB::Window::VirtualKey::ESCAPE:
				{
					CB::CRefPtr<Menu::CPause> pScreen = new Menu::CPause(this->m_GUIMain, GetGUISize());
					pScreen->OnReturn += CB::Signals::CMethod<CGame>(this, &CGame::MenuReturn);
					pScreen->OnMainMenu += CB::Signals::CMethod<CGame>(this, &CGame::MenuToMain);
					pScreen->OnNewGame += CB::Signals::CMethod<CGame>(this, &CGame::MenuNewGame);
					pScreen->OnExit += CB::Signals::CMethod<CGame>(this, &CGame::MenuExit);
					this->m_GUIMain.PushScreen( pScreen.Cast<GUI::CScreen>(), true );
					this->m_State = GameState::PauseMenu;
				}
				break;

			default:
				break;
			}
		}
		else{
			GUI::CEvent event;
			event.Type = GUI::EventType::KeyPress;
			event.Key = uKey;
			this->m_GUIMain.ProcessEvent(event);
		}
	}

	void	CGame::Render(){
		auto pDev = this->m_Env.GetDevice();

		pDev->Clear(CB::Math::CColor(0.5f, 0.6f, 0.3f));
		pDev->BeginRender();

		switch (this->m_State)
		{
		case GameState::Level:
			if(!this->m_GUIMain.IsInTransition()){
				this->m_pBackGround->Render(pDev);
				this->m_pLevel->Render(pDev);
			}
			break;

		case GameState::ReturnToMain:
		case GameState::ReturnToLevel:
			this->m_pBackGround->Render(pDev);
			this->m_pLevel->Render(pDev);
			break;

		case GameState::PauseMenu:
			this->m_pBackGround->Render(pDev);
			this->m_pLevel->Render(pDev);
			break;

		default:
			break;
		}

		this->m_GUIMain.Render();
		this->m_pCursor->Render(pDev);

		pDev->EndRender();
	}

	void	CGame::Update(const float32 fTD){
		this->m_GUIMain.Update(fTD);
		switch (this->m_State)
		{
		case GameState::MainMenu:
			break;

		case GameState::Level:
			if(!this->m_GUIMain.IsInTransition())
				this->m_pLevel->Update(fTD);
			break;

		case GameState::PauseMenu:
			break;

		case GameState::ReturnToLevel:
			if(!this->m_GUIMain.IsInTransition())
				this->m_State = GameState::Level;
			break;

		case GameState::ReturnToMain:
			if(!this->m_GUIMain.IsInTransition()){
				this->m_State = GameState::MainMenu;
				CB::CRefPtr<Menu::CMain> pScreen = new Menu::CMain(this->m_GUIMain, GetGUISize());
				pScreen->OnNewGame += CB::Signals::CMethod<CGame>(this, &CGame::MenuNewGame);
				pScreen->OnExit += CB::Signals::CMethod<CGame>(this, &CGame::MenuExit);
				this->m_GUIMain.PushScreen(pScreen.Cast<GUI::CScreen>(), true);
			}
			break;

		case GameState::Exit:
			if(!this->m_GUIMain.IsInTransition())
				this->m_State = GameState::Shutdown;
			break;

		default:
			break;
		}
	}

	const CB::Math::CVector2D	CGame::GetGUISize() const{
		return 	CB::Math::CVector2D(this->m_Env.GetAspectRatio() * GUI_SCREEN_H, GUI_SCREEN_H);
	}
}