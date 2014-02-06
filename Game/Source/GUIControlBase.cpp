#include "GUIControlBase.h"

namespace T3{
	namespace GUI{
		CControlBase::CControlBase(IScreen& Screen, const CB::CString& strID) :
			m_Screen(Screen),
			m_strID(strID)
		{
		}

		const CB::CString&	CControlBase::GetID() const{
			return this->m_strID;
		}

		IControlList&	CControlBase::GetControls() const{
			return this->m_Controls;
		}

		void	CControlBase::Update(const float32 fTD){
			for(uint32 i = 0; i < this->m_Controls.GetLength(); i++){
				this->m_Controls[i]->Update(fTD);
			}
		}

		void	CControlBase::Render(){
			for(uint32 i = 0; i < this->m_Controls.GetLength(); i++){
				this->m_Controls[i]->Render();
			}
		}
	}
}