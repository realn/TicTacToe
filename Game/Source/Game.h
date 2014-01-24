#pragma once

#include "DriverManager.h"
#include "GameResult.h"

namespace T3{
	enum class GameState{
		MainMenu,
		Level,
		Shutdown,
	};

	class CGame{
	private:
		GameState	m_State;
		CB::CRefPtr<CB::Window::IManager>	m_pWindowManager;
		CB::CRefPtr<CB::Window::IWindow>	m_pMainWindow;
		CB::CRefPtr<CB::Graphic::IManager>	m_pGraphicManager;
		CB::CRefPtr<CB::Graphic::IDevice>	m_pGraphicDevice;

	public:
		CGame(const CB::CString& strCmdLine, IDriverManager& DriverManager);
		~CGame();

		const GameResult	MainLoop();

		const bool	WindowClose(CB::CRefPtr<CB::Window::IWindow> pWindow);

	private:
		void	Render();
	};
}