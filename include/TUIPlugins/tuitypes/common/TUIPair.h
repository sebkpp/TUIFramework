#pragma once
#include <tuiframework/core/ISerializable.h>

template <typename T, typename P>
class TUIPair :
	public tuiframework::ISerializable
{
public:
	TUIPair();
	~TUIPair();

	void setFirst(T pFirst);
	void setSecond(P pSecond);

	T getFirst();
	P getSecond();

	virtual std::ostream & serialize(std::ostream & os) const;
	virtual std::istream & deSerialize(std::istream & is);

private:
	T first;
	P second;
};

