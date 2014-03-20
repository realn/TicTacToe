#pragma once

namespace T3{
	namespace GUI{
		class ISelectableItem{
		public:
			virtual ~ISelectableItem(){}

			virtual void	SetSelected(const bool bSet) = 0;
			virtual const bool	IsSelected() const = 0;
		};
	}
}