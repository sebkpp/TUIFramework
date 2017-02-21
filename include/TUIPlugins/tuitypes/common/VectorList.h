/*
    Copyright (C) 2010, 2011, 2012 The Fraunhofer Institute for Production Systems and
    Design Technology IPK. All rights reserved.

    This file is part of the TUIFramework library.
    It includes a software framework which contains common code
    providing generic functionality for developing applications
    with a tangible user interface (TUI).
    
    The TUIFramework library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    The TUIFramework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with the TUIFramework.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _tuiframework_Vector_h_
#define _tuiframework_Vector_h_

#include <tuiframework/core/ISerializable.h>

#include <iostream>
#include <vector>

#include <omp.h>

/**
 * \brief Wrapper-Class for a Vector.
 * 
 * This Class is used to store different DataTypes in TUI-Framework. Maybe needs to change in
 * future.
 *
 * \author Sebastian Keppler
 * \date 04.02.2016
 *
 * \sa tuiframework::ISerializable Interface for Serilization Data
 */

template <typename T>
class VectorList : public tuiframework::ISerializable {
public:
	/**
	 * \brief Default constructor.
	 *
	 * \author Sebastian Keppler
	 * \date 04.02.2016
	 */

	VectorList()
	{
		items = std::vector<T>();
	}

	/**
	 * \brief Constructor.
	 *
	 * \author Sebastian Keppler
	 * \date 04.02.2016
	 *
	 * \param pVector A vector.
	 */

	VectorList(std::vector<T> pVector)
	{
		this->items = pVector;
	}

    /**
     * \brief Copy constructor.
     *
     * \author Sebastian Keppler
     * \date 04.02.2016
     *
     * \param v The VectorList to process.
     */

    VectorList(const VectorList &v)
	{
		this->items = v.getData();
	}

    /**
     * \brief Destructor.
     *
     * \author Sebastian Keppler
     * \date 04.02.2016
     */

    ~VectorList()
	{
	}

	/**
	 * \brief Sets the data.
	 *
	 * \author Sebastian Keppler
	 * \date 04.02.2016
	 *
	 * \param [in,out] values If non-null, the values.
	 * \param size			  The size.
	 */

	void setData(T* values, int size)
	{
		this->items = std::vector<T>(&values[0],&values[size]);

    }

	/**
	 * \brief Gets the data.
	 *
	 * \author Sebastian Keppler
	 * \date 04.02.2016
	 *
	 * \return The data.
	 */

	std::vector<T> &getData()
	{
        return this->items;
    }

	 /**
	  * \brief Gets the data.
	  *
	  * \author Sebastian Keppler
	  * \date 04.02.2016
	  *
	  * \return The data.
	  */

	 const std::vector<T> &getData() const
	{
        return this->items;
    }

	/**
	 * \brief Gets the size of the stored items.
	 *
	 * \author Sebastian Keppler
	 * \date 04.02.2016
	 *
	 * \return The size.
	 */

	int size() const
	{
		return this->items.size();
	}

	/**
	 * \brief Gets an item stored in the data.
	 *
	 * \author Sebastian Keppler
	 * \date 04.02.2016
	 *
	 * \param i Zero-based index of the.
	 *
	 * \return an item in the data.
	 */

	const T  at(int i) const{
		return this->items.at(i);
	}

   /**
    * \brief serialize this object to the given stream.
    *
    * \author Sebastian Keppler
    * \date 04.02.2016
    *
    * \param [in,out] os Output-Stream.
    *
    * \return A std::ostream&amp;
    */

   virtual std::ostream & serialize(std::ostream & os) const {
	   
	    os << items.size();

		//#pragma omp parallel num_threads(6)    // Used for parallelzation
		{
			//#pragma omp for
			for (int i = 0; i < items.size(); ++i) 
			{
				os << " " << items[i];
			}
		}


        return os;
    }

    /**
     * \brief deSerialize this object to the given stream.
     *
     * \author Sebastian Keppler
     * \date 04.02.2016
     *
     * \param [in,out] is The Input-Stream.
     *
     * \return A std::istream&amp;
     */

    virtual std::istream & deSerialize(std::istream & is) {
		this->items.clear();
		
		int count;
        is >> count;

		//items.resize(count);
        //#pragma omp parallel for
		for (int i = 0; i < count; ++i) 
		{
            T payload;
            is >> payload;
			items.push_back(payload);
			
        }



        return is;
    }

protected:

    /**
     * \brief The items.
     *
     * ### summary \brief The items stored in a std::vector.
     */

    std::vector<T> items;
};



#endif
