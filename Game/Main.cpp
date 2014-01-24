#include "Application.h"

int32 __stdcall	WinMain(void* pInstance, void* pPrevInstance, int8* szCmdLine, int32 uShowCmd){
	T3::CApplication app(L"");

	return app.Run();
}