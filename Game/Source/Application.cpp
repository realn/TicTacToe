#include "Application.h"
#include "Game.h"

#include <IO_File.h>
#include <MessageBox.h>

namespace T3{
	CApplication::CApplication(const CB::CString& strCmdLine) :
		m_strCmdLine(strCmdLine)
	{
		this->InitializeLogger();
		this->LoadDrivers();
	}

	CApplication::~CApplication(){
	}

	const int32	CApplication::Run(){
		try{
			GameResult result;
			do{
				CGame	game(this->m_strCmdLine, *this);

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

	void	CApplication::InitializeLogger(){
		this->m_pLogger = CB::Log::CLogger::GetInstance();

		auto pFileStream = CB::IO::File::Open(L"main.log", CB::IO::File::AccessType::WriteOnly, CB::IO::File::OpenAction::AlwaysCreate);
		this->m_pLogger->AddStream(pFileStream.Cast<CB::IO::IStream>());

		if(this->m_strCmdLine.Find(L"-debug")){
			this->m_pLogger->SetDebugMode(true);
		}
	}

	void	CApplication::LoadDrivers(){
		this->m_pWindowDriver = CB::Window::LoadDriver(L"MSWindowDriver");
		this->m_pGraphicDriver = CB::Graphic::LoadDriver(L"OGLGraphicDriver");
	}

	CB::CRefPtr<CB::Window::IManager>	CApplication::CreateWindowManager(){
		return this->m_pWindowDriver->CreateManager();
	}

	CB::CRefPtr<CB::Graphic::IManager>	CApplication::CreateGraphicManager(CB::CRefPtr<CB::Window::IManager> pWindowManager){
		return this->m_pGraphicDriver->CreateManager(pWindowManager);
	}
}