// Singleton.cpp

#ifndef Singleton_cpp
#define Singleton_cpp

#include "Singleton.hpp"

template<class TYPE> 
Destroyer<TYPE>::Destroyer()
{ // Default constructor

	doomed_object = nullptr;
}

template<class TYPE> 
Destroyer<TYPE>::Destroyer(TYPE* t)	
{ // Constructor with reference to object to be destroyed

	doomed_object = t;
}

template<class TYPE> 
Destroyer<TYPE>::~Destroyer()	
{ // Destructor 
 
	delete doomed_object;
}

template<class TYPE> 
void Destroyer<TYPE>::doomed(TYPE* t)	
{ // Set the doomed object 

	doomed_object = t;
}


// Templated Singleton code
template<class TYPE> 
TYPE* Singleton<TYPE>::ins = nullptr;

template<class TYPE> 
Destroyer<TYPE> Singleton<TYPE>::des;


template<class TYPE>
Singleton<TYPE>::Singleton()
{ // Default constructor
}

template<class TYPE>
Singleton<TYPE>::Singleton(const Singleton<TYPE>& source)
{ // Copy constructor
}

template<class TYPE>
Singleton<TYPE>::~Singleton()
{ // Destructor

	int x;
	x = 0;
}

template<class TYPE> 
TYPE* Singleton<TYPE>::instance()
{ // Return the singleton instance

	if (ins == nullptr)
	{
		ins = new TYPE;
		des.doomed(ins);
	}

	return ins;
}

template<class TYPE>
Singleton<TYPE>& Singleton<TYPE>::operator = (const Singleton<TYPE>& source)
{ // Assignment operator
	
	return *this;
}

#endif	// Singleton_cpp