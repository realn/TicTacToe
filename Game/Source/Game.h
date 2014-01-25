#pragma once

#include "DriverManager.h"
#include "GameResult.h"
#include "GameConfig.h"

namespace T3{
	enum class GameState{
		MainMenu,
		Level,
		Shutdown,
	};

	class CGame{
	private:
		GameState	m_State;
		CGameConfig	m_Config;
		CB::CRefPtr<CB::Window::IManager>	m_pWindowManager;
		CB::CRefPtr<CB::Window::IWindow>	m_pMainWindow;
		CB::CRefPtr<CB::Graphic::IManager>	m_pGraphicManager;
		CB::CRefPtr<CB::Graphic::IDevice>	m_pGraphicDevice;

	public:
		CGame(const CB::Collection::ICountable<CB::CString>& strArgs, CGameConfig& Config, IDriverManager& DriverManager);
		~CGame();

		const GameResult	MainLoop();

		const bool	WindowClose(CB::CRefPtr<CB::Window::IWindow> pWindow);

	private:
		void	Render();
	};
}