#pragma once

#include <Math_Vector.h>
#include <Math_Color.h>

namespace T3{
	 namespace GUI{
		 class CVertex{
		 public:
			 CB::Math::CVector3D	Position;
			 CB::Math::CVector2D	TexCoord;

			 CVertex(const float32 x, const float32 y, const float32 z, const float32 s, const float32 t) :
				 Position(x, y, z),
				 TexCoord(s, t)
			 {}
		 };
	 }
}