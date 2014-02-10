#include "TextRenderer.h"
#include <IO_File.h>
#include <IO_Path.h>

namespace T3{
	const CB::CString	TEXT_SHADER = L"TextShader";
	const CB::CString	TEXT_FONT = L"Exo2.0-Regular.otf";

	CTextRenderer::CTextRenderer(CB::CRefPtr<CB::Graphic::IDevice> pDevice, CShaderManager& ShdMng, const CB::CString& strAssetsDIr) :
		m_pVShader(ShdMng.Load(TEXT_SHADER, CB::Graphic::ShaderType::Vertex)),
		m_pFShader(ShdMng.Load(TEXT_SHADER, CB::Graphic::ShaderType::Fragment))
	{
		auto strPath = CB::IO::Path::Combine(strAssetsDIr, TEXT_FONT);
		auto pFontStream = CB::IO::File::Open(strPath).Cast<CB::IO::IStream>();

		auto pFontMng = CB::Font::CManager::Create();
		auto pFont = pFontMng->Load(pFontStream);

		{
			CB::Collection::CList<CB::Tools::CFontCharDesc> charList;
			CB::Tools::CFontTextureGenerator fontTexGen(pDevice);
			this->m_pTexture = fontTexGen.Generate(pFont, charList);
			this->m_pTextGen = new CB::Tools::CTextMeshGenerator(charList);
		}

		{
			using namespace CB::Graphic;
			this->m_pVBuffer = pDevice->CreateBuffer(BufferType::Vertex, BufferUsage::Dynamic, BufferAccess::Write, 0);
			this->m_pTBuffer = pDevice->CreateBuffer(BufferType::Vertex, BufferUsage::Dynamic, BufferAccess::Write, 0);
			this->m_pIBuffer = pDevice->CreateBuffer(BufferType::Index, BufferUsage::Dynamic, BufferAccess::Write, 0);

			CBlendInstDesc instDesc(BlendOption::SourceAlpha, BlendOperation::Add, BlendOption::OneMinusSourceAlpha);
			CBlendStateDesc blendDesc(true, instDesc, instDesc, 0xFF);
			this->m_pBlendState = pDevice->CreateState(blendDesc).Cast<IDeviceState>();
		}
	}
}