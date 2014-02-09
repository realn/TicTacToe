#include "GUIControlBase.h"

namespace T3{
	namespace GUI{
		CControlBase::CControlBase(IScreen& Screen, const CB::CString& strID) :
			m_Screen(Screen),
			m_strID(strID),
			m_bVisible(true),
			m_bEnabled(true)
		{
		}

		const CB::CString&	CControlBase::GetID() const{
			return this->m_strID;
		}

		IControlList&	CControlBase::GetControls() const{
			return this->m_Controls;
		}

		void	CControlBase::SetParent(CB::CRefPtr<IControl> pControl){
			this->m_pParent = pControl;
		}

		void	CControlBase::SetParentNull(){
			this->m_pParent.Reset();
		}

		CB::CRefPtr<IControl>	CControlBase::GetParent() const{
			return this->m_pParent;
		}

		void	CControlBase::AddControl(CB::CRefPtr<IControl> pControl){
			if(!CB::Collection::Contains(this->m_Controls, pControl)){
				this->m_Controls.Add(pControl);
				pControl->SetParent(this);
			}
		}

		void	CControlBase::RemoveControl(CB::CRefPtr<IControl> pControl){
			uint32 uIndex = 0;
			if(CB::Collection::TryFind(this->m_Controls, pControl, uIndex)){
				this->RemoveControl(uIndex);
			}
		}

		void	CControlBase::RemoveControl(const uint32 uIndex){
			this->m_Controls.Remove(uIndex);
		}

		void	CControlBase::SetEnabled(const bool bEnabled){
			this->m_bEnabled = bEnabled;
		}

		const bool	CControlBase::IsEnabled() const{
			return this->m_bEnabled;
		}

		void	CControlBase::SetVisible(const bool bVisible){
			this->m_bVisible = bVisible;
		}

		const bool	CControlBase::IsVisible() const{
			return this->m_bVisible;
		}

		void	CControlBase::Update(const float32 fTD){
			if(!this->m_bEnabled)
				return;

			for(uint32 i = 0; i < this->m_Controls.GetLength(); i++){
				this->m_Controls[i]->Update(fTD);
			}
		}

		void	CControlBase::Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice){
			if(!this->m_bVisible)
				return;

			for(uint32 i = 0; i < this->m_Controls.GetLength(); i++){
				this->m_Controls[i]->Render(pDevice);
			}
		}
	}
}