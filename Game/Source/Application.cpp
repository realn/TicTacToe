#include "Application.h"
#include "Game.h"
#include "GameEnv.h"

#include <IO_File.h>
#include <MessageBox.h>
#include <CBStringEx.h>

namespace T3{
	CApplication::CApplication(const CB::CString& strAppName, const CB::CString& strCmdLine){
		this->m_Config.AppName = strAppName;
		CompileCmdLineArgs(strCmdLine, this->m_strCmdArgs);
		
		uint32 uIndex = 0;
		if(CB::Collection::TryFind(this->m_strCmdArgs, CB::CString(L"-config"), uIndex) && uIndex + 1 < this->m_strCmdArgs.GetLength()){
			this->m_strConfigPath = this->m_strCmdArgs[uIndex + 1];
		}
		else{
			this->m_strConfigPath = L"config.xml";
		}

		this->LoadConfig();

		if(CB::Collection::TryFind(m_strCmdArgs, CB::CString(L"-assets"), uIndex) && uIndex + 1 < m_strCmdArgs.GetLength()){
			this->m_Config.AssetsDir = this->m_strCmdArgs[uIndex + 1];
		}

		if(strCmdLine.Find(L"-debug")){
			this->m_Config.DebugMode = true;
		}

		this->InitializeLogger();
		this->LoadDrivers();
	}

	CApplication::~CApplication(){
	}

	const int32	CApplication::Run(){
		try{
			GameResult result;
			do{
				CGameEnv	env(*this, this->m_Config);
				CGame		game(this->m_strCmdArgs, env, this->m_Config);

				result = game.MainLoop();
				switch (result)
				{
				case T3::GameResult::ReloadDrivers:
					this->LoadDrivers();
					break;

				default:
					break;
				}
			}
			while(result != GameResult::Shutdown);
		}
		catch(CB::Exception::CException& Exception){
			CB::Log::Write(Exception, CB::Log::LogLevel::Fatal);
			CB::Message::Show(Exception, CB::Message::Icon::Error);

			return -1;
		}

		return 0;
	}

	CB::CRefPtr<CB::Window::IDriver>	CApplication::GetWindowDriver() const{
		return this->m_pWindowDriver;
	}

	CB::CRefPtr<CB::Graphic::IDriver>	CApplication::GetGraphicDriver() const{
		return this->m_pGraphicDriver;
	}

	void	CApplication::LoadConfig(){
	}
	
	void	CApplication::SaveConfig(){
	}

	void	CApplication::InitializeLogger(){
		this->m_pLogger = CB::Log::CLogger::GetInstance();

		auto pFileStream = CB::IO::File::Open(L"main.log", CB::IO::File::AccessType::WriteOnly, CB::IO::File::OpenAction::AlwaysCreate);
		this->m_pLogger->AddStream(pFileStream.Cast<CB::IO::IStream>());

		if(this->m_Config.DebugMode){
			this->m_pLogger->SetDebugMode(true);
		}
	}

	void	CApplication::LoadDrivers(){
		this->m_pWindowDriver = CB::Window::LoadDriver(L"MSWindowDriver");
		this->m_pGraphicDriver = CB::Graphic::LoadDriver(L"OGLGraphicDriver");
	}

	void	CApplication::CompileCmdLineArgs(const CB::CString& strCmdLine, CB::Collection::CList<CB::CString>& cmdArgsOut){
		CB::Collection::CStringList whiteList;
		whiteList.Add(L" ");
		whiteList.Add(L"\t");

		uint32 uEndPos = 0;
		uint32 uStartPos = 0;
		CB::CString strValue;
		while(CB::String::FindNot(strCmdLine, whiteList, uEndPos, uStartPos)){
			if(strCmdLine[uStartPos] == L'\"'){
				uStartPos++;
				if(strCmdLine.Find(L"\"", uStartPos + 1, uEndPos)){
					strValue = strCmdLine.SubStringIndexed(uStartPos, uEndPos);
					uEndPos++;
				}
				else{
					return;
				}
			}
			else{
				if(!CB::String::Find(strCmdLine, whiteList, uStartPos, uEndPos)){
					uEndPos = strCmdLine.GetLength();
				}
				strValue = strCmdLine.SubStringIndexed(uStartPos, uEndPos);
			}
			cmdArgsOut.Add(strValue);
		}
	}
}