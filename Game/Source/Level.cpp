#include "Level.h"

namespace T3{
	CLevel::CLevel(CB::CRefPtr<CB::Graphic::IDevice> pDevice, const CB::Math::CVector2D& vSize, CTextureManager& TexMng, CShaderManager& ShdMng) :
		m_vSize(vSize)
	{
		auto fieldSize = this->m_vSize / 3.0f;



	}
}