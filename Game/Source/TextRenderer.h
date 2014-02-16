#pragma once

#include <GraphicDriver.h>
#include <Tools_Font.h>
#include <SmartPointers_AutoPtr.h>

#include "ShaderManager.h"

namespace T3{
	class CTextRenderer{
	private:
		CB::CRefPtr<CB::Graphic::IDevice>		m_pDevice;
		CB::CRefPtr<CB::Graphic::IVertexDeclaration>	m_pVDeclaration;
		CB::CRefPtr<CB::Graphic::IShader>		m_pVShader;
		CB::CRefPtr<CB::Graphic::IShader>		m_pFShader;
		CB::CRefPtr<CB::Graphic::IBuffer>		m_pVBuffer;
		CB::CRefPtr<CB::Graphic::IBuffer>		m_pTBuffer;
		CB::CRefPtr<CB::Graphic::IBuffer>		m_pIBuffer;
		CB::CRefPtr<CB::Graphic::IDeviceState>	m_pBlendState;
		CB::CRefPtr<CB::Graphic::ITexture2D>	m_pTexture;
		CB::CAutoPtr<CB::Tools::CTextMeshGenerator>		m_pTextGen;

	public:
		CTextRenderer(CB::CRefPtr<CB::Graphic::IDevice> pDevice, CShaderManager& ShdMng, const CB::CString& strAssetsDir);

		void	SetTransform(const CB::Math::CMatrix& mTransform);
		void	SetColor(const CB::Math::CColor& Color);
		void	Print(const CB::CString& strText);

		void	BeginTextBlock();
		void	EndTextBlock();

	private:
		void	BeginPrint();
		void	EndPrint();
	};
}