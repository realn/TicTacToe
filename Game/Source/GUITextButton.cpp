#include "GUITextButton.h"
#include "GUIScreen.h"

namespace T3{
	namespace GUI{
		CTextButton::CTextButton(const CB::CString& strText) :
			CTextItem(strText),
			m_ColorValue(0.5f, 1.0f)
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

			if(this->m_Rect.Contains(this->m_pParent->GetCursorPos())){
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
			case EventType::MouseClick:
				if(this->m_Rect.Contains(this->m_pParent->GetCursorPos())){
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
	}
}