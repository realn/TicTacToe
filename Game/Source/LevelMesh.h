#pragma once

#include <GraphicDriver.h>
#include "LevelFieldMesh.h"

namespace T3{
	class CLevelMesh{
	private:
		CB::CRefPtr<CB::Graphic::ITexture2D>	m_pTexture;
		CB::CRefPtr<CB::Graphic::IBuffer>		m_pBuffer;
		uint32	m_uNumberOfPolygons;

	public:
		CLevelMesh(CB::CRefPtr<CB::Graphic::IDevice> pDevice, uint32 uMaxLength, CB::CRefPtr<CB::Graphic::ITexture2D> pTexture);

		void	Set(const CB::Collection::ICountable<CLevelFieldMesh>& Fields);

		CB::CRefPtr<CB::Graphic::IBaseTexture>	GetTexture() const;
		CB::CRefPtr<CB::Graphic::IBuffer>		GetBuffer() const;
		const uint32							GetNumberOfPolygons() const;
	};
}