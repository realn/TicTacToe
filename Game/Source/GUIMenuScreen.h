#pragma once

#include <Signals_Signal.h>

#include "GUIScreen.h"
#include "GUITextItem.h"

namespace T3{
	namespace GUI{
		class CMenuScreen :
			public CScreen
		{
		protected:
			const uint32			m_uItemsPerPage;
			CB::Math::CVector2D		m_vPercMargin;
			CB::Math::CVector2D		m_vPercPardding;
			float32					m_fPercTitleSize;
			CB::CRefPtr<CTextItem>	m_pTitleItem;
			CB::Collection::CList<CB::CRefPtr<GUI::IItem>>	m_MenuItemList;
			TransitionMode			m_uTransitionMode;
			CLinear					m_Linear;

		public:
			CMenuScreen(CMain& Main, const CB::CString& strTitle, const CB::Math::CVector2D& vSize, const uint32 uItemsPerPage);

			virtual void	Render() override;
			virtual void	Update(const float32 fTD) override;

			void	SetTitle(const CB::CString& strTitle);
			void	AddMenuButton( const CB::CString& strName, CB::Signals::CFunc<>& Action );

			virtual void	SetTransitionMode(const TransitionMode uMode) override;
			virtual void	ProcessEvent(const CEvent& Event) override;

			virtual const TransitionMode	GetTransitionMode() const override;
			virtual const CB::Math::CMatrix	GetTransform() const override;

		private:
			void	RecalcItems();
		};
	}
}