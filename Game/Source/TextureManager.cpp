#include "TextureManager.h"
#include <IO_Directory.h>
#include <IO_Path.h>
#include <IO_File.h>
#include <IO_Image.h>
#include <Logger.h>

namespace T3{
	const CB::CString g_strDefaultTexture = L"Invalid";

	CTextureManager::CTextureManager(CB::CRefPtr<CB::Graphic::IDevice> pDevice, const CB::CString& strTextureDir) :
		m_strTextureDir(strTextureDir),
		m_pDevice(pDevice)
	{
		CB::Log::Write(L"Texture Manager Initialization.");

		this->m_strExt.Add(L"bmp");
		this->m_strExt.Add(L"png");
		this->m_strExt.Add(L"tga");
		this->m_strExt.Add(L"jpg");
		this->m_strExt.Add(L"jpeg");

		CB::CString strDefault;
		this->FindFile(g_strDefaultTexture, strDefault);
		CB::Log::Write(L"Loading texture " + strDefault + L" as default.");
		this->m_pDefaultTexture = this->LoadTexture(strDefault);
	}

	CB::CRefPtr<CB::Graphic::ITexture2D>	CTextureManager::Load(const CB::CString& strAssetName){
		CB::CRefPtr<CB::Graphic::ITexture2D> pTexture;
		if(this->m_pTextureList.Find(strAssetName, pTexture)){
			return pTexture;
		}
		CB::Log::Write(L"Texture load: " + strAssetName);
		CB::CString strFilename;
		if(!this->FindFile(strAssetName, strFilename)){
			CB::Log::Write(L"Asset not found, returning default.");
			return this->m_pDefaultTexture;
		}

		pTexture = this->LoadTexture(strFilename);
		this->m_pTextureList.Add(strAssetName, pTexture);

		return pTexture;
	}

	void	CTextureManager::PurgeUnused(){
		for(auto En = this->m_pTextureList.GetEnumerator(); En.IsValid(); En.Next()){
			if(En.GetValue()->GetRefCount() == 1){
				this->m_pTextureList.Remove(En);
			}
		}
	}

	const bool	CTextureManager::FindFile(const CB::CString& strAssetName, CB::CString& strPathOut){
		for(uint32 uIndex = 0; uIndex < this->m_strExt.GetLength(); uIndex++){
			auto strFilename = CB::IO::Path::CombineFilename(strAssetName, this->m_strExt[uIndex]);
			strPathOut = CB::IO::Path::Combine(this->m_strTextureDir, strFilename);
			if(CB::IO::File::Exists(strPathOut)){
				return true;
			}
		}
		return false;
	}

	CB::CRefPtr<CB::Graphic::ITexture2D>	CTextureManager::LoadTexture(const CB::CString& strPath){
		auto pStream = CB::IO::File::Open(strPath).Cast<CB::IO::IStream>();
		CB::IO::CImage img;

		img.ReadFromStream(pStream);
		img.Convert(CB::IO::Image::BitFormat::f32Bit);

		CB::Graphic::BufferFormat uInputFormat;
		switch (img.GetColorFormat())
		{
		case CB::IO::Image::ColorFormat::RGB:
			uInputFormat = CB::Graphic::BufferFormat::B8G8R8;
			break;

		case CB::IO::Image::ColorFormat::RGBA:
			uInputFormat = CB::Graphic::BufferFormat::B8G8R8A8;
			break;

		default:
			return this->m_pDefaultTexture;
		}

		CB::Collection::CList<byte> data;
		img.GetPixels(data);

		try{
			return this->m_pDevice->CreateTexture2D(img.GetSize(), 
				CB::Graphic::BufferUsage::Static, 
				CB::Graphic::BufferAccess::Write, 
				CB::Graphic::BufferFormat::R8G8B8A8, 
				uInputFormat, data);
		}
		catch(CB::Exception::CException& Exception){
			CB::Log::Write(Exception);
			CB::Log::Write(L"Texture load of " + strPath + L" failed. Returning default.");
			return this->m_pDefaultTexture;
		}
	}
}