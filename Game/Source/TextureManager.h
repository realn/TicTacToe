#pragma once

#include <GraphicDriver.h>
#include <CBStringEx.h>

namespace T3{
	class CTextureManager{
	private:
		const CB::CString	m_strTextureDir;
		CB::CRefPtr<CB::Graphic::IDevice>	m_pDevice;
		CB::CRefPtr<CB::Graphic::ITexture2D>	m_pDefaultTexture;
		CB::Collection::CDictionary<CB::CString, CB::CRefPtr<CB::Graphic::ITexture2D>> m_pTextureList;
		CB::Collection::CStringList	m_strExt;

	public:
		CTextureManager(CB::CRefPtr<CB::Graphic::IDevice> pDevice, const CB::CString& strTextureDir);

		CB::CRefPtr<CB::Graphic::ITexture2D>	Load(const CB::CString& strAssetName);

		void	PurgeUnused();

	private:
		const bool	FindFile(const CB::CString& strAssetName, CB::CString& strPathOut);
		CB::CRefPtr<CB::Graphic::ITexture2D>	LoadTexture(const CB::CString& strPath);
	};
}