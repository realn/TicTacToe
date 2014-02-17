#pragma once

#include <Collection_List.h>
#include <Ref.h>
#include <SmartPointers_RefPtr.h>

namespace T3{
	namespace Manage{
		template<typename _Parent>
		class IItem{
		public:
			typename CB::CPtr<_Parent> ParentPtr;

		protected:
			ParentPtr	m_pParent;

		public:
			IItem(ParentPtr pParent);
			~IItem();

			ParentPtr	GetParent() const;
		};

		template<typename _Parent>
		IItem<_Parent>::IItem(typename IItem<_Parent>::ParentPtr pParent) :
			m_pParent(pParent)
		{}

		template<typename _Parent>
		IItem<_Parent>::~IItem(){}

		template<typename _Parent>
		IItem<_Parent>::ParentPtr	IItem<_Parent>::GetParent() const{
			return this->m_pParent;
		}

		

		template<typename _Parent, typename _Item>
		class IItemContainer{
		public:
			
		};
	}
}