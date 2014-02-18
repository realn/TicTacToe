#pragma once

#include <Collection_List.h>

#include "GUIScreen.h"
#include "TextRenderer.h"
#include "Manage_ItemContainer.h"

namespace T3{
	namespace GUI{
		class CMain :
			public CB::IRef,
			private Manage::IItemContainer<CScreen>
		{
		private:
			CB::CRefPtr<CB::Graphic::IDevice>	m_pDevice;
			CTextRenderer&	m_TextRender;

		public:
			CMain(CB::CRefPtr<CB::Graphic::IDevice> pDevice, CTextRenderer& TextRenderer);

			void	PushScreen(CB::CRefPtr<CScreen> pScreen);
			void	PopScreen();

			void	Render();
			void	Update(const float32 fTD);

			CB::CRefPtr<CB::Graphic::IDevice>	GetDevice();
			CTextRenderer&	GetTextRenderer();

			void	SetUpRender();
			void	FreeRender();
		};
	}
}