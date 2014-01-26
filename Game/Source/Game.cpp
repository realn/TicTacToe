#include "Game.h"
#include <Timer.h>
#include <Signals_Method.h>
#include "TextureManager.h"
#include <IO_Path.h>
#include <IO_File.h>
#include <IO_TextReader.h>

namespace T3{
	CGame::CGame(const CB::Collection::ICountable<CB::CString>& strArgs, CGameConfig& Config, IDriverManager& driverManager) :
		m_State(GameState::MainMenu),
		m_Config(Config)
	{
		uint32 uIndex = 0;
		if(CB::Collection::TryFind(strArgs, CB::CString(L"-assets"), uIndex) && uIndex + 1 < strArgs.GetLength()){
			this->m_Config.AssetsDir = strArgs[uIndex + 1];
		}

		this->m_pWindowManager = driverManager.CreateWindowManager();
		this->m_pMainWindow = this->m_pWindowManager->CreateWindow(L"TicTacToe", CB::Window::Style::Single, this->m_Config.Resolution, this->m_Config.WindowPosition);
		this->m_pMainWindow->OnClose += CB::Signals::CMethod<CGame, const bool, CB::CRefPtr<CB::Window::IWindow>>(this, &CGame::WindowClose);

		this->m_pGraphicManager = driverManager.CreateGraphicManager(this->m_pWindowManager);

		CB::Graphic::CDeviceDesc desc(
			this->m_pMainWindow, 
			CB::Graphic::CDisplayMode(this->m_Config.Resolution, 60, CB::Graphic::BufferFormat::R8G8B8A8),
			CB::Graphic::BufferFormat::D24S8,
			false
			);

		CB::Collection::CArray<CB::Graphic::FeatureLevel, 1> level;
		level[0] = CB::Graphic::FeatureLevel::Level_1;

		this->m_pGraphicDevice = this->m_pGraphicManager->GetDefaultAdapter()->CreateDevice(
			this->m_pMainWindow,
			desc,
			level
			);

		this->m_pTextureManager = new CTextureManager(m_pGraphicDevice, this->m_Config.AssetsDir);
		this->m_pMainWindow->SetVisible(true);

		CB::CString strSource;
		{
			auto pStream = CB::IO::File::Open(CB::IO::Path::Combine(this->m_Config.AssetsDir, L"BasicShader.cg")).Cast<CB::IO::IStream>();
			CB::IO::CTextReader reader(pStream, CB::String::Encoding::ANSI);

			strSource = reader.ReadText();
		}

		try{
			this->m_pVShader = this->m_pGraphicDevice->Compile(CB::Graphic::ShaderType::Vertex, CB::Graphic::ShaderVersion::ShaderModel_2, strSource, L"vmain");
			this->m_pFragment = this->m_pGraphicDevice->Compile(CB::Graphic::ShaderType::Fragment, CB::Graphic::ShaderVersion::ShaderModel_2, strSource, L"fmain");
		}
		catch(CB::Exception::CException& Exception){
			CR_RETHROW(this->m_pGraphicDevice->GetLastCompilationLog(), Exception);
		}

		CB::Collection::CList<CB::Graphic::CVertexElement> els;
		els.Add(CB::Graphic::CVertexElement(0, L"vinput.vPosition", 0, CB::Graphic::VertexType::Float, 3));
		els.Add(CB::Graphic::CVertexElement(1, L"vinput.vTexCoord", 0, CB::Graphic::VertexType::Float, 2));

		this->m_pVDecl = this->m_pGraphicDevice->CreateVertexDeclaration(this->m_pVShader, els);

		CB::Collection::CList<CB::Math::CVector3D> verts;
		CB::Collection::CList<CB::Math::CVector2D> tcoords;
		CB::Collection::CList<uint16>	inds;

		verts.Add(CB::Math::CVector3D(-1.0f, -1.0f, 0.0f));
		verts.Add(CB::Math::CVector3D(1.0f, -1.0f, 0.0f));
		verts.Add(CB::Math::CVector3D(1.0f, 1.0f, 0.0f));
		verts.Add(CB::Math::CVector3D(-1.0f, 1.0f, 0.0f));
		
		tcoords.Add(CB::Math::CVector2D(0.0f, 0.0f));
		tcoords.Add(CB::Math::CVector2D(1.0f, 0.0f));
		tcoords.Add(CB::Math::CVector2D(1.0f, 1.0f));
		tcoords.Add(CB::Math::CVector2D(0.0f, 1.0f));

		inds.Add(0);
		inds.Add(2);
		inds.Add(1);

		inds.Add(0);
		inds.Add(3);
		inds.Add(2);

		this->m_pVBuffer = this->m_pGraphicDevice->CreateBuffer(CB::Graphic::BufferType::Vertex, CB::Graphic::BufferUsage::Static, CB::Graphic::BufferAccess::Write, verts);
		this->m_pTBuffer = this->m_pGraphicDevice->CreateBuffer(CB::Graphic::BufferType::Vertex, CB::Graphic::BufferUsage::Static, CB::Graphic::BufferAccess::Write, tcoords);
		this->m_pIBuffer = this->m_pGraphicDevice->CreateBuffer(CB::Graphic::BufferType::Index, CB::Graphic::BufferUsage::Static, CB::Graphic::BufferAccess::Write, inds);

		this->m_pPointer = this->m_pTextureManager->Load(L"pointer");

		CB::Graphic::CBlendStateDesc blendDesc(true, 
			CB::Graphic::CBlendInstDesc(CB::Graphic::BlendOption::SourceAlpha, CB::Graphic::BlendOperation::Add, CB::Graphic::BlendOption::OneMinusSourceAlpha),
			CB::Graphic::CBlendInstDesc(CB::Graphic::BlendOption::SourceAlpha, CB::Graphic::BlendOperation::Add, CB::Graphic::BlendOption::OneMinusSourceAlpha),
			0xFF
			);
		this->m_pBlendState = this->m_pGraphicDevice->CreateState(blendDesc);
	}

	CGame::~CGame(){
		this->m_pMainWindow->OnClose -= CB::Signals::CMethod<CGame, const bool, CB::CRefPtr<CB::Window::IWindow>>(this, &CGame::WindowClose);
	}

	const GameResult	CGame::MainLoop(){
		CB::CTimer	timer;

		do{
			this->Render();

			timer.Update();
			this->m_pWindowManager->ProcessEvents();

			this->m_pGraphicDevice->Swap();
		}
		while(this->m_State != GameState::Shutdown);
		
		return GameResult::Shutdown;
	}

	const bool	CGame::WindowClose(CB::CRefPtr<CB::Window::IWindow> pWindow){
		this->m_State = GameState::Shutdown;

		return true;
	}

	void	CGame::Render(){
		this->m_pGraphicDevice->Clear(CB::Math::CColor(0.5f, 0.6f, 0.3f));
		this->m_pGraphicDevice->BeginRender();

		CB::Math::CMatrix	mModel = CB::Math::CMatrix::GetIdentity();
		CB::Math::CMatrix	mView = CB::Math::CMatrix::GetTranslation(0.0f, 0.0f, -1.0f);
		CB::Math::CMatrix	mProj = CB::Math::CMatrix::GetOrtho(4.0f, 4.0f, -2.0f, 2.0f);

		this->m_pGraphicDevice->SetShader(this->m_pVShader);
		this->m_pGraphicDevice->SetShader(this->m_pFragment);

		this->m_pVShader->SetUniform(L"mModelViewProj", mModel * mView * mProj);
		this->m_pFragment->SetSampler(L"texBase", this->m_pPointer.Cast<CB::Graphic::IBaseTexture>());

		this->m_pGraphicDevice->SetVertexDeclaration(this->m_pVDecl);
		this->m_pGraphicDevice->SetVertexBuffer(0, this->m_pVBuffer);
		this->m_pGraphicDevice->SetVertexBuffer(1, this->m_pTBuffer);
		this->m_pGraphicDevice->SetIndexBuffer(this->m_pIBuffer);

		this->m_pGraphicDevice->SetState(this->m_pBlendState.Cast<CB::Graphic::IDeviceState>());

		this->m_pGraphicDevice->RenderIndexed(2);

		this->m_pGraphicDevice->EndRender();
	}
}