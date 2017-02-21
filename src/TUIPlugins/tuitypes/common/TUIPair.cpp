#include <TUIPlugins\tuitypes\common\TUIPair.h>

template<typename T, typename P>
TUIPair<T, P>::TUIPair()
{
}

template<typename T, typename P>
TUIPair<T, P>::~TUIPair()
{
}

template<typename T, typename P>
void TUIPair<T, P>::setFirst(T pFirst)
{
	this->first = pFirst;
}

template<typename T, typename P>
void TUIPair<T, P>::setSecond(P pSecond)
{
	this->second = pSecond;
}

template<typename T, typename P>
T TUIPair<T, P>::getFirst()
{
	return this->first();
}

template<typename T, typename P>
P TUIPair<T, P>::getSecond()
{
	return this->second;
}

template<typename T, typename P>
std::ostream & TUIPair<T, P>::serialize(std::ostream & os) const
{
	os <<  this->first << "_" <<this->second;
	return os;
}

template<typename T, typename P>
std::istream & TUIPair<T,P>::deSerialize(std::istream & is)
{
	is >> this->first >> this->second;
}
