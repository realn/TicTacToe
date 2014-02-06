#pragma once

#include <Types.h>
#include <Ref.h>
#include <SmartPointers_RefPtr.h>
#include <Collection_Interface.h>
#include <GraphicDriver.h>

#include "GUIEnvInterface.h"

namespace T3{
	namespace GUI{
		class IControl;
		typedef CB::Collection::ICountable<CB::CRefPtr<IControl>> IControlList;

		class IControl : 
			public CB::IRef
		{
		public:
			virtual const CB::CString&	GetID() const = 0;
			virtual IControlList&		GetControls() const = 0;

			virtual void	Update(const float32 fTD) = 0;
			virtual void	Render(const IEnv& Env) = 0;
		};
	}
}