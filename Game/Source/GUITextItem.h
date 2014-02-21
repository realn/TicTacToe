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

		public:
			CTextItem(CB::CRefPtr<CScreen> Screen, const CB::CString& strText);

			virtual const ItemType	GetType() const override;

			virtual void	Render() override;
			virtual void	Update(const float32 fTD) override;
		};
	}
}