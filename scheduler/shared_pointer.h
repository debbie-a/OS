#ifndef __SHARED_POINTER_H__
#define __SHARED_POINTER_H__

#include <cstddef>
#include <iostream>


template<typename T>
class SharedPtr
{
public:
	explicit SharedPtr(T *ptr = NULL);
	~SharedPtr();
	SharedPtr(const SharedPtr<T>& other);

	template <typename U>
	SharedPtr(const SharedPtr<U>& other);
	SharedPtr& operator=(const SharedPtr<T>& other);
	template <typename U>
	SharedPtr<T>& operator=(const SharedPtr<U>& other);
	
	T* operator->();
    	T& operator*();
	operator bool() const;


	T* getPtr() const;
	size_t getNumCopies() const;
	size_t* getNumCopiesPtr() const;

private:
	T *m_ptr;
	size_t *m_numCopies;
	
	void releasePointer();
	void increaseNumCopies();
	void decreaseNumCopies();
	
	
};

template <typename T, typename U>
inline bool operator==(const SharedPtr<T>& sharedPtr1, const SharedPtr<U>& sharedPtr2);
template <typename T, typename U>
inline bool operator!=(const SharedPtr<T>& sharedPtr1, const SharedPtr<U>& sharedPtr2);


template<typename T>
inline SharedPtr<T>::SharedPtr(T *ptr):m_ptr(ptr),m_numCopies(ptr ? new size_t(1) : NULL)
{
	
}

template<typename T>
inline SharedPtr<T>::~SharedPtr()
{
	releasePointer();
}

template<typename T>
inline SharedPtr<T>::SharedPtr(const SharedPtr<T>& other):m_ptr(other.m_ptr),m_numCopies(other.m_numCopies)
{
        increaseNumCopies();
}

template <typename T>
template <typename U>
inline SharedPtr<T>::SharedPtr(const SharedPtr<U>& other):m_ptr(other.m_ptr),m_numCopies(other.m_numCopies)
{
	increaseNumCopies();
}

template<typename T>
inline SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other)
{
	if (this != &other)
	{
		if (m_numCopies != NULL)
		{
			releasePointer();
		}
		m_ptr = other.m_ptr;
		m_numCopies = other.m_numCopies;
		increaseNumCopies();
	}

	return *this;
}

template <typename T>
template <typename U>
inline SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<U>& other)
{
	if (*this != other)
	{
		if (m_numCopies != NULL)
		{
			releasePointer();
		}
		m_ptr = other.getPtr();
		m_numCopies = other.getNumCopiesPtr();
		increaseNumCopies();
	}

	return *this;
}

template<typename T>
inline T* SharedPtr<T>::operator->()
{
	return m_ptr;
}

template<typename T>
inline T& SharedPtr<T>::operator*()
{
	return *m_ptr;
}

template<typename T>
inline SharedPtr<T>::operator bool() const
{
	return m_ptr;
}

template<typename T>
inline T* SharedPtr<T>::getPtr() const
{
	return m_ptr;
}

template<typename T>
inline size_t SharedPtr<T>::getNumCopies() const
{
	return m_numCopies != NULL ? *m_numCopies : 0;
}

template<typename T>
inline void SharedPtr<T>::increaseNumCopies()
{
	(*m_numCopies)++;
}

template<typename T>
inline void SharedPtr<T>::decreaseNumCopies()
{
	(*m_numCopies)--;
}

template<typename T>
inline void SharedPtr<T>::releasePointer()
{
	decreaseNumCopies();
	if (0 == *m_numCopies)
	{
		delete m_ptr;
		delete m_numCopies; //don't allocate arrays
	}
}

template<typename T>
inline size_t* SharedPtr<T>::getNumCopiesPtr() const
{	
	return m_numCopies != NULL ? m_numCopies : NULL;
}

template <typename T, typename U>
inline bool operator==(const SharedPtr<T>& sharedPtr1, const SharedPtr<U>& sharedPtr2)
{
	return sharedPtr1.getPtr() == sharedPtr2.getPtr();
}

template <typename T, typename U>
inline bool operator!=(const SharedPtr<T>& sharedPtr1, const SharedPtr<U>& sharedPtr2)
{
	return !(sharedPtr1 == sharedPtr2);
}

template<typename T>
std::ostream& operator<<(std::ostream& cout, const SharedPtr<T>& sharedPtr) 
{ 
       	cout << sharedPtr.getPtr();
	return cout;
} 
  
#endif /*__SHARED_POINTER_H__*/
