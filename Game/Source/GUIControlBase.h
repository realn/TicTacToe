#pragma once

#include <Math_RectangleF.h>
#include <Collection_List.h>

#include "GUIControlInterface.h"
#include "GUIScreenInterface.h"

namespace T3{
	namespace GUI{
		class CControlBase :
			public IControl
		{
		protected:
			IScreen&			m_Screen;
			CB::CPtr<IControl>	m_pParent;
			CB::CString			m_strID;
			bool				m_bEnabled;
			bool				m_bVisible;
			mutable CB::Collection::CList<CB::CRefPtr<IControl>>	m_Controls;

		public:
			CControlBase(IScreen& Screen, const CB::CString& strStrID);

			const CB::CString&		GetID() const override;
			virtual IControlList&	GetControls() const override;

			void	SetParent(CB::CRefPtr<IControl> pControl) override;
			void	SetParentNull() override;
			CB::CRefPtr<IControl>	GetParent() const override;

			void	AddControl(CB::CRefPtr<IControl> pControl) override;
			void	RemoveControl(CB::CRefPtr<IControl> pControl) override;
			void	RemoveControl(const uint32 uIndex) override;

			void	SetEnabled(const bool bEnabled) override;
			const bool	IsEnabled() const override;

			void	SetVisible(const bool bVisible) override;
			const bool	IsVisible() const override;
			
			virtual void	Update(const float32 fTD) override;
			virtual void	Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice) override;
		};
	}
}