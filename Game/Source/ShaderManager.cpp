#include "ShaderManager.h"
#include <CBStringEx.h>
#include <IO_Path.h>
#include <IO_File.h>
#include <IO_TextReader.h>

namespace T3{
	const CB::CString	CShaderManager::VERTEX_SHADER_MAIN = L"vmain";
	const CB::CString	CShaderManager::FRAGMENT_SHADER_MAIN = L"fmain";

	CShaderManager::CShaderID::CShaderID() :
		Type(CB::Graphic::ShaderType::Vertex)
	{
	}

	CShaderManager::CShaderID::CShaderID(const CShaderManager::CShaderID& ID) :
		Name(ID.Name),
		Type(ID.Type)
	{
	}

	CShaderManager::CShaderID::CShaderID(const CB::CString& strAssetName, const CB::Graphic::ShaderType uType) : 
		Name(strAssetName),
		Type(uType)
	{
	}

	const bool	CShaderManager::CShaderID::operator==(const CShaderManager::CShaderID& ID){
		return this->Name == ID.Name && this->Type == ID.Type;
	}

	CShaderManager::CShaderManager(CB::CRefPtr<CB::Graphic::IDevice> pDevice, const CB::CString& strShaderDir, const CB::Graphic::ShaderVersion uVersion) :
		m_strShaderDir(strShaderDir),
		m_pDevice(pDevice),
		m_uVersion(uVersion)
	{

	}

	CB::CRefPtr<CB::Graphic::IShader>	CShaderManager::Load(const CB::CString& strAssetName, const CB::Graphic::ShaderType uType){
		CShaderID shaderId(strAssetName, uType);
		CB::CRefPtr<CB::Graphic::IShader> pShader;
		if(this->m_pShaderList.Find(shaderId, pShader)){
			return pShader;
		}

		CB::CString strFilename;
		if(!this->FindFile(strAssetName, strFilename)){
			CR_THROW(CB::String::Format(L"Failed to find shader file for asset {0}.", strAssetName));
		}

		pShader = this->LoadShader(strFilename, uType);
		this->m_pShaderList.Add(shaderId, pShader);
		
		return pShader;
	}

	const bool	CShaderManager::FindFile(const CB::CString& strAssetName, CB::CString& strFilename){
		CB::CString strFile = CB::IO::Path::CombineFilename(strAssetName, L"cg");
		CB::CString strPath = CB::IO::Path::Combine(this->m_strShaderDir, strFile);

		if(!CB::IO::File::Exists(strPath)){
			return false;
		}

		strFilename = strPath;
		return true;
	}

	const CB::CString	CShaderManager::GetFileSource(const CB::CString& strFilename){
		auto pStream = CB::IO::File::Open(strFilename).Cast<CB::IO::IStream>();
		CB::IO::CTextReader reader(pStream, CB::String::Encoding::ANSI);

		return reader.ReadText();
	}

	CB::CRefPtr<CB::Graphic::IShader>	CShaderManager::LoadShader(const CB::CString& strFilename, const CB::Graphic::ShaderType uType){
		auto strSource = this->GetFileSource(strFilename);
		CB::CString strMain;
		switch (uType)
		{
		case CB::Graphic::ShaderType::Vertex:	strMain = VERTEX_SHADER_MAIN;	break;
		case CB::Graphic::ShaderType::Fragment:	strMain = FRAGMENT_SHADER_MAIN;	break;
		default:
			CR_THROW(CB::String::Format(L"Unknown shader type {0}.", CB::String::ToString(uType)));
		}

		try{
			return this->m_pDevice->Compile(uType, this->m_uVersion, strSource, strMain);
		}
		catch(CB::Exception::CException& Exception){
			auto strLog = this->m_pDevice->GetLastCompilationLog();
			if(strLog.IsEmpty()){
				throw;
			}
			CR_RETHROW(strLog, Exception);
		}
	}
}