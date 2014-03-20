#include "GUITextButton.h"
#include "GUIScreen.h"

namespace T3{
	namespace GUI{
		CTextButton::CTextButton(const CB::CString& strText) :
			CTextItem(strText),
			m_ColorValue(0.5f, 1.0f),
			m_bSelected(false)
		{
		}

		const ItemType	CTextButton::GetType() const{
			return ItemType::Button;
		}

		void	CTextButton::Render(){
			CTextItem::Render();
		}

		void	CTextButton::Update(const float32 fTD){
			if(this->m_pParent.IsNull())
				return;

			if(this->m_bSelected){
				this->m_ColorValue.Increment(fTD);
			}
			else{
				this->m_ColorValue.Decrement(fTD);
			}
		}

		void	CTextButton::ProcessEvent(const CEvent& Event){
			if(this->m_pParent.IsNull())
				return;

			switch (Event.Type)
			{
			case EventType::MouseMove:
				this->m_bSelected = this->m_Rect.Contains(this->m_pParent->GetCursorPos());
				break;

			case EventType::MouseClick:
				if(Event.Key == CB::Window::VirtualKey::LBUTTON && this->m_bSelected){
					this->OnClick();
				}
				break;

			case EventType::KeyPress:
				if(Event.Key == CB::Window::VirtualKey::RETURN && this->m_bSelected){
					this->OnClick();
				}
				break;

			default:
				break;
			}

			CTextItem::ProcessEvent(Event);
		}

		const CB::Math::CColor	CTextButton::GetTextColor() const{
			return this->m_Color * CB::Math::CColor(this->m_ColorValue.GetValue());
		}

		void	CTextButton::SetSelected(const bool bSet){
			this->m_bSelected = bSet;
		}

		const bool	CTextButton::IsSelected() const{
			return this->m_bSelected;
		}
	}
}