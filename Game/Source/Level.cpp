#include "Level.h"

namespace T3{
	const uint32 FIELD_SIZE_X = 3;
	const uint32 FIELD_SIZE_Y = 3;

	const CB::CString	SHADER_GRID = L"BasicShader";
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

		CB::Collection::CArray<CB::Math::CVector3D, 4> verts;
		verts[0].Set(0.0f, 0.0f);
		verts[1].Set(this->m_vFieldSize.X, 0.0f);
		verts[2].Set(this->m_vFieldSize.X, this->m_vFieldSize.Y);
		verts[3].Set(0.0f, this->m_vFieldSize.Y);

		CB::Collection::CArray<CB::Math::CVector2D, 4> tcoords;
		tcoords[0].Set(0.0f, 0.0f);
		tcoords[1].Set(1.0f, 0.0f);
		tcoords[2].Set(1.0f, 1.0f);
		tcoords[3].Set(0.0f, 1.0f);

		CB::Collection::CArray<uint16, 6> idxs;
		idxs[0] = 0;
		idxs[1] = 2;
		idxs[2] = 1;

		idxs[3] = 0;
		idxs[4] = 3;
		idxs[5] = 2;

		CB::Collection::CList<CB::Graphic::CVertexElement> vEls;
		vEls.Add(CB::Graphic::CVertexElement(0, L"vinput.vPosition", 0, CB::Graphic::VertexType::Float, 3));
		vEls.Add(CB::Graphic::CVertexElement(1, L"vinput.vTexCoord", 0, CB::Graphic::VertexType::Float, 2));

		this->m_pVertexShader = ShdMng.Load(SHADER_GRID, CB::Graphic::ShaderType::Vertex);
		this->m_pFragmentShader = ShdMng.Load(SHADER_GRID, CB::Graphic::ShaderType::Fragment);

		this->m_pFrame = TexMng.Load(TEXTURE_FRAME);

		this->m_pDeclaration = pDevice->CreateVertexDeclaration(this->m_pVertexShader, vEls);
		this->m_pVertexBuffer = pDevice->CreateBuffer(CB::Graphic::BufferType::Vertex, CB::Graphic::BufferUsage::Static, CB::Graphic::BufferAccess::Write, verts);
		this->m_pTCoordBuffer = pDevice->CreateBuffer(CB::Graphic::BufferType::Vertex, CB::Graphic::BufferUsage::Static, CB::Graphic::BufferAccess::Write, tcoords);
		this->m_pIndexBuffer = pDevice->CreateBuffer(CB::Graphic::BufferType::Index, CB::Graphic::BufferUsage::Static, CB::Graphic::BufferAccess::Write, idxs);

		this->m_mModel = CB::Math::CMatrix::GetIdentity();
	}

	void	CLevel::Update(const float32 fTD){
	}

	void	CLevel::Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice){
		auto mView = CB::Math::CMatrix::GetIdentity();
		auto mProj = CB::Math::CMatrix::GetOrtho(0.0f, 6.4f, 0.0f, 4.8f, -1.0f, 4.0f);

		pDevice->SetShader(this->m_pVertexShader);
		pDevice->SetShader(this->m_pFragmentShader);

		this->m_pVertexShader->SetUniform(L"mModelViewProj", mProj * mView * this->m_mModel);
		this->m_pFragmentShader->SetSampler(L"texBase", this->m_pFrame.Cast<CB::Graphic::IBaseTexture>());

		pDevice->SetVertexDeclaration(this->m_pDeclaration);
		pDevice->SetVertexBuffer(0, this->m_pVertexBuffer);
		pDevice->SetVertexBuffer(1, this->m_pTCoordBuffer);
		pDevice->SetIndexBuffer(this->m_pIndexBuffer);

		for(uint32 y = 0; y < FIELD_SIZE_Y; y++){
			for(uint32 x = 0; x < FIELD_SIZE_X; x++){
				mView = CB::Math::CMatrix::GetTranslation(CB::Math::CVector3D(this->m_vFieldSize * CB::Math::CVector2D((float32)x, (float32)y), 0.0f));
				this->m_pVertexShader->SetUniform(L"mModelViewProj", mProj * mView * this->m_mModel);
				pDevice->RenderIndexed(2);
			}
		}

	}

	void	CLevel::SetModelMatrix(const CB::Math::CMatrix& mModel){
		this->m_mModel = mModel;
	}

	void	CLevel::SetMousePos(const CB::Math::CVector2D& vPosition){
		
	}
}