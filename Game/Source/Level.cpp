#include "Level.h"
#include <Math.h>

namespace T3{
	const uint32 FIELD_SIZE_X = 3;
	const uint32 FIELD_SIZE_Y = 3;
	const uint32 FIELD_SIZE_SUM = FIELD_SIZE_X * FIELD_SIZE_Y;

	const CB::CString	SHADER_GRID = L"GridShader";
	const CB::CString	TEXTURE_FRAME = L"frame";
	const CB::CString	TEXTURE_CIRCLE = L"circle";
	const CB::CString	TEXTURE_CROSS = L"cross";

	CLevel::CLevel(CB::CRefPtr<CB::Graphic::IDevice> pDevice, const CB::Math::CVector2D& vSize, CTextureManager& TexMng, CShaderManager& ShdMng) :
		m_vSize(vSize),
		m_Grid(pDevice, FIELD_SIZE_SUM, TexMng.Load(TEXTURE_FRAME)),
		m_Cross(pDevice, FIELD_SIZE_SUM, TexMng.Load(TEXTURE_CROSS)),
		m_Circle(pDevice, FIELD_SIZE_SUM, TexMng.Load(TEXTURE_CIRCLE))
	{
		this->m_vFieldSize = this->m_vSize / CB::Math::CVector2D((float32)FIELD_SIZE_X, (float32)FIELD_SIZE_Y);

		for(uint32 y = 0; y < FIELD_SIZE_Y; y++){
			for(uint32 x = 0; x < FIELD_SIZE_X; x++){
				uint32 uIndex = y * FIELD_SIZE_X + x;
				CField& field = this->m_Fields[uIndex];

				field.uIndex = uIndex;
				field.bHover = false;
				field.Rect.Position = this->m_vFieldSize * CB::Math::CVector2D((float32)x, (float32)y);
				field.Rect.Size = this->m_vFieldSize;
			}
		}

		CB::Collection::CList<CB::Math::CVector2D> tcoords;
		CB::Collection::CList<uint16> idxs;

		for(uint32 i = 0; i < this->m_Fields.GetLength(); i++){
			uint32 uIndex = tcoords.GetLength();

			CField& field = this->m_Fields[i];
			field.Alpha = 0.0f;

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

		this->m_pDeclaration = pDevice->CreateVertexDeclaration(this->m_pVertexShader, vEls);
		this->m_pTCoordBuffer = pDevice->CreateBuffer(CB::Graphic::BufferType::Vertex, CB::Graphic::BufferUsage::Static, CB::Graphic::BufferAccess::Write, tcoords);
		this->m_pIndexBuffer = pDevice->CreateBuffer(CB::Graphic::BufferType::Index, CB::Graphic::BufferUsage::Static, CB::Graphic::BufferAccess::Write, idxs);

		this->m_mModel = CB::Math::CMatrix::GetIdentity();
	}

	void	CLevel::Update(const float32 fTD){
		CB::Collection::CList<CLevelFieldMesh> fields;
		CB::Collection::CList<CLevelFieldMesh> crossFields;
	
		uint16 uIndex = 0;
		for(uint32 i = 0; i < this->m_Fields.GetLength(); i++){
			CField& field = this->m_Fields[i];

			field.Alpha += fTD * (field.bHover ? 1.0f : -1.0f);
			field.Alpha = CB::Math::Clamp01(field.Alpha);
			
			if(field.Alpha > 0.0f){
				CLevelFieldMesh mesh;

				mesh.SetRect(field.Rect);
				mesh.SetAlpha(field.Alpha);

				fields.Add(mesh);
			}

			switch (field.Type)
			{
			case FieldType::Cross:
				{
					CLevelFieldMesh mesh;
					mesh.SetRect(field.Rect);
					mesh.SetAlpha(1.0f);
					crossFields.Add(mesh);
				}
				break;

			default:
				break;
			}
		}

		this->m_Grid.Set(fields);
		this->m_Cross.Set(crossFields);
	}

	void	CLevel::Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice){
		auto mView = CB::Math::CMatrix::GetIdentity();
		auto mProj = CB::Math::CMatrix::GetOrtho(0.0f, 6.4f, 0.0f, 4.8f, -1.0f, 4.0f);

		pDevice->SetShader(this->m_pVertexShader);
		pDevice->SetShader(this->m_pFragmentShader);

		this->m_pVertexShader->SetUniform(L"mModelViewProj", mProj * mView * this->m_mModel);

		pDevice->SetVertexDeclaration(this->m_pDeclaration);
		pDevice->SetVertexBuffer(1, this->m_pTCoordBuffer);
		pDevice->SetIndexBuffer(this->m_pIndexBuffer);

		if(this->m_Grid.GetNumberOfPolygons() > 0){
			this->m_pFragmentShader->SetSampler(L"texBase", this->m_Grid.GetTexture());

			pDevice->SetVertexBuffer(0, this->m_Grid.GetBuffer());
			pDevice->RenderIndexed(this->m_Grid.GetNumberOfPolygons());
		}
		if(this->m_Cross.GetNumberOfPolygons() > 0){
			this->m_pFragmentShader->SetSampler(L"texBase", this->m_Cross.GetTexture());

			pDevice->SetVertexBuffer(0, this->m_Cross.GetBuffer());
			pDevice->RenderIndexed(this->m_Cross.GetNumberOfPolygons());
		}

		pDevice->FreeVertexDeclaration();
		pDevice->FreeIndexBuffer();
		pDevice->FreeShader(CB::Graphic::ShaderType::Vertex);
		pDevice->FreeShader(CB::Graphic::ShaderType::Fragment);
		pDevice->FreeVertexBuffer(0);
		pDevice->FreeVertexBuffer(1);
	}

	void	CLevel::SetModelMatrix(const CB::Math::CMatrix& mModel){
		this->m_mModel = mModel;
	}

	void	CLevel::SetMousePos(const CB::Math::CVector2D& vPosition){
		for(uint32 i = 0; i < this->m_Fields.GetLength(); i++){
			CField& field = this->m_Fields[i];

			field.bHover = field.Rect.Contains(vPosition * CB::Math::CVector2D(6.4f, 4.8f));
		}
	}

	void	CLevel::PutField(const CB::Math::CVector2D& vPosition, const CLevel::FieldType uType){
		for(uint32 i = 0; i < this->m_Fields.GetLength(); i++){
			CField& field = this->m_Fields[i];

			if(field.Rect.Contains(vPosition * CB::Math::CVector2D(6.4f, 4.8f))){
				field.Type = FieldType::Cross;
			}
		}
	}
}