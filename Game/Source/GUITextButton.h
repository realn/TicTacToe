#pragma once

#include <Signals_Slots.h>

#include "GUITextItem.h"
#include "GUISelectableItem.h"

namespace T3{
	namespace GUI{
		class CTextButton :
			public CTextItem,
			public ISelectableItem
		{
		protected:
			CLinear	m_ColorValue;
			bool	m_bSelected;

		public:
			CTextButton(const CB::CString& strText);

			virtual const ItemType	GetType() const override;

			virtual void	Render() override;
			virtual void	Update(const float32 fTD) override;

			virtual void	ProcessEvent(const CEvent& Event) override;

			virtual void	SetSelected(const bool bSet) override;
			virtual const bool	IsSelected() const override;

			CB::Signals::CSlot<> OnClick;

		protected:
			virtual const CB::Math::CColor	GetTextColor() const override;
		};
	}
}