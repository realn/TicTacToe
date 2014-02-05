#pragma once

#include <Types.h>
#include <GraphicDriver.h>

namespace T3{
	class CRectMesh{
	private:
		CB::CRefPtr<CB::Graphic::IVertexDeclaration>	m_pDeclaration;
		CB::CRefPtr<CB::Graphic::IBuffer>	m_pVBuffer;
		CB::CRefPtr<CB::Graphic::IBuffer>	m_pIBuffer;

	public:
		CRectMesh(CB::CRefPtr<CB::Graphic::IDevice> pDevice, const CB::Math::CVector2D& vSize, CB::CRefPtr<CB::Graphic::IShader> pVertexShader);

		void	BeginRender(CB::CRefPtr<CB::Graphic::IDevice> pDevice);
		void	Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice);
		void	EndRender(CB::CRefPtr<CB::Graphic::IDevice> pDevice);
	};
}