#include "Game.h"
#include <Timer.h>
#include <Signals_Method.h>

namespace T3{
	CGame::CGame(const CB::CString& strCmdLine, IDriverManager& driverManager) :
		m_State(GameState::MainMenu)
	{
		this->m_pWindowManager = driverManager.CreateWindowManager();
		this->m_pMainWindow = this->m_pWindowManager->CreateWindow(L"TicTacToe", CB::Window::Style::Single);
		this->m_pMainWindow->SetVisible(true);

		this->m_pMainWindow->OnClose += CB::Signals::CMethod<CGame, const bool, CB::CRefPtr<CB::Window::IWindow>>(this, &CGame::WindowClose);
	}

	CGame::~CGame(){
		this->m_pMainWindow->OnClose -= CB::Signals::CMethod<CGame, const bool, CB::CRefPtr<CB::Window::IWindow>>(this, &CGame::WindowClose);
	}

	const GameResult	CGame::MainLoop(){
		CB::CTimer	timer;

		do{
			timer.Update();
			this->m_pWindowManager->ProcessEvents();
		}
		while(this->m_State != GameState::Shutdown);
		
		return GameResult::Shutdown;
	}

	const bool	CGame::WindowClose(CB::CRefPtr<CB::Window::IWindow> pWindow){
		this->m_State = GameState::Shutdown;

		return true;
	}
}