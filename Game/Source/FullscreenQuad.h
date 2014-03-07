#pragma once

#include <GraphicDriver.h>
#include "ShaderManager.h"

namespace T3{
	class CFullscreenQuad{
	private:
		CB::CRefPtr<CB::Graphic::IDevice>	m_pDevice;
		CB::CRefPtr<CB::Graphic::IShader>	m_pVShader;
		CB::CRefPtr<CB::Graphic::IShader>	m_pFShader;
		CB::CRefPtr<CB::Graphic::IVertexDeclaration>	m_pVDecl;
		CB::CRefPtr<CB::Graphic::IBuffer>	m_pVBuffer;
		CB::CRefPtr<CB::Graphic::IBuffer>	m_pIBuffer;
		CB::CRefPtr<CB::Graphic::IDeviceState>	m_pBlendState;

	public:
		CFullscreenQuad(CB::CRefPtr<CB::Graphic::IDevice> pDevice, CShaderManager& ShaderManager);
		
		void	Render(const CB::Math::CColor& Color);
	};
}