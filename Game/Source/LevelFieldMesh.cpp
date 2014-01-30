#include "LevelFieldMesh.h"

namespace T3{
	void	CLevelFieldMesh::SetRect(const CB::Math::CRectangleF32& Rect){
		auto vPos = CB::Math::CVector4D(Rect.Position.X, Rect.Position.Y, 0.0f, 0.0f);
		auto vSize = Rect.Size;

		this->vPosition[0] = vPos + CB::Math::CVector4D(0.0f, 0.0f, 0.0f, this->vPosition[0].W);
		this->vPosition[1] = vPos + CB::Math::CVector4D(vSize.X, 0.0f, 0.0f, this->vPosition[1].W);
		this->vPosition[2] = vPos + CB::Math::CVector4D(vSize.X, vSize.Y, 0.0f, this->vPosition[2].W);
		this->vPosition[3] = vPos + CB::Math::CVector4D(0.0f, vSize.Y, 0.0f, this->vPosition[3].W);
	}

	void	CLevelFieldMesh::SetAlpha(const float32 fAlpha){
		for(uint32 i = 0; i < this->vPosition.GetLength(); i++){
			this->vPosition[i].W = fAlpha;
		}
	}

	void	CLevelFieldMesh::AddToList(CB::Collection::CList<CB::Math::CVector4D>& List) const{
		for(uint32 i = 0; i < this->vPosition.GetLength(); i++){
			List.Add(this->vPosition[i]);
		}
	}
}