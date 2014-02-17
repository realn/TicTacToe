#pragma once

#include <Collection_List.h>
#include <Manage_Object.h>

#include "GUIScreen.h"
#include "TextRenderer.h"

namespace T3{
	namespace GUI{
		class CMain :
			public CB::Manage::IObjectManager<CMain, CScreen>
		{
		public:
			CMain(CB::CRefPtr<CB::Graphic::IDevice> pDevice, CTextRenderer& TextRenderer);

			void	Render();
			void	Update(const float32 fTD);
		};
	}
}