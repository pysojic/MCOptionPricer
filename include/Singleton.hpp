// Singleton.hpp
//
// Header file for modified Singleton pattern in which the destruction
// of the Singleton takes place in a Destroyer object, thus resolving
// a major problem in the classic Gamma Singleton pattern.
//
// The Destroyer class is templated while clients can create subclasses 
// of Singleton to suit their needs. 

#pragma once

// Forward reference for singleton
template<class TYPE> class Singleton; 

template<class TYPE> 
class Destroyer 
{ // Class which is responsible for the destruction of another type TYPE

	friend class Singleton<TYPE>;	// Singleton is friend of mine

private:
	TYPE* doomed_object;			// The object that is to be destructed 
	
	// Prevent users doing funny things (e.g. double deletion)									
	Destroyer(TYPE* t);				// Constructor with a pointer to the doomed object
	// Modifier
	void doomed(TYPE* t);

public:
	Destroyer(const Destroyer<TYPE>& source) = delete;  // Copy constructor
	Destroyer<TYPE>& operator = (const Destroyer<TYPE>& source) = delete; // Assignment operator
	virtual ~Destroyer();	// Destructor 

	// Somehow the default constructor must be public else we got a compiler error in class ctype
	Destroyer();													// Default constructor
};


template<class TYPE> 
class Singleton
{ // Templated Singleton class

private:
	static TYPE* ins;
	static Destroyer<TYPE> des;

protected:
	Singleton();
	Singleton(const Singleton<TYPE>& source);
	virtual ~Singleton();
	Singleton<TYPE>& operator = (const Singleton<TYPE>& source);

public:
	static TYPE* instance();
};

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
