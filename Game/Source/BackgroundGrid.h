#pragma once

#include "RectMesh.h"
#include "TextureManager.h"
#include "ShaderManager.h"

namespace T3{
	class CBGGridModel :
		public CRectMesh
	{
	private:
		CB::CRefPtr<CB::Graphic::IShader>		m_pVertexShader;
		CB::CRefPtr<CB::Graphic::IShader>		m_pFragmentShader;
		CB::CRefPtr<CB::Graphic::IBaseTexture>	m_pTexture;
		CB::Math::CVector2D	m_vScreenSize;
		CB::Math::CVector2D	m_vGridPos;
		bool	m_bMultiRender;

	public:
		CBGGridModel(CB::CRefPtr<CB::Graphic::IDevice> pDevice, CTextureManager& TexMng, CShaderManager& ShdMng);

		void	SetScreenSize(const CB::Math::CVector2D& vSize);
		void	SetGridPos(const CB::Math::CVector2D& vPosition);

		void	BeginRender(CB::CRefPtr<CB::Graphic::IDevice> pDevice);
		void	Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice);
		void	EndRender(CB::CRefPtr<CB::Graphic::IDevice> pDevice);
	};
}