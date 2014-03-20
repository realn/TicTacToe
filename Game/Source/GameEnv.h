#pragma once

#include <WindowDriver.h>
#include <GraphicDriver.h>
#include <Signals_Slots.h>

#include "DriverContainer.h"
#include "GameConfig.h"

namespace T3{
	class CGameEnv{
	private:
		const CGameConfig&	m_Config;
		CB::Math::CPoint	m_MousePos;

		CB::CRefPtr<CB::Window::IManager>	m_pWindowManager;
		CB::CRefPtr<CB::Window::IWindow>	m_pWindow;
		CB::CRefPtr<CB::Graphic::IManager>	m_pGraphicManager;
		CB::CRefPtr<CB::Graphic::IAdapter>	m_pGraphicAdapter;
		CB::CRefPtr<CB::Graphic::IOutput>	m_pGraphicOutput;
		CB::CRefPtr<CB::Graphic::IDevice>	m_pGraphicDevice;

	public:
		CGameEnv(const IDriverContainer& Drivers, const CGameConfig& Config);
		~CGameEnv();

		CB::CRefPtr<CB::Window::IWindow>	GetWindow() const;
		CB::CRefPtr<CB::Graphic::IDevice>	GetDevice() const;

		const float32	GetAspectRatio() const;

		void	Update();

		CB::Signals::CSlot<void, const CB::Math::CVector2D&>	OnMouseMove;
		CB::Signals::CSlot<void, const CB::Math::CVector2D&, const CB::Window::VirtualKey>	OnMouseDown;
		CB::Signals::CSlot<void, const CB::Math::CVector2D&, const CB::Window::VirtualKey>	OnMouseUp;
		CB::Signals::CSlot<void, const CB::Window::VirtualKey>	OnKeyDown;
		CB::Signals::CSlot<void, const CB::Window::VirtualKey>	OnKeyUp;
		CB::Signals::CSlot<void, const wchar>	OnCharPress;

		const bool	EventMouseMove(CB::CRefPtr<CB::Window::IWindow> pWindow, const CB::Math::CPoint& Position);
		const bool	EventMouseDown(CB::CRefPtr<CB::Window::IWindow> pWindow, const CB::Window::VirtualKey uKey);
		const bool	EventMouseUp(CB::CRefPtr<CB::Window::IWindow> pWindow, const CB::Window::VirtualKey uKey);
		const bool	EventKeyDown(CB::CRefPtr<CB::Window::IWindow> pWindow, const CB::Window::VirtualKey uKey);
		const bool	EventKeyUp(CB::CRefPtr<CB::Window::IWindow> pWindow, const CB::Window::VirtualKey uKey);
		const bool	EventChar(CB::CRefPtr<CB::Window::IWindow> pWindow, const wchar Char);

	private:
		const CB::Math::CVector2D	GetMousePos() const;
	};
}