#pragma once

#include <GraphicDriver.h>
#include "TextureManager.h"
#include "ShaderManager.h"

namespace T3{
	class CGameCursor{
	private:
		static const CB::CString	CURSOR_TEXTURE;
		static const CB::CString	CURSOR_SHADER;

		CB::CRefPtr<CB::Graphic::IVertexDeclaration>	m_pVDecl;
		
		CB::CRefPtr<CB::Graphic::IShader>		m_pVShader;
		CB::CRefPtr<CB::Graphic::IShader>		m_pFragment;

		CB::CRefPtr<CB::Graphic::IBuffer>		m_pVBuffer;
		CB::CRefPtr<CB::Graphic::IBuffer>		m_pIBuffer;

		CB::CRefPtr<CB::Graphic::ITexture2D>	m_pPointer;
		CB::CRefPtr<CB::Graphic::IBlendState>	m_pBlendState;		

		CB::Math::CVector2D	m_vCursorPos;
		CB::Math::CSize		m_SreenSize;
		CB::Math::CSize		m_CursorSize;

	public:
		CGameCursor(CB::CRefPtr<CB::Graphic::IDevice> pDevice, const CB::Math::CSize& ScreenSize, CTextureManager& TexMgr, CShaderManager& ShaderMng);

		void	SetPos(const CB::Math::CVector2D& Position);

		void	Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice);
	};
}