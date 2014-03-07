#pragma once

#include <Signals_Slots.h>

#include "GUITextItem.h"

namespace T3{
	namespace GUI{
		class CTextButton :
			public CTextItem
		{
		protected:
			CLinear	m_ColorValue;

		public:
			CTextButton(const CB::CString& strText);

			virtual const ItemType	GetType() const override;

			virtual void	Render() override;
			virtual void	Update(const float32 fTD) override;

			virtual void	ProcessEvent(const CEvent& Event) override;

			CB::Signals::CSlot<> OnClick;

		protected:
			virtual const CB::Math::CColor	GetTextColor() const override;
		};
	}
}