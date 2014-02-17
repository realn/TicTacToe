#pragma once

#include <Ref.h>
#include <SmartPointers_RefPtr.h>
#include <Manage_Object.h>

namespace T3{
	namespace GUI{
		class CScreen;

		enum class ItemType{
			Text,
			Button,
		};

		class IItem :
			public CB::IRef,
			public CB::Manage::IManagedObject<CScreen, IItem>
		{
		public:
			IItem(CScreen& Screen);

			virtual ItemType	GetType() const = 0;

			virtual void	Render() = 0;
			virtual void	Update(const float32 fTD) = 0;
		};
	}
}