#pragma once

#include <GraphicDriver.h>
#include "TextureManager.h"

namespace T3{
	class CGameCursor{
	private:
		CB::CRefPtr<CB::Graphic::IShader>		m_pVShader;
		CB::CRefPtr<CB::Graphic::IShader>		m_pFragment;
		CB::CRefPtr<CB::Graphic::IVertexDeclaration>	m_pVDecl;
		CB::CRefPtr<CB::Graphic::IBuffer>		m_pVBuffer;
		CB::CRefPtr<CB::Graphic::IBuffer>		m_pTBuffer;
		CB::CRefPtr<CB::Graphic::IBuffer>		m_pIBuffer;
		CB::CRefPtr<CB::Graphic::ITexture2D>	m_pPointer;
		CB::CRefPtr<CB::Graphic::IBlendState>	m_pBlendState;		

	public:
		CGameCursor(CB::CRefPtr<CB::Graphic::IDevice> pDevice, CTextureManager& TexMgr, const CB::Math::CSize& ScreenSize);
	};
}