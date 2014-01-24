#pragma once

#include <Types.h>
#include <CBString.h>
#include <Logger.h>
#include <Timer.h>

#include "DriverManager.h"
#include "GameResult.h"

namespace T3{
	class CApplication : 
		public IDriverManager
	{
	private:
		const CB::CString	m_strCmdLine;

		CB::CRefPtr<CB::Log::CLogger>		m_pLogger;
		CB::CRefPtr<CB::Window::IDriver>	m_pWindowDriver;

	public:
		CApplication(const CB::CString& strCmdLine);
		~CApplication();

		const int32	Run();

		CB::CRefPtr<CB::Window::IManager>	CreateWindowManager() override;

	private:
		void	InitializeLogger();
		void	LoadDrivers();
	};
}