#pragma once

#include <Math_Point.h>
#include <Math_Size.h>

namespace T3{
	class CGameConfig{
	public:
		CB::Math::CPoint	WindowPosition;
		CB::Math::CSize		Resolution;

		CB::CString	AssetsDir;
		bool		DebugMode;

		CGameConfig();
	};
}