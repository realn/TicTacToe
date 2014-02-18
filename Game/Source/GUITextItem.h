#pragma once

#include <GraphicDriver.h>
#include <Math_RectangleF.h>

#include "GUIItem.h"

namespace T3{
	namespace GUI{
		class CTextItem :
			public IItem
		{
		protected:
			CB::CString	m_strText;
			CB::Math::CRectangleF32	m_Rect;

			CB::CRefPtr<CB::Graphic::IBuffer>		m_pVBuffer;
			CB::CRefPtr<CB::Graphic::IBuffer>		m_pTBuffer;
			CB::CRefPtr<CB::Graphic::IBuffer>		m_pIBuffer;
			CB::CRefPtr<CB::Graphic::IDeviceState>	m_pBState;

		public:
			CTextItem(CScreen& Screen, const CB::CString& strText);

			virtual const ItemType	GetType() const override;

			virtual void	Render() override;
			virtual void	Update(const float32 fTD) override;
		};
	}
}