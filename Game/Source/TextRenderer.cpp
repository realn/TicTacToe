#include "TextRenderer.h"
#include <IO_File.h>
#include <IO_Path.h>

namespace T3{
	const CB::CString	TEXT_SHADER = L"TextShader";
	const CB::CString	TEXT_FONT = L"Fonts/Exo2.0-Regular.otf";
	const CB::CString	SHADER_TRANSFORM = L"mTransform";
	const CB::CString	SHADER_COLOR = L"vColor";
	const CB::CString	SHADER_TEXTURE = L"texBase";
	const CB::CString	SHADER_PARAM_POSITION = L"vinput.vPosition";
	const CB::CString	SHADER_PARAM_TEXCOORD = L"vinput.vTexCoord";

	CTextRenderer::CTextRenderer(CB::CRefPtr<CB::Graphic::IDevice> pDevice, CShaderManager& ShdMng, const CB::CString& strAssetsDIr) :
		m_pDevice(pDevice),
		m_pVShader(ShdMng.Load(TEXT_SHADER, CB::Graphic::ShaderType::Vertex)),
		m_pFShader(ShdMng.Load(TEXT_SHADER, CB::Graphic::ShaderType::Fragment))
	{
		auto strPath = CB::IO::Path::Combine(strAssetsDIr, TEXT_FONT);
		auto pFontStream = CB::IO::File::Open(strPath).Cast<CB::IO::IStream>();

		auto pFontMng = CB::Font::CManager::Create();
		auto pFont = pFontMng->Load(pFontStream);

		pFont->SelectFace(0);
		pFont->SetSize(48);
		{
			using namespace CB::Graphic;

			CB::Collection::CList<CB::Tools::CFontCharDesc> charList;
			CB::Tools::CFontTextureGenerator fontTexGen(pDevice);
			fontTexGen.MaxTextureSize.Set(512, 512);
			fontTexGen.CharPadding.Set(4, 4);
			this->m_pTexture = fontTexGen.Generate(pFont, charList);
			this->m_pTexture->SetFilters(TextureFilter::Linear, TextureFilter::Linear, TextureFilter::Linear);
			this->m_pTextGen = new CB::Tools::CTextMeshGenerator(charList);
		}

		{
			using namespace CB::Graphic;

			CB::Collection::CList<CVertexElement>	els;
			els.Add(CVertexElement(0, SHADER_PARAM_POSITION, VertexType::Float, 3, 0));
			els.Add(CVertexElement(1, SHADER_PARAM_TEXCOORD, VertexType::Float, 2, 0));

			this->m_pVDeclaration = pDevice->CreateVertexDeclaration(this->m_pVShader, els);

			this->m_pVBuffer = pDevice->CreateBuffer(BufferType::Vertex, BufferUsage::Dynamic, BufferAccess::Write, 255 * sizeof(CB::Math::CVector3D));
			this->m_pTBuffer = pDevice->CreateBuffer(BufferType::Vertex, BufferUsage::Dynamic, BufferAccess::Write, 255 * sizeof(CB::Math::CVector2D));
			this->m_pIBuffer = pDevice->CreateBuffer(BufferType::Index, BufferUsage::Dynamic, BufferAccess::Write, 255 * sizeof(uint16));

			CBlendInstDesc instDesc(BlendOption::SourceAlpha, BlendOperation::Add, BlendOption::OneMinusSourceAlpha);
			CBlendStateDesc blendDesc(true, instDesc, instDesc, 0xFF);
			this->m_pBlendState = pDevice->CreateState(blendDesc).Cast<IDeviceState>();
		}

		this->SetTransform(CB::Math::CMatrix::GetIdentity());
		this->SetColor(CB::Math::CColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	void	CTextRenderer::SetTransform(const CB::Math::CMatrix& mTransform){
		this->m_pVShader->SetUniform(SHADER_TRANSFORM, mTransform);
	}

	void	CTextRenderer::SetColor(const CB::Math::CColor& Color){
		this->m_pFShader->SetUniform(SHADER_COLOR, Color.ToVector4D());
	}

	void	CTextRenderer::Print(const CB::CString& strText){
		CB::Tools::CMeshRawIVT mesh;
		this->m_pTextGen->Generate(strText, mesh);

		this->m_pVBuffer->LoadData(mesh.Vertices);
		this->m_pTBuffer->LoadData(mesh.TexCoords);
		this->m_pIBuffer->LoadData(mesh.Indices);

		this->BeginPrint();
		this->m_pDevice->RenderIndexed(mesh.uNumberOfPolygons);
		this->EndPrint();
	}

	void	CTextRenderer::BeginTextBlock(){
	}

	void	CTextRenderer::EndTextBlock(){
	}

	void	CTextRenderer::BeginPrint(){
		this->m_pFShader->SetSampler(SHADER_TEXTURE, this->m_pTexture.Cast<CB::Graphic::IBaseTexture>());

		this->m_pDevice->SetShader(this->m_pVShader);
		this->m_pDevice->SetShader(this->m_pFShader);
		this->m_pDevice->SetState(this->m_pBlendState);

		this->m_pDevice->SetVertexDeclaration(this->m_pVDeclaration);
		this->m_pDevice->SetVertexBuffer(0, this->m_pVBuffer);
		this->m_pDevice->SetVertexBuffer(1, this->m_pTBuffer);
		this->m_pDevice->SetIndexBuffer(this->m_pIBuffer);
	}

	void	CTextRenderer::EndPrint(){
		this->m_pDevice->FreeState(CB::Graphic::DeviceStateType::Blend);
		this->m_pDevice->FreeVertexBuffer(0);
		this->m_pDevice->FreeVertexBuffer(1);
		this->m_pDevice->FreeIndexBuffer();
		this->m_pDevice->FreeVertexDeclaration();
		this->m_pDevice->FreeShader(CB::Graphic::ShaderType::Vertex);
		this->m_pDevice->FreeShader(CB::Graphic::ShaderType::Fragment);
	}
}