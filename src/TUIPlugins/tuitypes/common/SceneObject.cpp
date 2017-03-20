#include "SceneObject.h"


SceneObject::SceneObject()
{
}

SceneObject::SceneObject(string name, vector<string> parameterList)
{
	this->name = name;
	this->parameterList = parameterList;
}


SceneObject::~SceneObject()
{
}

std::ostream & SceneObject::serialize(std::ostream & os) const
{
	os << parameterList.size() << "\n"; 
	for (int i = 0; i < parameterList.size(); i++)
	{
		os << parameterList.at(i) << "\n";
	}
	
	return os;
}

std::istream & SceneObject::deSerialize(std::istream & is)
{
	this->parameterList.clear();

	int count;
	is >> count;

	for (int i = 0; i < count; ++i)
	{
		string parameterString;
		is >> parameterString;
		this->parameterList.push_back(parameterString);
	}


	return is;
}

void SceneObject::changeValue(int index, string value)
{
	size_t position = this->parameterList.at(index).find_last_of("_");
	this->parameterList.at(index).replace(position + 1,parameterList.at(index).length() - position+1, value );
}

int SceneObject::getParameterListSize()
{
	return this->parameterList.size();
}
