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
			IScreen&	m_Screen;

			CB::CString				m_strID;
			mutable CB::Collection::CList<CB::CRefPtr<IControl>>	m_Controls;

		public:
			CControlBase(IScreen& Screen, const CB::CString& strStrID);

			const CB::CString&		GetID() const override;
			virtual IControlList&	GetControls() const override;
			
			virtual void	Update(const float32 fTD) override;
			virtual void	Render(const IEnv& Env) override;
		};
	}
}