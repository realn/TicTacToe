#include "RectMesh.h"

namespace T3{
	CRectMesh::CRectMesh(CB::CRefPtr<CB::Graphic::IDevice> pDevice, const CB::Math::CVector2D& vSize, CB::CRefPtr<CB::Graphic::IShader> pVertexShader){
		{
			CB::Collection::CList<CB::Graphic::CVertexElement> vEls;
			vEls.Add(CB::Graphic::CVertexElement(0, L"vinput.vPosition", CB::Graphic::VertexType::Float, 2, 0));
			vEls.Add(CB::Graphic::CVertexElement(0, L"vinput.vTexCoord", CB::Graphic::VertexType::Float, 2, vEls.Last().GetSize()));

			this->m_pDeclaration = pDevice->CreateVertexDeclaration(pVertexShader, vEls);
		}

		{
			float32 fX = vSize.X / 2.0f;
			float32 fY = vSize.Y / 2.0f;

			CB::Collection::CList<CB::Math::CVector4D> verts;
			verts.Add(CB::Math::CVector4D(-fX, -fY, 0.0f, 0.0f));
			verts.Add(CB::Math::CVector4D(fX, -fY, 1.0f, 0.0f));
			verts.Add(CB::Math::CVector4D(fX, fY, 1.0f, 1.0f));
			verts.Add(CB::Math::CVector4D(-fX, fY, 0.0f, 1.0f));

			this->m_pVBuffer = pDevice->CreateBuffer(
				CB::Graphic::BufferType::Vertex,
				CB::Graphic::BufferUsage::Static,
				CB::Graphic::BufferAccess::Write,
				verts
				);
		}

		{
			CB::Collection::CList<uint16> idxs;
			idxs.Add(0); idxs.Add(2); idxs.Add(1);
			idxs.Add(0); idxs.Add(3); idxs.Add(2);

			this->m_pIBuffer = pDevice->CreateBuffer(
				CB::Graphic::BufferType::Index,
				CB::Graphic::BufferUsage::Static,
				CB::Graphic::BufferAccess::Write,
				idxs
				);
		}
	}

	void	CRectMesh::BeginRender(CB::CRefPtr<CB::Graphic::IDevice> pDevice){
		pDevice->SetVertexDeclaration(this->m_pDeclaration);
		pDevice->SetVertexBuffer(0, this->m_pVBuffer);
		pDevice->SetIndexBuffer(this->m_pIBuffer);
		//pDevice->SetRenderPrimitive(CB::Graphic::PrimitiveType::Triangles);
	}

	void	CRectMesh::Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice){
		pDevice->RenderIndexed(2);
	}

	void	CRectMesh::EndRender(CB::CRefPtr<CB::Graphic::IDevice> pDevice){
		pDevice->FreeVertexDeclaration();
		pDevice->FreeVertexBuffer(0);
		pDevice->FreeIndexBuffer();
	}
}