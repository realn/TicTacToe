#pragma once

#include <GraphicDriver.h>
#include <Math_RectangleF.h>

namespace T3{
	class CLevelFieldMesh{
	public:
		CB::Collection::CArray<CB::Math::CVector4D, 4>	vPosition;

		void	SetRect(const CB::Math::CRectangleF32& Rect);
		void	SetAlpha(const float32 fAlpha);
		void	AddToList(CB::Collection::CList<CB::Math::CVector4D>& List) const;
	};
}