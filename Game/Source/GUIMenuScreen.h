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

		public:
			CMenuScreen(CMain& Main, const CB::CString& strTitle, const CB::Math::CVector2D& vSize, const uint32 uItemsPerPage);

			virtual void	Render() override;

			void	SetTitle(const CB::CString& strTitle);
			void	AddMenuButton( const CB::CString& strName, CB::Signals::CFunc<>& Action );

		private:
			void	RecalcItems();
		};
	}
}