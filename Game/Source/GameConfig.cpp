#include "GameConfig.h"

namespace T3{
	CGameConfig::CGameConfig() : 
		WindowPosition(0, 0),
		Resolution(640, 480),
		DebugMode(false),
		AssetsDir(L"Assets/")
	{
	}
}