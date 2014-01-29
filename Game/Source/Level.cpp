#include "Level.h"
#include <Math.h>

namespace T3{
	const uint32 FIELD_SIZE_X = 3;
	const uint32 FIELD_SIZE_Y = 3;

	const CB::CString	SHADER_GRID = L"GridShader";
	const CB::CString	TEXTURE_FRAME = L"frame";
	const CB::CString	TEXTURE_CIRCLE = L"circle";
	const CB::CString	TEXTURE_CROSS = L"cross";

	CLevel::CLevel(CB::CRefPtr<CB::Graphic::IDevice> pDevice, const CB::Math::CVector2D& vSize, CTextureManager& TexMng, CShaderManager& ShdMng) :
		m_vSize(vSize)
	{
		this->m_vFieldSize = this->m_vSize / CB::Math::CVector2D((float32)FIELD_SIZE_X, (float32)FIELD_SIZE_Y);

		for(uint32 y = 0; y < FIELD_SIZE_Y; y++){
			for(uint32 x = 0; x < FIELD_SIZE_X; x++){
				uint32 uIndex = y * FIELD_SIZE_X + x;
				CField& field = this->m_Fields[uIndex];

				field.uIndex = uIndex;
				field.Rect.Position = this->m_vFieldSize * CB::Math::CVector2D((float32)x, (float32)y);
				field.Rect.Size = this->m_vFieldSize;
			}
		}

		CB::Collection::CList<CB::Math::CVector4D> verts;
		CB::Collection::CList<CB::Math::CVector2D> tcoords;
		CB::Collection::CList<uint16> idxs;

		for(uint32 i = 0; i < this->m_Fields.GetLength(); i++){
			uint32 uIndex = verts.GetLength();

			CField& field = this->m_Fields[i];
			field.Alpha = 0.0f;

			verts.Add(field.Rect.Position + CB::Math::CVector4D(0.0f, 0.0f, 0.0f, field.Alpha));
			verts.Add(field.Rect.Position + CB::Math::CVector4D(this->m_vFieldSize.X, 0.0f, 0.0f, field.Alpha));
			verts.Add(field.Rect.Position + CB::Math::CVector4D(this->m_vFieldSize.X, this->m_vFieldSize.Y, 0.0f, field.Alpha));
			verts.Add(field.Rect.Position + CB::Math::CVector4D(0.0f, this->m_vFieldSize.Y, 0.0f, field.Alpha));

			tcoords.Add(CB::Math::CVector2D(0.0f, 0.0f));
			tcoords.Add(CB::Math::CVector2D(1.0f, 0.0f));
			tcoords.Add(CB::Math::CVector2D(1.0f, 1.0f));
			tcoords.Add(CB::Math::CVector2D(0.0f, 1.0f));

			idxs.Add(uIndex + 0);
			idxs.Add(uIndex + 2);
			idxs.Add(uIndex + 1);
			idxs.Add(uIndex + 0);
			idxs.Add(uIndex + 3);
			idxs.Add(uIndex + 2);
		}

		CB::Collection::CList<CB::Graphic::CVertexElement> vEls;
		vEls.Add(CB::Graphic::CVertexElement(0, L"vinput.vPosition", 0, CB::Graphic::VertexType::Float, 4));
		vEls.Add(CB::Graphic::CVertexElement(1, L"vinput.vTexCoord", 0, CB::Graphic::VertexType::Float, 2));

		this->m_pVertexShader = ShdMng.Load(SHADER_GRID, CB::Graphic::ShaderType::Vertex);
		this->m_pFragmentShader = ShdMng.Load(SHADER_GRID, CB::Graphic::ShaderType::Fragment);

		this->m_pFrame = TexMng.Load(TEXTURE_FRAME);
		this->m_pCircle = TexMng.Load(TEXTURE_CIRCLE);
		this->m_pCross = TexMng.Load(TEXTURE_CROSS);

		this->m_pCross->SetFilters(CB::Graphic::TextureFilter::Linear, CB::Graphic::TextureFilter::Linear, CB::Graphic::TextureFilter::Linear);

		this->m_pDeclaration = pDevice->CreateVertexDeclaration(this->m_pVertexShader, vEls);
		this->m_pGridVertexBuffer = pDevice->CreateBuffer(CB::Graphic::BufferType::Vertex, CB::Graphic::BufferUsage::Static, CB::Graphic::BufferAccess::Write, verts);
		this->m_pTCoordBuffer = pDevice->CreateBuffer(CB::Graphic::BufferType::Vertex, CB::Graphic::BufferUsage::Static, CB::Graphic::BufferAccess::Write, tcoords);
		this->m_pIndexBuffer = pDevice->CreateBuffer(CB::Graphic::BufferType::Index, CB::Graphic::BufferUsage::Static, CB::Graphic::BufferAccess::Write, idxs);
		this->m_uNumberOfGridTris = 0;

		this->m_mModel = CB::Math::CMatrix::GetIdentity();
	}

	void	CLevel::Update(const float32 fTD){
		CB::Collection::CList<CB::Math::CVector4D> verts;
	
		uint16 uIndex = 0;
		for(uint32 i = 0; i < this->m_Fields.GetLength(); i++){
			CField& field = this->m_Fields[i];

			field.Alpha += fTD * (field.bHover ? 1.0f : -1.0f);
			field.Alpha = CB::Math::Clamp01(field.Alpha);
			
			if(field.Alpha > 0.0f){
				CB::Math::CVector4D vPos(field.Rect.Position, 0.0f, 0.0f);
				verts.Add(vPos + CB::Math::CVector4D(0.0f, 0.0f, 0.0f, field.Alpha));
				verts.Add(vPos + CB::Math::CVector4D(this->m_vFieldSize.X, 0.0f, 0.0f, field.Alpha));
				verts.Add(vPos + CB::Math::CVector4D(this->m_vFieldSize.X, this->m_vFieldSize.Y, 0.0f, field.Alpha));
				verts.Add(vPos + CB::Math::CVector4D(0.0f, this->m_vFieldSize.Y, 0.0f, field.Alpha));
			}
		}

		this->m_pGridVertexBuffer->LoadSubData(verts, 0);
		this->m_uNumberOfGridTris = (verts.GetLength() / 4) * 2;
	}

	void	CLevel::Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice){
		if(this->m_uNumberOfGridTris == 0)
			return;

		auto mView = CB::Math::CMatrix::GetIdentity();
		auto mProj = CB::Math::CMatrix::GetOrtho(0.0f, 6.4f, 0.0f, 4.8f, -1.0f, 4.0f);

		pDevice->SetShader(this->m_pVertexShader);
		pDevice->SetShader(this->m_pFragmentShader);

		this->m_pVertexShader->SetUniform(L"mModelViewProj", mProj * mView * this->m_mModel);
		this->m_pFragmentShader->SetSampler(L"texBase", this->m_pFrame.Cast<CB::Graphic::IBaseTexture>());

		pDevice->SetVertexDeclaration(this->m_pDeclaration);
		pDevice->SetVertexBuffer(0, this->m_pGridVertexBuffer);
		pDevice->SetVertexBuffer(1, this->m_pTCoordBuffer);
		pDevice->SetIndexBuffer(this->m_pIndexBuffer);

		pDevice->RenderIndexed(this->m_uNumberOfGridTris);
	}

	void	CLevel::SetModelMatrix(const CB::Math::CMatrix& mModel){
		this->m_mModel = mModel;
	}

	void	CLevel::SetMousePos(const CB::Math::CVector2D& vPosition){
		for(uint32 i = 0; i < this->m_Fields.GetLength(); i++){
			CField& field = this->m_Fields[i];

			field.bHover = field.Rect.Contains(vPosition);
		}
	}
}