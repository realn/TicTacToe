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
#include "BackgroundGrid.h"
#include "GUIMain.h"

#include "TextRenderer.h"

namespace T3{
	enum class GameState{
		MainMenu,
		Level,
		PauseMenu,
		ReturnToLevel,
		ReturnToMain,
		Exit,
		Shutdown,
	};

	class CGame{
	private:
		CGameConfig&	m_Config;
		CGameEnv&		m_Env;
		GameState		m_State;

		CTextureManager	m_TextureManager;
		CShaderManager	m_ShaderManager;
		GUI::CMain		m_GUIMain;
		CB::CAutoPtr<CGameCursor>		m_pCursor;
		CB::CAutoPtr<CLevel>			m_pLevel;
		CB::CAutoPtr<CBGGridModel>		m_pBackGround;

		CLevel::FieldType	m_uField;

		CTextRenderer	m_Text;

	public:
		CGame(const CB::Collection::ICountable<CB::CString>& strArgs, CGameEnv& Env, CGameConfig& Config);
		~CGame();

		const GameResult	MainLoop();

		void	EventMouseMove(const CB::Math::CVector2D& vPos);
		void	EventMouseDown(const CB::Math::CVector2D& vPos, const CB::Window::VirtualKey uKey);
		void	EventKeyDown(const CB::Window::VirtualKey uKey);

		const bool	WindowClose(CB::CRefPtr<CB::Window::IWindow> pWindow);

		void	MenuNewGame();
		void	MenuExit();
		void	MenuReturn();
		void	MenuToMain();

	private:
		void	Render();
		void	Update(const float32 fTD);

		const CB::Math::CVector2D	GetGUISize() const;
	};
}