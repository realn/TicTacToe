#pragma once

#include <GraphicDriver.h>

namespace T3{
	namespace GUI{
		class IEnv{
		public:
			virtual CB::CRefPtr<CB::Graphic::IDevice>	GetDevice() const = 0;
			virtual void	BeginText();
			virtual void	Print(const CB::CString& strText);
			virtual void	EndText();
		};
	}
}