#pragma once

#include <GraphicDriver.h>
#include <Math_RectangleF.h>
#include "TextureManager.h"
#include "ShaderManager.h"

namespace T3{
	class CLevel{
	private:
		enum class FieldType{
			None = 0,
			Cross = 1,
			Circle = 2,
		};

		class CField{
		public:
			uint32		uIndex;
			bool		bHover;
			FieldType	Type;
			CB::Math::CRectangleF32	Rect;
		};

		CB::Math::CMatrix	m_mModel;
		CB::Math::CVector2D	m_vSize;
		CB::Math::CVector2D m_vFieldSize;
		CB::Collection::CArray<CField, 9>	m_Fields;

		CB::CRefPtr<CB::Graphic::IShader>	m_pVertexShader;
		CB::CRefPtr<CB::Graphic::IShader>	m_pFragmentShader;

		CB::CRefPtr<CB::Graphic::IVertexDeclaration>	m_pDeclaration;
		CB::CRefPtr<CB::Graphic::IBuffer>	m_pVertexBuffer;
		CB::CRefPtr<CB::Graphic::IBuffer>	m_pTCoordBuffer;
		CB::CRefPtr<CB::Graphic::IBuffer>	m_pIndexBuffer;

		CB::CRefPtr<CB::Graphic::ITexture2D>	m_pCircle;
		CB::CRefPtr<CB::Graphic::ITexture2D>	m_pCross;
		CB::CRefPtr<CB::Graphic::ITexture2D>	m_pFrame;

	public:
		CLevel(CB::CRefPtr<CB::Graphic::IDevice> pDevice, const CB::Math::CVector2D& vSize, CTextureManager& TexMng, CShaderManager& ShdMng);

		void	Update(const float32 fTD);
		void	Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice);

		void	SetModelMatrix(const CB::Math::CMatrix& mModel);
		void	SetMousePos(const CB::Math::CVector2D& vPosition);
	}
}