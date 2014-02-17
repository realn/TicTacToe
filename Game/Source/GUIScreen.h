#pragma once

#include <Ref.h>
#include <SmartPointers_RefPtr.h>
#include <Manage_Object.h>

#include "GUIItem.h"

namespace T3{
	namespace GUI{
		class CMain;

		class CScreen : 
			public CB::IRef,
			public CB::Manage::IObjectManager<CScreen, IItem>,
			public CB::Manage::IManagedObject<CMain, CScreen>
		{
		public:
			CScreen(CMain& Main);

			virtual void	Render();
			virtual void	Update(const float32 fTD);
		};
	}
}