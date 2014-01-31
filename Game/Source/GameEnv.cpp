#include "GameEnv.h"
#include <Signals_Method.h>

namespace T3{
	CGameEnv::CGameEnv(const IDriverContainer& Drivers, const CGameConfig& Config){
		this->m_pWindowManager = Drivers.GetWindowDriver()->CreateManager();

		this->m_pWindow = this->m_pWindowManager->CreateWindow(L"TicTacToe", CB::Window::Style::Single, Config.Resolution, Config.WindowPosition);
		
		this->m_pGraphicManager = Drivers.GetGraphicDriver()->CreateManager(this->m_pWindowManager);

		this->m_pGraphicAdapter = this->m_pGraphicManager->GetDefaultAdapter();

		CB::Graphic::CDeviceDesc desc(
			this->m_pWindow, 
			CB::Graphic::CDisplayMode(Config.Resolution, 60, CB::Graphic::BufferFormat::R8G8B8A8),
			CB::Graphic::BufferFormat::D24S8,
			false
			);

		CB::Collection::CArray<CB::Graphic::FeatureLevel, 1> level;
		level[0] = CB::Graphic::FeatureLevel::Level_1;

		this->m_pGraphicDevice = this->m_pGraphicManager->GetDefaultAdapter()->CreateDevice(this->m_pWindow, desc, level);

		this->m_pWindow->OnMouseMove += CB::Signals::CMethod<CGameEnv, const bool, CB::CRefPtr<CB::Window::IWindow>, const CB::Math::CPoint&>(this, &CGameEnv::EventMouseMove);
		this->m_pWindow->OnMouseButtonDown += CB::Signals::CMethod<CGameEnv, const bool, CB::CRefPtr<CB::Window::IWindow>, const CB::Window::VirtualKey>(this, &CGameEnv::EventMouseDown);
		this->m_pWindow->OnMouseButtonUp += CB::Signals::CMethod<CGameEnv, const bool, CB::CRefPtr<CB::Window::IWindow>, const CB::Window::VirtualKey>(this, &CGameEnv::EventMouseUp);

		this->m_pWindow->SetVisible(true);
		this->m_pWindowManager->SetCursorVisible(false);
	}

	CGameEnv::~CGameEnv(){
		this->m_pWindow->OnMouseMove.Clear(this);
		this->m_pWindow->OnMouseButtonDown.Clear(this);
		this->m_pWindow->OnMouseButtonUp.Clear(this);
	}

	CB::CRefPtr<CB::Window::IWindow>	CGameEnv::GetWindow() const{
		return this->m_pWindow;
	}

	CB::CRefPtr<CB::Graphic::IDevice>	CGameEnv::GetDevice() const{
		return this->m_pGraphicDevice;
	}

	void	CGameEnv::Update(){
		this->m_pWindowManager->ProcessEvents();
	}

	const bool	CGameEnv::EventMouseMove(CB::CRefPtr<CB::Window::IWindow> pWindow, const CB::Math::CPoint& Position){
		this->m_MousePos = Position;

		if(this->OnMouseMove.IsValid()){
			auto vPos = CB::Math::CVector2D(Position) / CB::Math::CVector2D(this->m_Config.Resolution.ToPoint());
			this->OnMouseMove(vPos);
		}

		return true;
	}

	const bool	CGameEnv::EventMouseDown(CB::CRefPtr<CB::Window::IWindow> pWindow, const CB::Window::VirtualKey uKey){
		if(this->OnMouseDown.IsValid()){
			auto vPos = CB::Math::CVector2D(this->m_MousePos) / CB::Math::CVector2D(this->m_Config.Resolution.ToPoint());
			this->OnMouseDown(vPos, uKey);
		}

		return true;
	}

	const bool	CGameEnv::EventMouseUp(CB::CRefPtr<CB::Window::IWindow> pWindow, const CB::Window::VirtualKey uKey){
		if(this->OnMouseUp.IsValid()){
			auto vPos = CB::Math::CVector2D(this->m_MousePos) / CB::Math::CVector2D(this->m_Config.Resolution.ToPoint());
			this->OnMouseUp(vPos, uKey);
		}

		return true;
	}
}