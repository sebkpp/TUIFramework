#include <TUIPlugins\tuitypes\common\WinModData.h>

#include <iostream>

WinModData::WinModData()
{
	//this->dWordValues = new VectorList<unsigned long>();
	//this->wordValues = new VectorList<unsigned short>();
	this->floatValues = VectorList<float>();
	
	//this->floatValues = VectorList<float>();
	//this->floatValues = Pointer<float*>();
	//this->byteValues = new VectorList<int>();
};

WinModData::~WinModData() {
}



void WinModData::setDWords(VectorList<unsigned long> dwordVector)
{
	dWordValues = dwordVector;
}
void WinModData::setDWords(unsigned long* dWords, int dWordSize)
{
	dWordValues.setData(dWords,dWordSize);
}


void WinModData::setWords(VectorList<unsigned short> wordVector)
{
	wordValues = wordVector;
}

void WinModData::setWords(unsigned short* words,int wordSize)
{
	wordValues.setData(words,wordSize);
}

void WinModData::setFloats(VectorList<float> floatVector)
{
	floatValues = floatVector;
}


void WinModData::setFloats(float* floats,int floatSize)
{
	floatValues.setData(floats,floatSize);
}

void WinModData::setBytes(VectorList<int> byteVector){
	byteValues = byteVector;
}

void WinModData::setBytes(int* bytes,int byteSize)
{
	byteValues.setData(bytes,byteSize);
}

VectorList<float> WinModData::getFloats() const
{
	return floatValues;
}


VectorList<unsigned long> WinModData::getDWords() const
{
	return dWordValues;
}

VectorList<unsigned short> WinModData::getWords() const
{
	return wordValues;
}

VectorList<int> WinModData::getBytes() const
{
	return byteValues;
}


std::ostream & WinModData::serialize(std::ostream & os) const {
	
	os  << 1 ;
	
	return os;
	
}


std::istream & WinModData::deSerialize(std::istream & is) {
	int version;
	is >> version;

	return is;
	
}