#include "GameEnv.h"
#include <Signals_Method.h>

namespace T3{
	CGameEnv::CGameEnv(const IDriverContainer& Drivers, const CGameConfig& Config) : 
		m_Config(Config)
	{
		this->m_pWindowManager = Drivers.GetWindowDriver()->CreateManager();

		this->m_pWindow = this->m_pWindowManager->CreateWindow(this->m_Config.AppName, CB::Window::Style::Single, Config.Resolution, Config.WindowPosition);
		
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

		{
			using namespace CB;
			using namespace CB::Math;
			using namespace CB::Window;
			using namespace CB::Signals;

			this->m_pWindow->OnMouseMove += CMethod<CGameEnv, const bool, CRefPtr<IWindow>, const CPoint&>(this, &CGameEnv::EventMouseMove);
			this->m_pWindow->OnMouseButtonDown += CMethod<CGameEnv, const bool, CRefPtr<IWindow>, const VirtualKey>(this, &CGameEnv::EventMouseDown);
			this->m_pWindow->OnMouseButtonUp += CMethod<CGameEnv, const bool, CRefPtr<IWindow>, const VirtualKey>(this, &CGameEnv::EventMouseUp);
			this->m_pWindow->OnKeyDown += CMethod<CGameEnv, const bool, CRefPtr<IWindow>, const VirtualKey>(this, &CGameEnv::EventKeyDown);
			this->m_pWindow->OnKeyUp += CMethod<CGameEnv, const bool, CRefPtr<IWindow>, const VirtualKey>(this, &CGameEnv::EventKeyUp);
			this->m_pWindow->OnChar += CMethod<CGameEnv, const bool, CRefPtr<IWindow>, const wchar>(this, &CGameEnv::EventChar);
		}

		this->m_pWindow->SetVisible(true);
		this->m_pWindowManager->SetCursorVisible(false);
	}

	CGameEnv::~CGameEnv(){
		this->m_pWindow->OnMouseMove.Clear(this);
		this->m_pWindow->OnMouseButtonDown.Clear(this);
		this->m_pWindow->OnMouseButtonUp.Clear(this);
		this->m_pWindow->OnKeyDown.Clear(this);
		this->m_pWindow->OnKeyUp.Clear(this);
		this->m_pWindow->OnChar.Clear(this);
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

	const float32	CGameEnv::GetAspectRatio() const{
		return (float32)this->m_Config.Resolution.Width / (float32)this->m_Config.Resolution.Height;
	}

	const bool	CGameEnv::EventMouseMove(CB::CRefPtr<CB::Window::IWindow> pWindow, const CB::Math::CPoint& Position){
		this->m_MousePos = Position;

		if(this->OnMouseMove.IsValid()){
			this->OnMouseMove(this->GetMousePos());
		}

		return true;
	}

	const bool	CGameEnv::EventMouseDown(CB::CRefPtr<CB::Window::IWindow> pWindow, const CB::Window::VirtualKey uKey){
		if(this->OnMouseDown.IsValid()){
			auto vPos = CB::Math::CVector2D(this->m_MousePos) / CB::Math::CVector2D(this->m_Config.Resolution.ToPoint());
			this->OnMouseDown(this->GetMousePos(), uKey);
		}

		return true;
	}

	const bool	CGameEnv::EventMouseUp(CB::CRefPtr<CB::Window::IWindow> pWindow, const CB::Window::VirtualKey uKey){
		if(this->OnMouseUp.IsValid()){
			auto vPos = CB::Math::CVector2D(this->m_MousePos) / CB::Math::CVector2D(this->m_Config.Resolution.ToPoint());
			this->OnMouseUp(this->GetMousePos(), uKey);
		}

		return true;
	}

	const bool	CGameEnv::EventKeyDown(CB::CRefPtr<CB::Window::IWindow> pWindow, const CB::Window::VirtualKey uKey){
		if(this->OnKeyDown.IsValid()){
			this->OnKeyDown(uKey);
		}

		return true;
	}

	const bool	CGameEnv::EventKeyUp(CB::CRefPtr<CB::Window::IWindow> pWindow, const CB::Window::VirtualKey uKey){
		if(this->OnKeyUp.IsValid()){
			this->OnKeyUp(uKey);
		}

		return true;
	}

	const bool	CGameEnv::EventChar(CB::CRefPtr<CB::Window::IWindow> pWindow, const wchar Char){
		if(this->OnCharPress.IsValid()){
			this->OnCharPress(Char);
		}

		return true;
	}
	
	const CB::Math::CVector2D	CGameEnv::GetMousePos() const{
		auto vPos = CB::Math::CVector2D(this->m_MousePos) / CB::Math::CVector2D(this->m_Config.Resolution.ToPoint());
		vPos.Y = 1.0f - vPos.Y;
		return vPos;
	}
}