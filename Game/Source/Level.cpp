#include "Level.h"
#include <Math.h>

namespace T3{
	const uint32 FIELD_SIZE_X = 3;
	const uint32 FIELD_SIZE_Y = 3;
	const uint32 FIELD_SIZE_SUM = FIELD_SIZE_X * FIELD_SIZE_Y;

	const CB::CString	SHADER_GRID = L"GridShader";
	const CB::CString	SHADER_BACKGROUND = L"BackGroundShader";
	const CB::CString	TEXTURE_FRAME = L"frame";
	const CB::CString	TEXTURE_CIRCLE = L"circle";
	const CB::CString	TEXTURE_CROSS = L"cross";
	const CB::CString	TEXTURE_BACKGROUND = L"backgroud";

	CLevel::CLevel(CB::CRefPtr<CB::Graphic::IDevice> pDevice, CTextureManager& TexMng, CShaderManager& ShdMng) :
		m_Grid(pDevice, FIELD_SIZE_SUM, TexMng.Load(TEXTURE_FRAME)),
		m_Cross(pDevice, FIELD_SIZE_SUM, TexMng.Load(TEXTURE_CROSS)),
		m_Circle(pDevice, FIELD_SIZE_SUM, TexMng.Load(TEXTURE_CIRCLE)),
		m_vSceneSize(1.0f, 1.0f)
	{
		for(uint32 y = 0; y < FIELD_SIZE_Y; y++){
			for(uint32 x = 0; x < FIELD_SIZE_X; x++){
				uint32 uIndex = y * FIELD_SIZE_X + x;
				CField& field = this->m_Fields[uIndex];

				float32 fX = (float32)x / (float32)FIELD_SIZE_X;
				float32 fY = (float32)y / (float32)FIELD_SIZE_Y;
				float32 fW = 1.0f / (float32)FIELD_SIZE_X;
				float32 fH = 1.0f / (float32)FIELD_SIZE_Y;

				field.uIndex = uIndex;
				field.bHover = false;
				field.Rect.Position.Set(fX, fY);
				field.Rect.Size.Set(fW, fH);
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
		vEls.Add(CB::Graphic::CVertexElement(0, L"vinput.vPosition", CB::Graphic::VertexType::Float, 4, 0));
		vEls.Add(CB::Graphic::CVertexElement(1, L"vinput.vTexCoord", CB::Graphic::VertexType::Float, 2, 0));

		this->m_pVertexShader = ShdMng.Load(SHADER_GRID, CB::Graphic::ShaderType::Vertex);
		this->m_pFragmentShader = ShdMng.Load(SHADER_GRID, CB::Graphic::ShaderType::Fragment);

		this->m_pDeclaration = pDevice->CreateVertexDeclaration(this->m_pVertexShader, vEls);
		this->m_pTCoordBuffer = pDevice->CreateBuffer(CB::Graphic::BufferType::Vertex, CB::Graphic::BufferUsage::Static, CB::Graphic::BufferAccess::Write, tcoords);
		this->m_pIndexBuffer = pDevice->CreateBuffer(CB::Graphic::BufferType::Index, CB::Graphic::BufferUsage::Static, CB::Graphic::BufferAccess::Write, idxs);

		CB::Graphic::CBlendStateDesc desc(true, 
			CB::Graphic::CBlendInstDesc(CB::Graphic::BlendOption::SourceAlpha, CB::Graphic::BlendOperation::Add, CB::Graphic::BlendOption::OneMinusSourceAlpha),
			CB::Graphic::CBlendInstDesc(CB::Graphic::BlendOption::SourceAlpha, CB::Graphic::BlendOperation::Add, CB::Graphic::BlendOption::OneMinusSourceAlpha),
			0xFF);
		this->m_pState = pDevice->CreateState(desc).Cast<CB::Graphic::IDeviceState>();

		this->m_mModel = CB::Math::CMatrix::GetIdentity();
	}

	void	CLevel::Update(const float32 fTD){
		CB::Collection::CList<CLevelFieldMesh> fields;
		CB::Collection::CList<CLevelFieldMesh> crossFields;
		CB::Collection::CList<CLevelFieldMesh> circleFields;
	
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

			case FieldType::Circle:
				{
					CLevelFieldMesh mesh;
					mesh.SetRect(field.Rect);
					mesh.SetAlpha(1.0f);
					circleFields.Add(mesh);
				}
				break;

			default:
				break;
			}
		}

		this->m_Grid.Set(fields);
		this->m_Cross.Set(crossFields);
		this->m_Circle.Set(circleFields);
	}

	void	CLevel::Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice){
		auto mView = CB::Math::CMatrix::GetTranslation(CB::Math::CVector3D(this->m_vGridPos, 0.0f));
		auto mProj = CB::Math::CMatrix::GetOrtho(0.0f, this->m_vSceneSize.X, 0.0f, this->m_vSceneSize.Y, -1.0f, 4.0f);

		this->m_pVertexShader->SetUniform(L"mModelViewProj", mProj * mView * this->m_mModel);

		pDevice->SetShader(this->m_pVertexShader);
		pDevice->SetShader(this->m_pFragmentShader);
		pDevice->SetVertexDeclaration(this->m_pDeclaration);
		pDevice->SetVertexBuffer(1, this->m_pTCoordBuffer);
		pDevice->SetIndexBuffer(this->m_pIndexBuffer);
		pDevice->SetState(this->m_pState);

		this->RenderMesh(pDevice, this->m_Grid);
		this->RenderMesh(pDevice, this->m_Cross);
		this->RenderMesh(pDevice, this->m_Circle);

		pDevice->FreeState(CB::Graphic::DeviceStateType::Blend);
		pDevice->FreeVertexDeclaration();
		pDevice->FreeIndexBuffer();
		pDevice->FreeShader(CB::Graphic::ShaderType::Vertex);
		pDevice->FreeShader(CB::Graphic::ShaderType::Fragment);
		pDevice->FreeVertexBuffer(0);
		pDevice->FreeVertexBuffer(1);
	}

	void	CLevel::SetSceneSize(const CB::Math::CVector2D& vSize){
		this->m_vSceneSize = vSize;
	}

	void	CLevel::SetGridPos(const CB::Math::CVector2D& vPosition){
		this->m_vGridPos = vPosition;
	}

	void	CLevel::SetModelMatrix(const CB::Math::CMatrix& mModel){
		this->m_mModel = mModel;
	}

	void	CLevel::SetMousePos(const CB::Math::CVector2D& vPosition){
		auto vPos = vPosition * this->m_vSceneSize - this->m_vGridPos;
		for(uint32 i = 0; i < this->m_Fields.GetLength(); i++){
			CField& field = this->m_Fields[i];

			field.bHover = field.Rect.Contains(vPos);
		}
	}

	void	CLevel::PutField(const CB::Math::CVector2D& vPosition, const CLevel::FieldType uType){
		auto vPos = vPosition * this->m_vSceneSize - this->m_vGridPos;
		for(uint32 i = 0; i < this->m_Fields.GetLength(); i++){
			CField& field = this->m_Fields[i];

			if(field.Rect.Contains(vPos)){
				field.Type = uType;
			}
		}
	}

	void	CLevel::RenderMesh(CB::CRefPtr<CB::Graphic::IDevice> pDevice, CLevelMesh& Mesh){
		if(Mesh.GetNumberOfPolygons() > 0){
			this->m_pFragmentShader->SetSampler(L"texBase", Mesh.GetTexture());

			pDevice->SetVertexBuffer(0, Mesh.GetBuffer());
			pDevice->RenderIndexed(Mesh.GetNumberOfPolygons());
		}
	}
}