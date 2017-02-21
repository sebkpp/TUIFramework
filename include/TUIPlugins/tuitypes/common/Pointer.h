#ifndef _tuiframework_Pointer_h_
#define _tuiframework_Pointer_h_

#include <tuiframework/core/ISerializable.h>
#include <iostream>

template <typename T>
class Pointer : public tuiframework::ISerializable {

public:
	Pointer()
	{
		pointer = T();
	}

	Pointer(T* pPointer)
	{
		pointer = pPointer;
	}
    
    Pointer(const Pointer &p)
	{
		this->pointer = p.getData();
	}
	
	~Pointer(void);
	
	T& getData()
	{
        return this->pointer;
    }

	const T& getData() const
	{
        return this->pointer;
    }

	const void setData(T values) const
	{
		this->pointer = values;
    }

	virtual std::ostream & serialize(std::ostream & os) const {
        //os << this->items.size();
        //typename std::vector<T>::const_iterator i = this->items.begin();
        //typename std::vector<T>::const_iterator e = this->items.end();
        
		
		//while (i != e) {
        //  os << " " << *i;
        //  ++i;
        //}
		os << pointer;

        return os;
    }
    
    
    virtual std::istream & deSerialize(std::istream & is) {
		//this->items.clear();
		
		//int count;
        //is >> count;
		//is >> pointer;
		/*
        for (int i = 0; i < count; ++i) {
            T payload;
            is >> payload;
            this->items.push_back(payload);
        }
		*/
        return is;
    }
private:

	T pointer;
};

#endif