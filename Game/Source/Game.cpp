#include "Game.h"
#include <Timer.h>
#include <Signals_Method.h>
#include "TextureManager.h"
#include <IO_Path.h>
#include <IO_File.h>
#include <IO_TextReader.h>

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
		this->m_pMainWindow->OnMouseMove += CB::Signals::CMethod<CGame, const bool, CB::CRefPtr<CB::Window::IWindow>, const CB::Math::CPoint&>(this, &CGame::WindowMouseMove);
		this->m_pMainWindow->OnMouseButtonDown += CB::Signals::CMethod<CGame, const bool, CB::CRefPtr<CB::Window::IWindow>, const CB::Window::VirtualKey>(this, &CGame::WindowMouseDown);

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

		this->m_pTextureManager = new CTextureManager(m_pGraphicDevice, this->m_Config.AssetsDir);
		this->m_pShaderManager = new CShaderManager(this->m_pGraphicDevice, this->m_Config.AssetsDir, CB::Graphic::ShaderVersion::ShaderModel_2);
		
		this->m_pCursor = new CGameCursor(this->m_pGraphicDevice, this->m_pMainWindow->GetSize(), *this->m_pTextureManager, *this->m_pShaderManager);

		this->m_pLevel = new CLevel(this->m_pGraphicDevice, CB::Math::CVector2D(4.0f, 4.0f), *this->m_pTextureManager, *this->m_pShaderManager); 

		this->m_pMainWindow->SetVisible(true);
		this->m_pWindowManager->SetCursorVisible(false);
	}

	CGame::~CGame(){
		this->m_pMainWindow->OnClose.Clear(this);
		this->m_pMainWindow->OnMouseMove.Clear(this);
		this->m_pMainWindow->OnMouseButtonDown.Clear(this);
		//this->m_pMainWindow->OnClose -= CB::Signals::CMethod<CGame, const bool, CB::CRefPtr<CB::Window::IWindow>>(this, &CGame::WindowClose);
		//this->m_pMainWindow->OnMouseMove -= CB::Signals::CMethod<CGame, const bool, CB::CRefPtr<CB::Window::IWindow>, const CB::Math::CPoint&>(this, &CGame::WindowMouseMove);
		//this->m_pMainWindow->OnMouseButtonDown -= CB::Signals::CMethod<CGame, const bool, CB::CRefPtr<CB::Window::IWindow>, const CB::Window::VirtualKey>(this, &CGame::WindowMouseDown);
	}

	const GameResult	CGame::MainLoop(){
		CB::CTimer	timer;

		do{
			this->Render();

			timer.Update();
			this->m_pWindowManager->ProcessEvents();

			this->Update(timer.GetTimeDelta());

			this->m_pGraphicDevice->Swap();
		}
		while(this->m_State != GameState::Shutdown);
		
		return GameResult::Shutdown;
	}

	const bool	CGame::WindowClose(CB::CRefPtr<CB::Window::IWindow> pWindow){
		this->m_State = GameState::Shutdown;

		return true;
	}

	const bool	CGame::WindowMouseMove(CB::CRefPtr<CB::Window::IWindow> pWindow, const CB::Math::CPoint& Position){
		this->m_pCursor->SetPos(Position);
		
		auto posNorm = CB::Math::CVector2D(Position) / CB::Math::CVector2D(this->m_pMainWindow->GetSize().ToPoint());
		posNorm.Y = 1.0f - posNorm.Y;
		auto posLevel = posNorm * CB::Math::CVector2D(6.4f, 4.8f);

		this->m_pLevel->SetMousePos(posLevel);
		this->m_vLevelMousePos = posLevel;

		return true;
	}

	const bool	CGame::WindowMouseDown(CB::CRefPtr<CB::Window::IWindow> pWindow, const CB::Window::VirtualKey Button){
		if(Button == CB::Window::VirtualKey::LBUTTON){
			this->m_pLevel->PutField(this->m_vLevelMousePos, CLevel::FieldType::Cross);
		}
		return true;
	}

	void	CGame::Render(){
		this->m_pGraphicDevice->Clear(CB::Math::CColor(0.5f, 0.6f, 0.3f));
		this->m_pGraphicDevice->BeginRender();

		this->m_pLevel->Render(this->m_pGraphicDevice);
		this->m_pCursor->Render(this->m_pGraphicDevice);

		this->m_pGraphicDevice->EndRender();
	}

	void	CGame::Update(const float32 fTD){
		this->m_pLevel->Update(fTD);
	}
}