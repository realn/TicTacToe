#include "GameCursor.h"

namespace T3{
	const CB::CString	CGameCursor::CURSOR_SHADER = L"BasicShader";
	const CB::CString	CGameCursor::CURSOR_TEXTURE = L"pointer";

	CGameCursor::CGameCursor(CB::CRefPtr<CB::Graphic::IDevice> pDevice, const CB::Math::CSize& ScreenSize, CTextureManager& TexMgr, CShaderManager& ShaderMng) : 
		m_SreenSize(ScreenSize)
	{
		this->m_pVShader = ShaderMng.Load(CURSOR_SHADER, CB::Graphic::ShaderType::Vertex);
		this->m_pFragment = ShaderMng.Load(CURSOR_SHADER, CB::Graphic::ShaderType::Vertex);
		this->m_pPointer = TexMgr.Load(CURSOR_TEXTURE);

		CB::Collection::CList<CB::Graphic::CVertexElement> els;
		els.Add(CB::Graphic::CVertexElement(0, L"vinput.vPosition", 0, CB::Graphic::VertexType::Float, 3));
		els.Add(CB::Graphic::CVertexElement(1, L"vinput.vTexCoord", 0, CB::Graphic::VertexType::Float, 2));

		this->m_pVDecl = pDevice->CreateVertexDeclaration(this->m_pVShader, els);

		CB::Collection::CList<CB::Math::CVector3D> verts;
		CB::Collection::CList<CB::Math::CVector2D> tcoords;
		CB::Collection::CList<uint16>	inds;

		verts.Add(CB::Math::CVector3D(-1.0f, -1.0f, 0.0f));
		verts.Add(CB::Math::CVector3D(1.0f, -1.0f, 0.0f));
		verts.Add(CB::Math::CVector3D(1.0f, 1.0f, 0.0f));
		verts.Add(CB::Math::CVector3D(-1.0f, 1.0f, 0.0f));
		
		tcoords.Add(CB::Math::CVector2D(0.0f, 0.0f));
		tcoords.Add(CB::Math::CVector2D(1.0f, 0.0f));
		tcoords.Add(CB::Math::CVector2D(1.0f, 1.0f));
		tcoords.Add(CB::Math::CVector2D(0.0f, 1.0f));

		inds.Add(0);
		inds.Add(2);
		inds.Add(1);

		inds.Add(0);
		inds.Add(3);
		inds.Add(2);

		this->m_pVBuffer = pDevice->CreateBuffer(CB::Graphic::BufferType::Vertex, CB::Graphic::BufferUsage::Static, CB::Graphic::BufferAccess::Write, verts);
		this->m_pTBuffer = pDevice->CreateBuffer(CB::Graphic::BufferType::Vertex, CB::Graphic::BufferUsage::Static, CB::Graphic::BufferAccess::Write, tcoords);
		this->m_pIBuffer = pDevice->CreateBuffer(CB::Graphic::BufferType::Index, CB::Graphic::BufferUsage::Static, CB::Graphic::BufferAccess::Write, inds);


		CB::Graphic::CBlendStateDesc blendDesc(true, 
			CB::Graphic::CBlendInstDesc(CB::Graphic::BlendOption::SourceAlpha, CB::Graphic::BlendOperation::Add, CB::Graphic::BlendOption::OneMinusSourceAlpha),
			CB::Graphic::CBlendInstDesc(CB::Graphic::BlendOption::SourceAlpha, CB::Graphic::BlendOperation::Add, CB::Graphic::BlendOption::OneMinusSourceAlpha),
			0xFF
			);
		this->m_pBlendState = pDevice->CreateState(blendDesc);

	}

	void	CGameCursor::SetPos(const CB::Math::CPoint& Position){
		this->m_CursorPos = Position;
	}

	void	CGameCursor::Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice){
		CB::Math::CMatrix	mModel = CB::Math::CMatrix::GetIdentity();
		CB::Math::CMatrix	mView = CB::Math::CMatrix::GetTranslation(0.0f, 0.0f, -1.0f);
		CB::Math::CMatrix	mProj = CB::Math::CMatrix::GetOrtho(4.0f, 4.0f, -2.0f, 2.0f);

		pDevice->SetShader(this->m_pVShader);
		pDevice->SetShader(this->m_pFragment);

		this->m_pVShader->SetUniform(L"mModelViewProj", mModel * mView * mProj);
		this->m_pFragment->SetSampler(L"texBase", this->m_pPointer.Cast<CB::Graphic::IBaseTexture>());

		pDevice->SetVertexDeclaration(this->m_pVDecl);
		pDevice->SetVertexBuffer(0, this->m_pVBuffer);
		pDevice->SetVertexBuffer(1, this->m_pTBuffer);
		pDevice->SetIndexBuffer(this->m_pIBuffer);

		pDevice->SetState(this->m_pBlendState.Cast<CB::Graphic::IDeviceState>());

		pDevice->RenderIndexed(2);
	}

}