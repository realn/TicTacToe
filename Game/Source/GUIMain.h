#pragma once

#include <Collection_List.h>

#include "GUIScreen.h"
#include "TextRenderer.h"
#include "Manage_ItemContainer.h"
#include "ShaderManager.h"
#include "GUIEvent.h"
#include "FullscreenQuad.h"

namespace T3{
	namespace GUI{
		class CMain :
			public Manage::IContItemContainer<CMain, CScreen>
		{
		private:
			CB::CRefPtr<CB::Graphic::IDevice>	m_pDevice;
			CTextRenderer&	m_TextRender;
			CFullscreenQuad	m_Background;
			
			CB::Math::CVector2D	m_vCursorPos;

			CB::CRefPtr<GUI::CScreen>	m_pTransitScreen;
			TransitionMode				m_uTransitMode;

			CB::CRefPtr<CB::Graphic::IVertexDeclaration>	m_pVertexDeclaration;
			CB::CRefPtr<CB::Graphic::IShader>				m_pVertexShader;
			CB::CRefPtr<CB::Graphic::IShader>				m_pFragmentShader;
			CB::CRefPtr<CB::Graphic::IDeviceState>			m_pBState;

		public:
			CMain(CB::CRefPtr<CB::Graphic::IDevice> pDevice, CTextRenderer& TextRenderer, CShaderManager& ShdMng);
			virtual ~CMain();

			void	PushScreen(CB::CRefPtr<CScreen> pScreen, const bool bTransition);
			void	PopScreen(const bool bTransition);

			void	Render();
			void	Update(const float32 fTD);

			CB::CRefPtr<CB::Graphic::IDevice>	GetDevice();
			CTextRenderer&		GetTextRenderer();
			CFullscreenQuad&	GetBackground();

			void	SetUpRender();
			void	FreeRender();

			void	SetColor(const CB::Math::CColor& Color);
			void	SetProjection(const CB::Math::CMatrix& Matrix);
			void	SetCursorPos(const CB::Math::CVector2D& vPosition);

			const CB::Math::CVector2D&	GetCursorPos() const;

			void	ProcessEvent(const CEvent& Event);
		};
	}
}