#pragma once

#include <WindowDriver.h>
#include <GraphicDriver.h>

namespace T3{
	class IDriverManager{
	public:
		virtual CB::CRefPtr<CB::Window::IManager>	CreateWindowManager() = 0;
		virtual CB::CRefPtr<CB::Graphic::IManager>	CreateGraphicManager(CB::CRefPtr<CB::Window::IManager> pWindowManager) = 0;
	};
}