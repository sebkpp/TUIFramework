#pragma once
#include <tuiframework/core/ISerializable.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class SceneObject : public tuiframework::ISerializable
{
public:
	SceneObject();
	SceneObject(string name, vector< string > parameterList);
	~SceneObject();
	virtual std::ostream & serialize(std::ostream & os) const;
	virtual std::istream & deSerialize(std::istream & is);
	void changeValue(int index, string value);
	int getParameterListSize();

private:
	string name;
	vector<string> parameterList;
};

