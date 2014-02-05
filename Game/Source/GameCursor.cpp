#include "GameCursor.h"

namespace T3{
	const CB::CString	CGameCursor::CURSOR_SHADER = L"CursorShader";
	const CB::CString	CGameCursor::CURSOR_TEXTURE = L"pointer";

	class CCursorVertex{
	public:
		CB::Math::CVector2D	Position;
		CB::Math::CVector2D TexCoord;

		CCursorVertex(const float32 x, const float32 y, const float32 s, const float32 t) :
			Position(x, y),
			TexCoord(s, t)
		{}
	};

	CGameCursor::CGameCursor(CB::CRefPtr<CB::Graphic::IDevice> pDevice, const CB::Math::CSize& ScreenSize, CTextureManager& TexMgr, CShaderManager& ShaderMng) : 
		m_SreenSize(ScreenSize),
		m_pVShader(ShaderMng.Load(CURSOR_SHADER, CB::Graphic::ShaderType::Vertex)),
		m_pFragment(ShaderMng.Load(CURSOR_SHADER, CB::Graphic::ShaderType::Fragment)),
		m_pPointer(TexMgr.Load(CURSOR_TEXTURE))
	{
		CB::Collection::CList<CB::Graphic::CVertexElement> els;
		els.Add(CB::Graphic::CVertexElement(0, L"vinput.vPosition", CB::Graphic::VertexType::Float, 2, 0));
		els.Add(CB::Graphic::CVertexElement(0, L"vinput.vTexCoord", CB::Graphic::VertexType::Float, 2, els.Last().GetSize()));
		this->m_pVDecl = pDevice->CreateVertexDeclaration(this->m_pVShader, els);

		CB::Collection::CList<CCursorVertex>	verts;
		CB::Collection::CList<uint16>			inds;

		auto texSize = this->m_pPointer->GetSize();
		verts.Add(CCursorVertex(0.0f, 0.0f, 0.0f, 0.0f));
		verts.Add(CCursorVertex((float32)texSize.Width, 0.0f, 1.0f, 0.0f));
		verts.Add(CCursorVertex((float32)texSize.Width, (float32)texSize.Height, 1.0f, 1.0f));
		verts.Add(CCursorVertex(0.0f, (float32)texSize.Height, 0.0f, 1.0f));
		this->m_CursorSize = texSize;

		inds.Add(0); inds.Add(2); inds.Add(1);
		inds.Add(0); inds.Add(3); inds.Add(2);

		this->m_pVBuffer = pDevice->CreateBuffer(CB::Graphic::BufferType::Vertex, CB::Graphic::BufferUsage::Static, CB::Graphic::BufferAccess::Write, verts);
		this->m_pIBuffer = pDevice->CreateBuffer(CB::Graphic::BufferType::Index, CB::Graphic::BufferUsage::Static, CB::Graphic::BufferAccess::Write, inds);

		CB::Graphic::CBlendStateDesc blendDesc(true, 
			CB::Graphic::CBlendInstDesc(CB::Graphic::BlendOption::SourceAlpha, CB::Graphic::BlendOperation::Add, CB::Graphic::BlendOption::OneMinusSourceAlpha),
			CB::Graphic::CBlendInstDesc(CB::Graphic::BlendOption::SourceAlpha, CB::Graphic::BlendOperation::Add, CB::Graphic::BlendOption::OneMinusSourceAlpha),
			0xFF
			);
		this->m_pBlendState = pDevice->CreateState(blendDesc);
	}

	void	CGameCursor::SetPos(const CB::Math::CVector2D& Position){
		this->m_vCursorPos = Position * CB::Math::CVector2D(this->m_SreenSize.ToPoint());
	}

	void	CGameCursor::Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice){
		CB::Math::CMatrix	mModel	= CB::Math::CMatrix::GetTranslation(0.0f, -(float32)this->m_CursorSize.Height, 0.0f);
		CB::Math::CMatrix	mView	= CB::Math::CMatrix::GetTranslation(this->m_vCursorPos.X, this->m_vCursorPos.Y, 0.0f);
		CB::Math::CMatrix	mProj	= CB::Math::CMatrix::GetOrtho(0.0f, (float32)this->m_SreenSize.Width, 0.0f, (float32)this->m_SreenSize.Height, -3.0f, 1.0f);
		//CB::Math::CMatrix	mProj = CB::Math::CMatrix::GetOrtho((float32)this->m_SreenSize.Width, (float32)this->m_SreenSize.Height, -2.0f, 2.0f);

		this->m_pVShader->SetUniform(L"mModelViewProj", mProj * mView * mModel );
		this->m_pFragment->SetSampler(L"texBase", this->m_pPointer.Cast<CB::Graphic::IBaseTexture>());

		pDevice->SetShader(this->m_pVShader);
		pDevice->SetShader(this->m_pFragment);

		pDevice->SetVertexDeclaration(this->m_pVDecl);
		pDevice->SetVertexBuffer(0, this->m_pVBuffer);
		pDevice->SetIndexBuffer(this->m_pIBuffer);

		pDevice->SetState(this->m_pBlendState.Cast<CB::Graphic::IDeviceState>());

		pDevice->RenderIndexed(2);

		pDevice->FreeState(CB::Graphic::DeviceStateType::Blend);
		pDevice->FreeVertexBuffer(0);
		pDevice->FreeIndexBuffer();
		pDevice->FreeShader(CB::Graphic::ShaderType::Vertex);
		pDevice->FreeShader(CB::Graphic::ShaderType::Fragment);
		pDevice->FreeVertexDeclaration();
	}

}