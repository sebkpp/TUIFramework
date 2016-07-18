#ifndef _OPCType_h_
#define _OPCType_h_

#include <tuiframework/core/ISerializable.h>
#include <iostream>

template <typename T>
class OPCType : public tuiframework::ISerializable 
{
public:
	OPCType() {}
	~OPCType() {}

	std::pair<std::string, T> &getData()
	{
		return this->value;
	}

	void setData(std::pair<std::string, T> pair)
	{
		this->value = pair;
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

	virtual std::ostream & serialize(std::ostream & os) const
	{
		os << this->value.first << " " << this->value.second << " ";

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

	virtual std::istream & deSerialize(std::istream & is) 
	{
		std::string first;
		T second;
		is >> first;
		is >> second;

		this->value = std::pair<std::string, T>(first, second);
		return is;
	}

protected:
	std::pair<std::string, T> value;
};

#endif