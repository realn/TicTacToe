#pragma once

#include "GUIControlInterface.h"
#include "GUIScreenInterface.h"

namespace T3{
	namespace GUI{
		class CScreen :
			public IScreen
		{
		protected:
			CB::CRefPtr<CB::Graphic::IDevice>				m_pDevice;
			mutable CB::Collection::CList<CB::CRefPtr<IControl>>	m_Controls;
			CB::Math::CVector2D	m_vSize;

		public:
			CScreen(CB::CRefPtr<CB::Graphic::IDevice> pDevice, const CB::Math::CVector2D& vSize);

			CB::CRefPtr<CB::Graphic::IDevice>	GetDevice() const override;
			const CB::Math::CVector2D			GetSize() const override;

			IControlList&	GetControls() const override;

			template<typename _Type>
			CB::CRefPtr<_Type>	AddControl(const CB::CString& strID){
				_Type* pControl = new _Type(*this, strID);
				this->m_Controls.Add(pControl);
				return pControl;
			}

			virtual void	Update(const float32 fTD) override;
			virtual void	Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice) override;
		};
	}
}