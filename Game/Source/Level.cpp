#include "Level.h"

namespace T3{
	const uint32 FIELD_SIZE_X = 3;
	const uint32 FIELD_SIZE_Y = 3;

	const CB::CString	TEXTURE_FRAME = L"frame";
	const CB::CString	TEXTURE_CIRCLE = L"circle";
	const CB::CString	TEXTURE_CROSS = L"corss";

	CLevel::CLevel(CB::CRefPtr<CB::Graphic::IDevice> pDevice, const CB::Math::CVector2D& vSize, CTextureManager& TexMng, CShaderManager& ShdMng) :
		m_vSize(vSize)
	{
		auto fieldSize = this->m_vSize / CB::Math::CVector2D((float32)FIELD_SIZE_X, (float32)FIELD_SIZE_Y);

		for(uint32 y = 0; y < FIELD_SIZE_Y; y++){
			for(uint32 x = 0; x < FIELD_SIZE_X; x++){
				uint32 uIndex = y * FIELD_SIZE_X + x;
				CField& field = this->m_Fields[uIndex];

				field.uIndex = uIndex;
				field.Rect.Position = fieldSize * CB::Math::CVector2D((float32)x, (float32)y);
				field.Rect.Size = fieldSize;
			}
		}

	}
}