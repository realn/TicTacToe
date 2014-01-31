#pragma once

#include <WindowDriver.h>
#include <GraphicDriver.h>

namespace T3{
	class IDriverContainer{
	public:
		virtual CB::CRefPtr<CB::Window::IDriver>	GetWindowDriver() const = 0;
		virtual CB::CRefPtr<CB::Graphic::IDriver>	GetGraphicDriver() const = 0;
	};
}