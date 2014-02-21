#pragma once

#include <Ref.h>
#include <SmartPointers_RefPtr.h>
#include <Collection_List.h>
#include <Collection_Funcs.h>

namespace T3{
	namespace Manage{
		template<typename _Parent>
		class IItem{
		public:
			typedef CB::CPtr<_Parent> ParentPtr;

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
		typename IItem<_Parent>::ParentPtr	IItem<_Parent>::GetParent() const{
			return this->m_pParent;
		}

		

		template<typename _Item>
		class IItemContainer{
		public:
			typedef CB::CRefPtr<_Item>	ItemPtr;

		protected:
			CB::Collection::CList<ItemPtr>	m_Items;
			IItemContainer();

		public:
			~IItemContainer();

			virtual	const bool	AddItem(ItemPtr Item);
			virtual	const bool	RemoveItem(ItemPtr Item);
			virtual void	Clear();
		};

		template<typename _Item>
		IItemContainer<_Item>::IItemContainer(){}

		template<typename _Item>
		IItemContainer<_Item>::~IItemContainer(){}

		template<typename _Item>
		const bool	IItemContainer<_Item>::AddItem(typename IItemContainer<_Item>::ItemPtr Item){
			if(!CB::Collection::Contains(this->m_Items, Item)){
				this->m_Items.Add(Item);
				return true;
			}
			return false;
		}

		template<typename _Item>
		const bool	IItemContainer<_Item>::RemoveItem(typename IItemContainer<_Item>::ItemPtr Item){
			uint32 uIndex = 0;
			if(CB::Collection::TryFind(this->m_Items, Item, uIndex)){
				this->m_Items.Remove(uIndex);
				return true;
			}
			return false;
		}

		template<typename _Item>
		void	IItemContainer<_Item>::Clear(){
			this->m_Items.Clear();
		}
	}
}