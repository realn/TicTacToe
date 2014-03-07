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
			CB::Math::CColor	m_Color;
			CB::CString			m_strText;

		public:
			CTextItem(const CB::CString& strText);

			virtual const ItemType	GetType() const override;

			virtual void	Render() override;
			virtual void	Update(const float32 fTD) override;

			void	SetText(const CB::CString& strText);
			const CB::CString	GetText() const;

			void	SetColor(const CB::Math::CColor& Color);
			const CB::Math::CColor	GetColor() const;

		protected:
			virtual const CB::Math::CColor		GetTextColor() const;
			virtual const CB::Math::CVector2D	GetTextScale() const;
			virtual const CB::Math::CMatrix		GetTextTransform() const;
		};
	}
}