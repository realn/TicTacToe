#pragma once

#include <Ref.h>
#include <SmartPointers_RefPtr.h>
#include <GraphicDriver.h>

#include "Manage_ItemContainer.h"
#include "GUIItem.h"

namespace T3{
	namespace GUI{
		class CMain;

		class CScreen : 
			public CB::IRef,
			public Manage::IItem<CMain>,
			public Manage::IItemContainer<IItem>
		{
		protected:
			CB::Math::CVector2D	m_vSize;

		public:
			CScreen(CMain& pMain, const CB::Math::CVector2D& vSize);

			virtual void	Render();
			virtual void	Update(const float32 fTD);

			virtual const bool	AddItem(CScreen::ItemPtr pItem) override;

			const CB::Math::CMatrix	GetTransform() const;
		};
	}
}