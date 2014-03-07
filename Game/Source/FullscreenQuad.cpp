#include "FullscreenQuad.h"
#include <Tools_BindGuards.h>

namespace T3{
	const CB::CString	SHADER_ASSET = L"BasicColorShader";
	const CB::CString	SHADER_PARAM_POSITION = L"vinput.vPosition";
	const CB::CString	SHADER_CONST_TRANSFORM = L"mTransform";
	const CB::CString	SHADER_CONST_COLOR = L"vColor";

	CFullscreenQuad::CFullscreenQuad(CB::CRefPtr<CB::Graphic::IDevice> pDevice, CShaderManager& ShaderManager) :
		m_pDevice(pDevice),
		m_pVShader(ShaderManager.Load(SHADER_ASSET, CB::Graphic::ShaderType::Vertex)),
		m_pFShader(ShaderManager.Load(SHADER_ASSET, CB::Graphic::ShaderType::Fragment))
	{
		using namespace CB::Graphic;
		{
			CB::Collection::CList<CVertexElement> els;
			els.Add(CVertexElement(0, SHADER_PARAM_POSITION, VertexType::Float, 3, 0));
			this->m_pVDecl = pDevice->CreateVertexDeclaration(this->m_pVShader, els);
		}
		{
			CB::Collection::CList<CB::Math::CVector3D>	verts;
			verts.Add(CB::Math::CVector3D(0.0f, 0.0f, 0.0f));
			verts.Add(CB::Math::CVector3D(1.0f, 0.0f, 0.0f));
			verts.Add(CB::Math::CVector3D(1.0f, 1.0f, 0.0f));
			verts.Add(CB::Math::CVector3D(0.0f, 1.0f, 0.0f));

			this->m_pVBuffer = pDevice->CreateBuffer(BufferType::Vertex, BufferUsage::Static, BufferAccess::Write, verts);
		}
		{
			CB::Collection::CList<uint16> indices;
			indices.Add(0);	indices.Add(2);	indices.Add(1);	
			indices.Add(0);	indices.Add(3);	indices.Add(2);

			this->m_pIBuffer = pDevice->CreateBuffer(BufferType::Index, BufferUsage::Static, BufferAccess::Write, indices);
		}
		{
			CBlendInstDesc inst(BlendOption::SourceAlpha, BlendOperation::Add, BlendOption::OneMinusSourceAlpha);
			CBlendStateDesc desc(true, inst, inst, 0xFF);

			this->m_pBlendState = pDevice->CreateState(desc).Cast<CB::Graphic::IDeviceState>();
		}
	}

	void	CFullscreenQuad::Render(const CB::Math::CColor& Color){
		auto mTransform = CB::Math::CMatrix::GetOrtho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);

		this->m_pVShader->SetUniform(SHADER_CONST_TRANSFORM, mTransform);
		this->m_pFShader->SetUniform(SHADER_CONST_COLOR, Color.ToVector4D());
		
		CB::Tools::CGuardShader			vshader(this->m_pDevice, this->m_pVShader);
		CB::Tools::CGuardShader			fshader(this->m_pDevice, this->m_pFShader);
		CB::Tools::CGuardVertexDecl		vdecl(this->m_pDevice, this->m_pVDecl);
		CB::Tools::CGuardVertexStream	vstream(this->m_pDevice, 0, this->m_pVBuffer);
		CB::Tools::CGuardIndexStream	istream(this->m_pDevice, this->m_pIBuffer);
		CB::Tools::CGuardDeviceState	bstate(this->m_pDevice, this->m_pBlendState);

		this->m_pDevice->RenderIndexed(2);
	}
}