#include "GUIControlPanel.h"

namespace T3{
	namespace GUI{
		namespace Control{
			CPanel::CPanel(IScreen& Screen, const CB::CString& strID) :
				CControlBase(Screen, strID)
			{

			}

			void	CPanel::SetRect(const CB::Math::CRectangleF32& Rect){

			}

			const CB::Math::CRectangleF32& CPanel::GetRect() const{
				return this->m_Rect;
			}

			void	CPanel::Render(const IEnv& Env){
				Env.GetDevice()->SetVertexBuffer(0, this->m_pVertexBuffer);
				Env.GetDevice()->SetIndexBuffer(this->m_pIndexBuffer);

				Env.GetDevice()->RenderIndexed(2);

				CControlBase::Render(Env);
			}
		}
	}
}