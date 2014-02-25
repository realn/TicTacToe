#include "GUITextItem.h"
#include "GUIScreen.h"
#include "GUIMain.h"
#include "TextRenderer.h"

namespace T3{
	namespace GUI{
		CTextItem::CTextItem(CB::CRefPtr<CScreen> pScreen, const CB::CString& strText) :
			IItem(pScreen),
			m_strText(strText),
			m_ColorValue(0.5f, 1.0f)
		{
			
		}

		const ItemType	CTextItem::GetType() const{
			return ItemType::Text;
		}

		void	CTextItem::Render(){
			CB::Math::CVector3D vPos(this->m_Rect.Position, 0.0f);
			auto mMatrix = this->m_pParent->GetTransform() * CB::Math::CMatrix::GetTranslation(vPos);

			this->GetTR().SetColor(CB::Math::CColor(this->m_ColorValue.GetValue()));
			this->GetTR().SetTransform(mMatrix);
			this->GetTR().SetScale(this->m_Rect.Size.Y);
			this->GetTR().Print(this->m_strText);
			this->m_pParent->GetParent()->SetUpRender();
		}

		void	CTextItem::Update(const float32 fTD){
			if(this->m_Rect.Contains(this->m_pParent->GetCursorPos())){
				this->m_ColorValue.Increment(fTD);
			}
			else{
				this->m_ColorValue.Decrement(fTD);
			}
		}
	}
}