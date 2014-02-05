#include "BackgroundGrid.h"

namespace T3{
	const CB::CString	TEXTURE_BACKGROUND	= L"BackGround";
	const CB::CString	SHADER_BACKGROUND	= L"BackGroundShader";

	CBGGridModel::CBGGridModel(CB::CRefPtr<CB::Graphic::IDevice> pDevice, CTextureManager& TexMng, CShaderManager& ShdMng) :
		m_pTexture(TexMng.Load(TEXTURE_BACKGROUND).Cast<CB::Graphic::IBaseTexture>()),
		m_pVertexShader(ShdMng.Load(SHADER_BACKGROUND, CB::Graphic::ShaderType::Vertex)),
		m_pFragmentShader(ShdMng.Load(SHADER_BACKGROUND, CB::Graphic::ShaderType::Fragment)),
		m_vGridPos(-0.5f),
		m_vScreenSize(1.0f),
		m_bMultiRender(false),
		CRectMesh(pDevice, CB::Math::CVector2D(1.0f, 1.0f), ShdMng.Load(SHADER_BACKGROUND, CB::Graphic::ShaderType::Vertex))
	{

	}

	void	CBGGridModel::SetScreenSize(const CB::Math::CVector2D& vSize){
		this->m_vScreenSize = vSize;
	}

	void	CBGGridModel::SetGridPos(const CB::Math::CVector2D& vPosition){
		this->m_vGridPos = vPosition - CB::Math::CVector2D(-0.5f);
	}

	void	CBGGridModel::BeginRender(CB::CRefPtr<CB::Graphic::IDevice> pDevice){
		if(this->m_bMultiRender){
			return;
		}
		this->m_bMultiRender = true;
		auto mView = CB::Math::CMatrix::GetTranslation(CB::Math::CVector3D(this->m_vGridPos, 0.0f));
		auto mProj = CB::Math::CMatrix::GetOrtho(0.0f, this->m_vScreenSize.X, 0.0f, this->m_vScreenSize.Y, -1.0f, 1.0f);

		this->m_pVertexShader->SetUniform(L"mModelViewProj", mProj * mView);
		this->m_pFragmentShader->SetSampler(L"texBase", this->m_pTexture);

		pDevice->SetShader(this->m_pVertexShader);
		pDevice->SetShader(this->m_pFragmentShader);
		
		CRectMesh::BeginRender(pDevice);
	}

	void	CBGGridModel::Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice){
		if(this->m_bMultiRender){
			CRectMesh::Render(pDevice);
		}
		else{
			this->BeginRender(pDevice);
			CRectMesh::Render(pDevice);
			this->EndRender(pDevice);
		}
	}

	void	CBGGridModel::EndRender(CB::CRefPtr<CB::Graphic::IDevice> pDevice){
		if(!this->m_bMultiRender){
			return;
		}
		this->m_bMultiRender = false;
		pDevice->FreeShader(CB::Graphic::ShaderType::Vertex);
		pDevice->FreeShader(CB::Graphic::ShaderType::Fragment);
		CRectMesh::EndRender(pDevice);
	}
}