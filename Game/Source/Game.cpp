#include "Game.h"
#include <Timer.h>
#include <Signals_Method.h>

namespace T3{
	CGame::CGame(const CB::Collection::ICountable<CB::CString>& strArgs, CGameConfig& Config, IDriverManager& driverManager) :
		m_State(GameState::MainMenu),
		m_Config(Config)
	{
		uint32 uIndex = 0;
		if(CB::Collection::TryFind(strArgs, CB::CString(L"-assets"), uIndex) && uIndex + 1 < strArgs.GetLength()){
			this->m_Config.AssetsDir = strArgs[uIndex + 1];
		}

		this->m_pWindowManager = driverManager.CreateWindowManager();
		this->m_pMainWindow = this->m_pWindowManager->CreateWindow(L"TicTacToe", CB::Window::Style::Single, this->m_Config.Resolution, this->m_Config.WindowPosition);
		this->m_pMainWindow->OnClose += CB::Signals::CMethod<CGame, const bool, CB::CRefPtr<CB::Window::IWindow>>(this, &CGame::WindowClose);

		this->m_pGraphicManager = driverManager.CreateGraphicManager(this->m_pWindowManager);

		CB::Graphic::CDeviceDesc desc(
			this->m_pMainWindow, 
			CB::Graphic::CDisplayMode(this->m_Config.Resolution, 60, CB::Graphic::BufferFormat::R8G8B8A8),
			CB::Graphic::BufferFormat::D24S8,
			false
			);

		CB::Collection::CArray<CB::Graphic::FeatureLevel, 1> level;
		level[0] = CB::Graphic::FeatureLevel::Level_1;

		this->m_pGraphicDevice = this->m_pGraphicManager->GetDefaultAdapter()->CreateDevice(
			this->m_pMainWindow,
			desc,
			level
			);

		this->m_pMainWindow->SetVisible(true);
	}

	CGame::~CGame(){
		this->m_pMainWindow->OnClose -= CB::Signals::CMethod<CGame, const bool, CB::CRefPtr<CB::Window::IWindow>>(this, &CGame::WindowClose);
	}

	const GameResult	CGame::MainLoop(){
		CB::CTimer	timer;

		do{
			this->Render();

			timer.Update();
			this->m_pWindowManager->ProcessEvents();

			this->m_pGraphicDevice->Swap();
		}
		while(this->m_State != GameState::Shutdown);
		
		return GameResult::Shutdown;
	}

	const bool	CGame::WindowClose(CB::CRefPtr<CB::Window::IWindow> pWindow){
		this->m_State = GameState::Shutdown;

		return true;
	}

	void	CGame::Render(){
		this->m_pGraphicDevice->Clear(CB::Math::CColor(0.5f, 0.6f, 0.3f));
		this->m_pGraphicDevice->BeginRender();

		this->m_pGraphicDevice->EndRender();
	}
}