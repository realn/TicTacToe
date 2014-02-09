#pragma once

#include <Types.h>
#include <CBString.h>

#include "ShaderManager.h"
#include "GUIScreenInterface.h"

namespace T3{
	namespace GUI{
		class CManager
		{
		private:
			CB::CRefPtr<CB::Graphic::IShader>		m_pVShader;
			CB::CRefPtr<CB::Graphic::IShader>		m_pFShader;
			CB::CRefPtr<CB::Graphic::IDeviceState>	m_pBlendState;
			CB::CRefPtr<CB::Graphic::IVertexDeclaration>	m_pVertexDeclaration;

			CB::Collection::CList<CB::CRefPtr<IScreen>>	m_Screens;

		public:
			CManager(CB::CRefPtr<CB::Graphic::IDevice> pDevice, CShaderManager& ShdMng);

			void PushScreen(CB::CRefPtr<IScreen> pScreen);
			void PopScreen();

			void	Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice);
			void	Update(const float32 fTD);
		};
	}
}