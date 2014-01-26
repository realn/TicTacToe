#pragma once

#include <SmartPointers_AutoPtr.h>
#include <GraphicTools.h>

#include "DriverManager.h"
#include "GameResult.h"
#include "GameConfig.h"
#include "TextureManager.h"

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

		CB::CRefPtr<CB::Graphic::IShader>		m_pVShader;
		CB::CRefPtr<CB::Graphic::IShader>		m_pFragment;
		CB::CRefPtr<CB::Graphic::IVertexDeclaration>	m_pVDecl;
		CB::CRefPtr<CB::Graphic::IBuffer>		m_pVBuffer;
		CB::CRefPtr<CB::Graphic::IBuffer>		m_pTBuffer;
		CB::CRefPtr<CB::Graphic::IBuffer>		m_pIBuffer;
		CB::CRefPtr<CB::Graphic::ITexture2D>	m_pPointer;

	public:
		CGame(const CB::Collection::ICountable<CB::CString>& strArgs, CGameConfig& Config, IDriverManager& DriverManager);
		~CGame();

		const GameResult	MainLoop();

		const bool	WindowClose(CB::CRefPtr<CB::Window::IWindow> pWindow);

	private:
		void	Render();
	};
}