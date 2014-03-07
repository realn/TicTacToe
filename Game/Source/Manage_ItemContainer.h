#pragma once

#include <Ref.h>
#include <SmartPointers_RefPtr.h>
#include <Collection_List.h>
#include <Collection_Funcs.h>

namespace T3{
	namespace Manage{
		template<typename _Parent>
		class IContItem{
		public:
			typedef CB::CPtr<_Parent> ParentPtr;

		protected:
			ParentPtr	m_pParent;

		public:
			IContItem();
			IContItem(ParentPtr pParent);
			virtual ~IContItem();

			void		SetParent(ParentPtr pParent);
			void		SetParentNull();
			ParentPtr	GetParent() const;
		};

		template<typename _Parent>
		IContItem<_Parent>::IContItem()
		{}

		template<typename _Parent>
		IContItem<_Parent>::IContItem(typename IContItem<_Parent>::ParentPtr pParent) :
			m_pParent(pParent)
		{}

		template<typename _Parent>
		IContItem<_Parent>::~IContItem(){}

		template<typename _Parent>
		void	IContItem<_Parent>::SetParent(typename IContItem<_Parent>::ParentPtr pParent){
			this->m_pParent = pParent;
		}

		template<typename _Parent>
		void	IContItem<_Parent>::SetParentNull(){
			this->m_pParent.Reset();
		}

		template<typename _Parent>
		typename IContItem<_Parent>::ParentPtr	IContItem<_Parent>::GetParent() const{
			return this->m_pParent;
		}

		

		template<typename _Parent, typename _Item>
		class IContItemContainer{
		public:
			typedef CB::CRefPtr<_Item>	ItemPtr;

		protected:
			CB::Collection::CList<ItemPtr>	m_Items;
			IContItemContainer();

		public:
			virtual ~IContItemContainer();

			virtual	const bool	AddItem(ItemPtr Item);
			virtual	const bool	RemoveItem(ItemPtr Item);
			virtual void	Clear();
		};

		template<typename _Parent, typename _Item>
		IContItemContainer<_Parent, _Item>::IContItemContainer(){}

		template<typename _Parent, typename _Item>
		IContItemContainer<_Parent, _Item>::~IContItemContainer(){
			this->Clear();
		}

		template<typename _Parent, typename _Item>
		const bool	IContItemContainer<_Parent, _Item>::AddItem(typename IContItemContainer<_Parent, _Item>::ItemPtr Item){
			if(!CB::Collection::Contains(this->m_Items, Item)){
				Item->SetParent(dynamic_cast<_Parent*>(this));
				this->m_Items.Add(Item);
				return true;
			}
			return false;
		}

		template<typename _Parent, typename _Item>
		const bool	IContItemContainer<_Parent, _Item>::RemoveItem(typename IContItemContainer<_Parent, _Item>::ItemPtr Item){
			uint32 uIndex = 0;
			if(CB::Collection::TryFind(this->m_Items, Item, uIndex)){
				this->m_Items[uIndex]->SetParentNull();
				this->m_Items.Remove(uIndex);
				return true;
			}
			return false;
		}

		template<typename _Parent, typename _Item>
		void	IContItemContainer<_Parent, _Item>::Clear(){
			for(uint32 i = 0; i < this->m_Items.GetLength(); i++)
				this->m_Items[i]->SetParentNull();
			this->m_Items.Clear();
		}
	}
}