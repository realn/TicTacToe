#pragma once

#include <Ref.h>
#include <SmartPointers_RefPtr.h>
#include <GraphicDriver.h>

#include "Manage_ItemContainer.h"
#include "GUIItem.h"

namespace T3{
	namespace GUI{
		class CMain;

		enum class TransitionMode{
			None,
			Show,
			Hide,
		};

		class CScreen : 
			public CB::IRef,
			public Manage::IContItem<CMain>,
			public Manage::IContItemContainer<CScreen, IItem>
		{
		protected:
			CB::Math::CVector2D	m_vSize;

		public:
			CScreen(CMain& pMain, const CB::Math::CVector2D& vSize);

			virtual void	Render();
			virtual void	Update(const float32 fTD);

			virtual void	ProcessEvent(const CEvent& Event);

			virtual const bool	AddItem(CScreen::ItemPtr pItem) override;

			virtual void	SetTransitionMode(const TransitionMode uMode);

			virtual const CB::Math::CMatrix	GetTransform() const;
			virtual const TransitionMode	GetTransitionMode() const;

			const CB::Math::CVector2D	GetCursorPos() const;
		};
	}
}