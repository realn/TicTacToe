#include "GameCursor.h"

namespace T3{
	const CB::CString	CGameCursor::CURSOR_SHADER = L"BasicShader";
	const CB::CString	CGameCursor::CURSOR_TEXTURE = L"pointer";

	CGameCursor::CGameCursor(CB::CRefPtr<CB::Graphic::IDevice> pDevice, const CB::Math::CSize& ScreenSize, CTextureManager& TexMgr, CShaderManager& ShaderMng) : 
		m_SreenSize(ScreenSize)
	{
		this->m_pVShader = ShaderMng.Load(CURSOR_SHADER, CB::Graphic::ShaderType::Vertex);
		this->m_pFragment = ShaderMng.Load(CURSOR_SHADER, CB::Graphic::ShaderType::Fragment);
		this->m_pPointer = TexMgr.Load(CURSOR_TEXTURE);

		CB::Collection::CList<CB::Graphic::CVertexElement> els;
		els.Add(CB::Graphic::CVertexElement(0, L"vinput.vPosition", 0, CB::Graphic::VertexType::Float, 3));
		els.Add(CB::Graphic::CVertexElement(1, L"vinput.vTexCoord", 0, CB::Graphic::VertexType::Float, 2));

		this->m_pVDecl = pDevice->CreateVertexDeclaration(this->m_pVShader, els);

		CB::Collection::CList<CB::Math::CVector3D> verts;
		CB::Collection::CList<CB::Math::CVector2D> tcoords;
		CB::Collection::CList<uint16>	inds;

		auto texSize = this->m_pPointer->GetSize();
		verts.Add(CB::Math::CVector3D(0.0f, 0.0f, 0.0f));
		verts.Add(CB::Math::CVector3D((float32)texSize.Width, 0.0f, 0.0f));
		verts.Add(CB::Math::CVector3D((float32)texSize.Width, (float32)texSize.Height, 0.0f));
		verts.Add(CB::Math::CVector3D(0.0f, (float32)texSize.Height, 0.0f));
		this->m_CursorSize = texSize;
		
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
		this->m_CursorPos = CB::Math::CPoint(Position.X, this->m_SreenSize.Height - Position.Y);
	}

	void	CGameCursor::Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice){
		CB::Math::CMatrix	mModel = CB::Math::CMatrix::GetTranslation(0.0f, -(float32)this->m_CursorSize.Height, 0.0f);
		CB::Math::CMatrix	mView = CB::Math::CMatrix::GetTranslation((float32)this->m_CursorPos.X, (float32)this->m_CursorPos.Y, 0.0f);
		CB::Math::CMatrix	mProj = CB::Math::CMatrix::GetOrtho(0.0f, (float32)this->m_SreenSize.Width, 0.0f, (float32)this->m_SreenSize.Height, -3.0f, 1.0f);
		//CB::Math::CMatrix	mProj = CB::Math::CMatrix::GetOrtho((float32)this->m_SreenSize.Width, (float32)this->m_SreenSize.Height, -2.0f, 2.0f);

		this->m_pVShader->SetUniform(L"mModelViewProj", mProj * mView * mModel );
		this->m_pFragment->SetSampler(L"texBase", this->m_pPointer.Cast<CB::Graphic::IBaseTexture>());

		pDevice->SetShader(this->m_pVShader);
		pDevice->SetShader(this->m_pFragment);

		pDevice->SetVertexDeclaration(this->m_pVDecl);
		pDevice->SetVertexBuffer(0, this->m_pVBuffer);
		pDevice->SetVertexBuffer(1, this->m_pTBuffer);
		pDevice->SetIndexBuffer(this->m_pIBuffer);

		pDevice->SetState(this->m_pBlendState.Cast<CB::Graphic::IDeviceState>());

		pDevice->RenderIndexed(2);
	}

}