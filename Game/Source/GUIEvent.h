#pragma once

#include <WindowDriver.h>
#include <Math_Vector.h>

namespace T3{
	namespace GUI{
		enum class EventType{
			MouseMove,
			MouseClick,
			CharPress,
			KeyPress,
		};

		class CEvent{
		public:
			EventType	Type;
			
			CB::Math::CVector2D	Position;
			CB::Window::VirtualKey	Key;
			wchar	Character;
		};
	}
}