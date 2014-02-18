#pragma once

#include <Ref.h>
#include <SmartPointers_RefPtr.h>

#include "Manage_ItemContainer.h"

namespace T3{
	namespace GUI{
		class CScreen;

		enum class ItemType{
			Text,
			Button,
		};

		class IItem :
			public CB::IRef,
			public Manage::IItem<CScreen>
		{
		public:
			IItem(CB::CRefPtr<CScreen> Screen);

			virtual const ItemType	GetType() const = 0;

			virtual void	Render() = 0;
			virtual void	Update(const float32 fTD) = 0;
		};
	}
}