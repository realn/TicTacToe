#pragma once

#include <GraphicDriver.h>
#include <Math_RectangleF.h>
#include "LevelMesh.h"
#include "TextureManager.h"
#include "ShaderManager.h"

namespace T3{
	class CLevel{
	public:
		enum class FieldType{
			None = 0,
			Cross = 1,
			Circle = 2,
		};

	private:
		class CField{
		public:
			uint32		uIndex;
			bool		bHover;
			FieldType	Type;
			float32		Alpha;
			CB::Math::CRectangleF32	Rect;
		};

		CB::Math::CMatrix	m_mModel;
		CB::Math::CVector2D	m_vSceneSize;
		CB::Math::CVector2D	m_vGridPos;
		CB::Collection::CArray<CField, 9>	m_Fields;

		CB::CRefPtr<CB::Graphic::IShader>	m_pVertexShader;
		CB::CRefPtr<CB::Graphic::IShader>	m_pFragmentShader;

		CB::CRefPtr<CB::Graphic::IVertexDeclaration>	m_pDeclaration;
		CB::CRefPtr<CB::Graphic::IBuffer>	m_pTCoordBuffer;
		CB::CRefPtr<CB::Graphic::IBuffer>	m_pIndexBuffer;
		CB::CRefPtr<CB::Graphic::IDeviceState>	m_pState;

		CLevelMesh	m_Grid;
		CLevelMesh	m_Cross;
		CLevelMesh	m_Circle;

	public:
		CLevel(CB::CRefPtr<CB::Graphic::IDevice> pDevice, CTextureManager& TexMng, CShaderManager& ShdMng);

		void	Update(const float32 fTD);
		void	Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice);

		void	SetSceneSize(const CB::Math::CVector2D& vSize);
		void	SetGridPos(const CB::Math::CVector2D& vPosition);

		void	SetModelMatrix(const CB::Math::CMatrix& mModel);
		void	SetMousePos(const CB::Math::CVector2D& vPosition);

		void	PutField(const CB::Math::CVector2D& vPosition, const FieldType uType);

	private:
		void	RenderMesh(CB::CRefPtr<CB::Graphic::IDevice> pDevice, CLevelMesh& Mesh);
	};
}