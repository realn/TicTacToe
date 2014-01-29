#pragma once

#include <SmartPointers_AutoPtr.h>
#include <GraphicTools.h>

#include "DriverManager.h"
#include "GameResult.h"
#include "GameConfig.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "GameCursor.h"
#include "Level.h"

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

		CB::CAutoPtr<CTextureManager>	m_pTextureManager;
		CB::CAutoPtr<CShaderManager>	m_pShaderManager;
		CB::CAutoPtr<CGameCursor>		m_pCursor;
		CB::CAutoPtr<CLevel>			m_pLevel;

	public:
		CGame(const CB::Collection::ICountable<CB::CString>& strArgs, CGameConfig& Config, IDriverManager& DriverManager);
		~CGame();

		const GameResult	MainLoop();

		const bool	WindowClose(CB::CRefPtr<CB::Window::IWindow> pWindow);
		const bool	WindowMouseMove(CB::CRefPtr<CB::Window::IWindow> pWindow, const CB::Math::CPoint& Position);

	private:
		void	Render();
		void	Update(const float32 fTD);
	};
}