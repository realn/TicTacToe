#include "GUIMenuScreen.h"
#include "GUITextButton.h"
#include "GUIMain.h"

namespace T3{
	namespace GUI{
		CMenuScreen::CMenuScreen(CMain& Main, const CB::CString& strTitle, const CB::Math::CVector2D& vSize, const uint32 uItemPerPage) :
			CScreen(Main, vSize),
			m_uItemsPerPage(uItemPerPage),
			m_vPercMargin(0.05f, 0.05f),
			m_vPercPardding(0.1f, 0.005f),
			m_fPercTitleSize(0.2f),
			m_uTransitionMode(TransitionMode::None),
			m_Linear(0.0f, 1.0f, 2.0f)
		{
			this->m_pTitleItem = new CTextItem( strTitle );
			CScreen::AddItem(this->m_pTitleItem.Get());
			this->RecalcItems();
		}

		void	CMenuScreen::Render(){
			this->m_pParent->GetBackground().Render(CB::Math::CColor(1.0f, 1.0f, 1.0f, 0.5f));
			CScreen::Render();
		}

		void	CMenuScreen::Update(const float fTD){
			switch (this->m_uTransitionMode)
			{
			case TransitionMode::Show:
				this->m_Linear.Decrement( fTD );
				if(this->m_Linear.IsMin()){
					this->m_uTransitionMode = TransitionMode::None;
				}
				break;

			case TransitionMode::Hide:
				this->m_Linear.Increment(fTD);
				if(this->m_Linear.IsMax()){
					this->m_uTransitionMode = TransitionMode::None;
				}
				break;

			default:
				CScreen::Update(fTD);
				break;
			}
		}

		void	CMenuScreen::SetTitle(const CB::CString& strTitle){
			this->m_pTitleItem->SetText(strTitle);
		}

		void	CMenuScreen::AddMenuButton(const CB::CString& strName, CB::Signals::CFunc<>& Action){
			CB::CRefPtr<CTextButton> pButton = new CTextButton(strName);
			pButton->OnClick += Action;
			if(CScreen::AddItem(pButton.Cast<GUI::IItem>())){
				this->m_MenuItemList.Add(pButton.Cast<GUI::IItem>());
				this->RecalcItems();
			}
		}

		void	CMenuScreen::RecalcItems(){
			auto vMargin = this->m_vSize * this->m_vPercMargin;
			auto vPadding = this->m_vSize * this->m_vPercPardding;

			float32 fTitleSizeY = this->m_vSize.Y * this->m_fPercTitleSize;
			auto vTitlePos = CB::Math::CVector2D(vMargin.X, this->m_vSize.Y - vMargin.Y - fTitleSizeY);
			float32 fListTop = vTitlePos.Y - vMargin.Y;
			float32 fListSizeY = fListTop - vMargin.Y;
			float32 fMenuSizeX = this->m_vSize.X - vMargin.X * 2.0f;
			float32 fItemSizeY = fListSizeY / (float32)this->m_uItemsPerPage;

			CB::Math::CVector2D vPaddedItemSize = CB::Math::CVector2D(fMenuSizeX, fItemSizeY) - vPadding * 2.0f;

			{
				CB::Math::CRectangleF32 rect(vTitlePos, fMenuSizeX, fTitleSizeY);				
				this->m_pTitleItem->SetRect( rect );				
			}

			for(uint32 uIndex = 0; uIndex < this->m_MenuItemList.GetLength(); uIndex++){
				CB::Math::CVector2D vPos;
				vPos.X = vMargin.X + vPadding.X;
				vPos.Y = fListTop - (uIndex + 1) * fItemSizeY + vPadding.Y;

				CB::Math::CRectangleF32 rect(vPos, vPaddedItemSize);

				this->m_MenuItemList[uIndex]->SetRect(rect);
			}
		}

		void	CMenuScreen::SetTransitionMode(const TransitionMode uMode){
			this->m_uTransitionMode = uMode;
			switch (this->m_uTransitionMode)
			{
			case TransitionMode::Show:
				this->m_Linear.Fill();
				break;

			case TransitionMode::Hide:
				this->m_Linear.Reset();
				break;

			default:
				break;
			}
		}

		void	CMenuScreen::ProcessEvent(const CEvent& Event){
			if(this->m_uTransitionMode != TransitionMode::None)
				return;
			CScreen::ProcessEvent(Event);
		}

		const TransitionMode	CMenuScreen::GetTransitionMode() const{
			return this->m_uTransitionMode;
		}

		const CB::Math::CMatrix	CMenuScreen::GetTransform() const{
			if(this->m_uTransitionMode == TransitionMode::None){
				return CScreen::GetTransform();
			}
			else{
				auto mMove = CB::Math::CMatrix::GetTranslation(this->m_vSize.X * this->m_Linear.GetValue(), 0.0f, 0.0f);
				return CScreen::GetTransform() * mMove;
			}
		}
	}
}