#include "GUIItem.h"
#include "GUIScreen.h"
#include "GUIMain.h"
#include "TextRenderer.h"

namespace T3{
	namespace GUI{
		IItem::IItem(){}

		IItem::~IItem(){}

		void	IItem::ProcessEvent(const CEvent& Event){
		}

		void	IItem::SetRect(const CB::Math::CRectangleF32& Rect){
			this->m_Rect = Rect;
		}

		const CB::Math::CRectangleF32	IItem::GetRect() const{
			return this->m_Rect;
		}

		CTextRenderer&	IItem::GetTR(){
			return this->m_pParent->GetParent()->GetTextRenderer();
		}
	}
}