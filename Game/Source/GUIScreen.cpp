#include "GUIScreen.h"
#include "GUIMain.h"

namespace T3{
	namespace GUI{
		CScreen::CScreen(CMain& pMain, const CB::Math::CVector2D& vSize) :
			Manage::IItem<CMain>(&pMain),
			m_vSize(vSize)
		{

		}

		void	CScreen::Render(){
			auto mProj = this->GetTransform();
			this->m_pParent->SetProjection(mProj);

			for(uint32 i = 0; i < this->m_Items.GetLength(); i++){
				this->m_Items[i]->Render();
			}
		}

		void	CScreen::Update(const float32 fTD){
			for(uint32 i = 0; i < this->m_Items.GetLength(); i++){
				this->m_Items[i]->Update(fTD);
			}
		}

		const bool	CScreen::AddItem(CScreen::ItemPtr pItem){
			if(!Manage::IItemContainer<GUI::IItem>::AddItem(pItem)){
				return false;
			}

			return true;
		}

		const CB::Math::CMatrix	CScreen::GetTransform() const{
			return CB::Math::CMatrix::GetOrtho(0.0f, this->m_vSize.X, 0.0f, this->m_vSize.Y, -1.0f, 1.0f);
		}

		const CB::Math::CVector2D	CScreen::GetCursorPos() const{
			return this->m_pParent->GetCursorPos() * this->m_vSize;
		}
	}
}