#include "GUITextItem.h"
#include "GUIScreen.h"
#include "GUIMain.h"
#include "TextRenderer.h"

namespace T3{
	namespace GUI{
		CTextItem::CTextItem(CB::CRefPtr<CScreen> pScreen, const CB::CString& strText) :
			IItem(pScreen),
			m_strText(strText)
		{
			
		}

		const ItemType	CTextItem::GetType() const{
			return ItemType::Text;
		}

		void	CTextItem::Render(){
			CB::Math::CVector3D vPos(this->m_Rect.Position, 0.0f);
			auto mMatrix = this->m_pParent->GetTransform() * CB::Math::CMatrix::GetTranslation(vPos);

			this->GetTR().SetTransform(mMatrix);
			this->GetTR().Print(this->m_strText);
			this->m_pParent->GetParent()->SetUpRender();
		}

		void	CTextItem::Update(const float32 fTD){

		}
	}
}