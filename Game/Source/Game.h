#pragma once

#include <SmartPointers_AutoPtr.h>
#include <GraphicTools.h>

#include "GameEnv.h"
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
		CGameConfig&	m_Config;
		CGameEnv&		m_Env;
		GameState		m_State;
		CB::Math::CVector2D	m_vLevelMousePos;

		CTextureManager	m_TextureManager;
		CShaderManager	m_ShaderManager;
		CB::CAutoPtr<CGameCursor>		m_pCursor;
		CB::CAutoPtr<CLevel>			m_pLevel;

	public:
		CGame(const CB::Collection::ICountable<CB::CString>& strArgs, CGameEnv& Env, CGameConfig& Config);
		~CGame();

		const GameResult	MainLoop();

		void	EventMouseMove(const CB::Math::CVector2D& vPos);
		void	EventMouseDown(const CB::Math::CVector2D& vPos, const CB::Window::VirtualKey uKey);

		const bool	WindowClose(CB::CRefPtr<CB::Window::IWindow> pWindow);

	private:
		void	Render();
		void	Update(const float32 fTD);
	};
}