#pragma once

#include <GraphicDriver.h>

namespace T3{
	namespace GUI{
		class IEnv{
		public:
			virtual CB::CRefPtr<CB::Graphic::IDevice>	GetDevice() const = 0;
		};
	}
}