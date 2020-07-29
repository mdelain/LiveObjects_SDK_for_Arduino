#pragma once
template<typename T>
class ListNode
{
  public:
    ListNode(T* el, ListNode* p) : element(el),prev(p){};
    ~ListNode(){delete element;}
  public:
    T* element;
    ListNode* prev;
};

template<typename T>
class LinkedList
{
  public:
    LinkedList(): m_nSize(0), head(nullptr), tail(nullptr){}
    ~LinkedList();
  public:
    void push(T* element);
    int size();
    int find(T* element);
    T* operator[](size_t index);
  private:
    ListNode<T>* head;
    ListNode<T>* tail;
    int m_nSize;
};

template<typename T>
LinkedList<T>::~LinkedList()
{
  ListNode<T> * tmp = tail;
  for(int i=0;i<m_nSize;++i)
  {
    tmp = tail->prev;
    delete tail;
    if(i<m_nSize-1)tail = tmp;
  }

}

template<typename T>
int LinkedList<T>::size()
{
  return m_nSize;
}

template<typename T>
void LinkedList<T>::push(T* element)
{
  tail = new ListNode<T>(element, tail);
  m_nSize++;
}

template<typename T>
T* LinkedList<T>::operator[](size_t index)
{
  ListNode<T>* tmp=tail;
  for(int i=0;i<index;++i)
  {
    tmp = tmp->prev;
  }
  return tmp->element;
}

template<typename T>
int LinkedList<T>::find(T* element)
{
  ListNode<T>* tmp=tail;
  for(int i=0;i<m_nSize;++i)
  {
    if(*(tmp->element)==*element) return i;
    tmp = tmp->prev;
  }
  return -1;
}
