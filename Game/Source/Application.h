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
		CB::CRefPtr<CB::Graphic::IDriver>	m_pGraphicDriver;

	public:
		CApplication(const CB::CString& strCmdLine);
		~CApplication();

		const int32	Run();

		CB::CRefPtr<CB::Window::IManager>	CreateWindowManager() override;
		CB::CRefPtr<CB::Graphic::IManager>	CreateGraphicManager(CB::CRefPtr<CB::Window::IManager> pWindowManager) override;

	private:
		void	InitializeLogger();
		void	LoadDrivers();
	};
}