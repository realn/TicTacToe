#pragma once

#include <GraphicDriver.h>

namespace T3{
	class CShaderManager{
	private:
		static const CB::CString	VERTEX_SHADER_MAIN;
		static const CB::CString	FRAGMENT_SHADER_MAIN;

		class CShaderID{
		public:
			CB::CString	Name;
			CB::Graphic::ShaderType	Type;

			CShaderID();
			CShaderID(const CShaderID& ID);
			CShaderID(const CB::CString& strAssetName, const CB::Graphic::ShaderType uType);

			const bool	operator==(const CShaderID& Shader);
		};

		const CB::CString					m_strShaderDir;
		const CB::Graphic::ShaderVersion	m_uVersion;
		CB::CRefPtr<CB::Graphic::IDevice>	m_pDevice;
		CB::Collection::CDictionary<CShaderID, CB::CRefPtr<CB::Graphic::IShader>>	m_pShaderList;

	public:
		CShaderManager(CB::CRefPtr<CB::Graphic::IDevice> pDevice, const CB::CString& strShaderDir, const CB::Graphic::ShaderVersion uVersion);

		CB::CRefPtr<CB::Graphic::IShader>	Load(const CB::CString& strAssetName, const CB::Graphic::ShaderType uType);

	private:
		const bool	FindFile(const CB::CString& strAssetName, CB::CString& strFilenameOut);
		const CB::CString	GetFileSource(const CB::CString& strFilename);
		CB::CRefPtr<CB::Graphic::IShader>	LoadShader(const CB::CString& strFilename, const CB::Graphic::ShaderType uType);
	};
}