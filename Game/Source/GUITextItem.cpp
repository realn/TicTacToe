#include "GUITextItem.h"
#include "GUIScreen.h"
#include "GUIMain.h"
#include "TextRenderer.h"

namespace T3{
	namespace GUI{
		CTextItem::CTextItem(const CB::CString& strText) :
			m_strText(strText),
			m_Color(1.0f)
		{
			
		}

		const ItemType	CTextItem::GetType() const{
			return ItemType::Text;
		}

		void	CTextItem::Render(){
			if(this->m_pParent.IsNull())
				return;

			this->GetTR().SetColor(this->GetTextColor());
			this->GetTR().SetTransform(this->GetTextTransform());
			this->GetTR().SetScale(this->GetTextScale());
			this->GetTR().Print(this->m_strText);
			this->m_pParent->GetParent()->SetUpRender();
		}

		void	CTextItem::Update(const float32 fTD){
		}

		void	CTextItem::SetText(const CB::CString& strText){
			this->m_strText = strText;
		}

		const CB::CString	CTextItem::GetText() const{
			return this->m_strText;
		}

		void	CTextItem::SetColor(const CB::Math::CColor& Color){
			this->m_Color = Color;
		}

		const CB::Math::CColor	CTextItem::GetColor() const{
			return this->m_Color;
		}

		const CB::Math::CColor	CTextItem::GetTextColor() const{
			return this->m_Color;
		}

		const CB::Math::CVector2D	CTextItem::GetTextScale() const{
			return this->m_Rect.Size.Y;
		}

		const CB::Math::CMatrix	CTextItem::GetTextTransform() const{
			CB::Math::CVector3D vPos(this->m_Rect.Position, 0.0f);
			return this->m_pParent->GetTransform() * CB::Math::CMatrix::GetTranslation(vPos);
		}
	}
}