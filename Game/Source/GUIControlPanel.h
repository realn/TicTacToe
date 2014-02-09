#pragma once

#include "GUIControlBase.h"

namespace T3{
	namespace GUI{
		namespace Control{
			class CPanel :
				public CControlBase
			{
			protected:
				CB::Math::CRectangleF32				m_Rect;
				CB::CRefPtr<CB::Graphic::IBuffer>	m_pVertexBuffer;
				CB::CRefPtr<CB::Graphic::IBuffer>	m_pIndexBuffer;

			public:
				CPanel(IScreen& Screen, const CB::CString& strID);

				void	SetRect(const CB::Math::CRectangleF32& Rect);
				const CB::Math::CRectangleF32&	GetRect() const;

				virtual void	Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice) override;

			private:

			};
		}
	}
}