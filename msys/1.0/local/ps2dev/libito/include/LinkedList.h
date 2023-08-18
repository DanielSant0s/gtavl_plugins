#ifndef _ITO_LINKEDLIST_H
#define _ITO_LINKEDLIST_H

#include <ItoTypes.h>

namespace Ito
{

/** Simple Double Linked List.  
*/
template<class T> class LinkedList
{
	private:
		class Link
		{
			public:
			Link*	prev;
			Link*	next;
			T		item;
			public:
				Link(Link* prev, Link* next, T item)
				{
					this->prev = prev;
					this->next = next;
					this->item = item;
				}
				~Link() 
				{
					//delete item;
				}
		};
		
		Link* m_Head; 
		Link* m_Tail;
		u32 m_Size;
	

	private:
		Link* GetLink(u32 index)
		{
			if(index == 0)
				return m_Head;

			if(index == (m_Size-1))
				return m_Tail;

			
			if(index < (m_Size/2))
			{
				Link* link = m_Head;

				for(u32 i=0; i < index; i++)
				{	
					link = link->next;
				}

				return link;
			}
			else
			{
				Link* link = m_Tail;

				for(u32 i=(m_Size-1); i > index; i--)
				{	
					link = link->prev;
				}
				
				return link;
			}
		}

		void RemoveLink(Link *link)
		{
			if(link->prev)
			{
				link->prev->next = link->next;
			
				if(link == m_Tail)
					m_Tail = link->prev;
			}

			if(link->next)
			{
				link->next->prev = link->prev;

				if(link == m_Head)
					m_Head = link->next;
			}

			delete link;	
		}
	public:
		LinkedList();
		~LinkedList();
		/** Add an item. 
			\param item item to add
			\return index at which the item was added
		*/
		u32 Add(T item);
		/** Add an item at a specific index. If there
			are more items after the index at which the item
			is insert, then the items will be moved up one index.
			\param index index to add the item to.
			\param item item to add
		*/
		void Add(u32 index, T item);

		/** Get an item at a specific index.
			\param index index to get the item from
			\return item, if item is 0 then there was no item at the index.
		*/
		T Get(u32 index);
		/** Set the item at a specific index. This function will replace
			the existing item at the index.
			\param index index to set the item at
			\param item item to set
			\return previous item at the index, if item is 0 then there was no item at the index and
					the item was not replaced.
		*/
		T Set(u32 index, T item);
		/** Remove a item at a specific index.
			\param index index for the item to remove
		*/
		void Remove(u32 index);

		/** Get the size of the list.
			\return size size of the list
		*/
		u32 Size();
		/** Check if list is empty.
			\return true if list is empty otherwise false.
		*/
		bool IsEmpty();

		/** Clear the list. Removes all items from the list and resets the size to 0.
		*/
		void Clear();
		/** Get the last item of the list.
			\return last item of the list
		*/
		T Last();
		/** Get the index of an item. This function searches the list for a matching item.
			\return -1 if the item wasn't found otherwise the index of the item
		*/
		s32 IndexOf(T item);

		/** Check if the list contains an item. This function searches the list for a matching item.
			\return true if the list contains the item otherwise false
		*/
		bool Contains(T item);
};


template<class T> LinkedList<T>::LinkedList()
{
	m_Head = 0;
	m_Tail = 0;
	m_Size = 0;
}

template<class T> LinkedList<T>::~LinkedList()
{
	Clear();
}

template<class T> void LinkedList<T>::Add(u32 index, T item)
{

	if(index > (m_Size-1)) return;

	Link *link = GetLink(index);

	Link* newlink = new Link(link->prev, link, item);

	link->prev->next = newlink;
	link->prev = newlink;

}

template<class T> u32 LinkedList<T>::Add(T item)
{
	if(m_Head == 0)
	{
		m_Head = new Link(0,0, item);
		m_Tail = m_Head;
	}
	else
	{
		m_Tail->next = new Link(m_Tail, 0, item);
		m_Tail = m_Tail->next;
	}
	
	m_Size++;

	return m_Size-1;
}

template<class T> void LinkedList<T>::Remove(u32 index)
{
	if(index > (m_Size-1)) return;

	RemoveLink(GetLink(index));

	m_Size--;

	if(m_Size == 0)
	{
		m_Head = 0;
		m_Tail = 0;
	}
}

template<class T> T LinkedList<T>::Set(u32 index, T item)
{
	if(index > (m_Size-1)) return T();

	Link *link = GetLink(index);
	T old = link->item;
	
	link->item = item;

	return old;
}

template<class T> T LinkedList<T>::Get(u32 index)
{
	if(index > (m_Size-1)) return T();
	
	return GetLink(index)->item;
}

template<class T> u32 LinkedList<T>::Size()
{
	return m_Size;
}

template<class T> bool LinkedList<T>::IsEmpty()
{
	return (bool)(m_Size == 0);
}

template<class T> void LinkedList<T>::Clear()
{

	while(Size() > 0) Remove(0);
}

template<class T> T LinkedList<T>::Last()
{
	if(m_Tail)
		return	m_Tail->item;
	else
		return T();
}

template<class T> s32 LinkedList<T>::IndexOf(T item)
{
	for(u32 i=0; i < Size(); i++)
	{
		if(Get(i) == item)
			return i;
	}

	return -1;
}

template<class T> bool LinkedList<T>::Contains(T item)
{
	if(IndexOf(item) > 0)
		return true;
	else
		return false;
}

}

#endif
