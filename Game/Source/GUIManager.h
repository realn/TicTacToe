#pragma once

#include <Types.h>
#include <CBString.h>
#include <Tools_Font.h>

namespace T3{
	namespace GUI{
		class CManager{
		private:
			CB::Tools::CTextMeshGenerator m_TextGen;

			CB::CRefPtr<CB::Graphic::IShader>		m_pVShader;
			CB::CRefPtr<CB::Graphic::IShader>		m_pFShader;
			CB::CRefPtr<CB::Graphic::IDeviceState>	m_pBlendState;

		public:
			CManager(const CB::CString& strFontAsset, const CB::CString& strVShaderAsset, const CB::CString& strFShaderAsset);

			
		};
	}
}