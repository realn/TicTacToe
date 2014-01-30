#include "LevelMesh.h"

namespace T3{
	CLevelMesh::CLevelMesh(CB::CRefPtr<CB::Graphic::IDevice> pDevice, uint32 uMaxLength, CB::CRefPtr<CB::Graphic::ITexture2D> pTexture) :
		m_pTexture(pTexture),
		m_uNumberOfPolygons(0)
	{
		this->m_pTexture->SetFilters(CB::Graphic::TextureFilter::Linear, CB::Graphic::TextureFilter::Linear, CB::Graphic::TextureFilter::Linear);

		uint32 uMaxSize = sizeof(CB::Math::CVector4D) * 4 * uMaxLength;
		this->m_pBuffer = pDevice->CreateBuffer(CB::Graphic::BufferType::Vertex, CB::Graphic::BufferUsage::Stream, CB::Graphic::BufferAccess::Write, uMaxSize);
	}

	void	CLevelMesh::Set(const CB::Collection::ICountable<CLevelFieldMesh>& Field){
		CB::Collection::CList<CB::Math::CVector4D> verts;

		for(uint32 i = 0; i < Field.GetLength(); i++){
			Field[i].AddToList(verts);
		}
		if(Field.GetLength() > 0){
			this->m_pBuffer->LoadSubData(verts, 0);
		}
		this->m_uNumberOfPolygons = Field.GetLength() * 2;
	}

	CB::CRefPtr<CB::Graphic::IBaseTexture>	CLevelMesh::GetTexture() const{
		return this->m_pTexture.Cast<CB::Graphic::IBaseTexture>();
	}

	CB::CRefPtr<CB::Graphic::IBuffer>	CLevelMesh::GetBuffer() const{
		return this->m_pBuffer;
	}

	const uint32	CLevelMesh::GetNumberOfPolygons() const{
		return this->m_uNumberOfPolygons;
	}
}