#pragma once

#include <GraphicDriver.h>
#include <Math_RectangleF.h>

#include "GUIItem.h"
#include "Linear.h"

namespace T3{
	namespace GUI{
		class CTextItem :
			public IItem
		{
		protected:
			CB::CString	m_strText;
			CLinear	m_ColorValue;

		public:
			CTextItem(CB::CRefPtr<CScreen> Screen, const CB::CString& strText);

			virtual const ItemType	GetType() const override;

			virtual void	Render() override;
			virtual void	Update(const float32 fTD) override;
		};
	}
}