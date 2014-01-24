#pragma once

#include <WindowDriver.h>

namespace T3{
	class IDriverManager{
	public:
		virtual CB::CRefPtr<CB::Window::IManager>	CreateWindowManager() = 0;
	};
}