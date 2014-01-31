#pragma once

#include <Types.h>
#include <CBString.h>
#include <Logger.h>
#include <Timer.h>
#include <CBStringEx.h>

#include "DriverContainer.h"
#include "GameResult.h"
#include "GameConfig.h"

namespace T3{
	class CApplication : 
		public IDriverContainer
	{
	private:
		CB::Collection::CStringList	m_strCmdArgs;
		CB::CString		m_strConfigPath;
		CGameConfig		m_Config;

		CB::CRefPtr<CB::Log::CLogger>		m_pLogger;
		CB::CRefPtr<CB::Window::IDriver>	m_pWindowDriver;
		CB::CRefPtr<CB::Graphic::IDriver>	m_pGraphicDriver;

	public:
		CApplication(const CB::CString& strCmdLine);
		~CApplication();

		const int32	Run();

		CB::CRefPtr<CB::Window::IDriver>	GetWindowDriver() const override;
		CB::CRefPtr<CB::Graphic::IDriver>	GetGraphicDriver() const override;

	private:
		void	LoadConfig();
		void	SaveConfig();
		void	InitializeLogger();
		void	LoadDrivers();

		static void	CompileCmdLineArgs(const CB::CString& strCmdLine, CB::Collection::CList<CB::CString>& cmdArgsOut);
	};
}