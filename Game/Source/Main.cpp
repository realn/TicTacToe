#include "Application.h"

int32 __stdcall	wWinMain(void* pInstance, void* pPrevInstance, wchar* szCmdLine, int32 uShowCmd){
	T3::CApplication app(L"TicTacToe", szCmdLine);

	return app.Run();
}