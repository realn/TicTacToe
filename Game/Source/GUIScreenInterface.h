#pragma once

#include "GUIControlInterface.h"

namespace T3{
	namespace GUI{
		class IScreen : 
			public CB::IRef
		{
		public:
			virtual CB::CRefPtr<CB::Graphic::IDevice>	GetDevice() const = 0;
			virtual const CB::Math::CVector2D	GetSize() const = 0;

			virtual IControlList&	GetControls() const = 0;

			virtual void	Render(IEnv& Env) = 0;
			virtual void	Update(const float32 fTD) = 0;
		};
	}
}