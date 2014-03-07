#pragma once

#include <Ref.h>
#include <SmartPointers_RefPtr.h>
#include <Math_RectangleF.h>

#include "Manage_ItemContainer.h"
#include "GUIEvent.h"

namespace T3{
	class CTextRenderer;

	namespace GUI{
		class CScreen;

		enum class ItemType{
			Text,
			Button,
		};

		class IItem :
			public CB::IRef,
			public Manage::IContItem<CScreen>
		{
		protected:
			CB::Math::CRectangleF32	m_Rect;

		public:
			IItem();
			virtual ~IItem();

			virtual const ItemType	GetType() const = 0;

			virtual void	Render() = 0;
			virtual void	Update(const float32 fTD) = 0;

			virtual void	ProcessEvent(const CEvent& Event);

			virtual void	SetRect(const CB::Math::CRectangleF32& Rect);
			virtual const CB::Math::CRectangleF32	GetRect() const;

		protected:
			CTextRenderer&	GetTR();
		};
	}
}