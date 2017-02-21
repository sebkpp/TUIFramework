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


// old stuff, will be deleted
void WinModData::setDWords(VectorList<unsigned long> dwordVector)
{
	dWordValues = dwordVector;
}

void WinModData::setDWords(DWORD* dWords, int dWordSize)
{
	vector<pair<int, DWORD>> dwordVector;
	for (int i = 0; i < dWordSize; ++i)
	{
		dwordVector.push_back(pair<int, int>(i, dWords[i]));
	}
	this->dWordValues2.setItems(dwordVector);

	//dWordValues2.assign(dWords, dWords + dWordSize);

	// old stuff, will be deleted
	dWordValues.setData(dWords,dWordSize);
}

// old stuff, will be deleted
void WinModData::setWords(VectorList<unsigned short> wordVector)
{
	wordValues = wordVector;
}

void WinModData::setWords(WORD* words,int wordSize)
{

	vector<pair<int, WORD>> wordVector;
	for (int i = 0; i < wordSize; ++i)
	{
		wordVector.push_back(pair<int, int>(i, words[i]));
	}
	this->wordValues2.setItems(wordVector);


	//wordValues2.assign(words, words + wordSize);

	// old stuff, will be deleted
	wordValues.setData(words,wordSize);
}

// old stuff, will be deleted
void WinModData::setFloats(VectorList<float> floatVector)
{
	floatValues = floatVector;
}


void WinModData::setFloats(float* floats,int floatSize)
{
	vector<pair<int, float>> floatVector;
	for (int i = 0; i < floatSize; ++i)
	{
		floatVector.push_back(pair<int, int>(i, floats[i]));
	}
	this->floatValues2.setItems(floatVector);

	//floatValues2.assign(floats, floats + floatSize);
	
	// old stuff, will be deleted
	floatValues.setData(floats,floatSize);
}

// old stuff, will be deleted
void WinModData::setBytes(VectorList<int> byteVector){
	byteValues = byteVector;
}

void WinModData::setBytes(int* bytes,int byteSize)
{
	vector<pair<int, int>> byteVector;
	for (int i = 0; i < byteSize; ++i)
	{
		byteVector.push_back(pair<int, int>(i, bytes[i]));
	}
	this->byteValues2.setItems(byteVector);

	//byteValues2.assign(bytes, bytes + byteSize);
	// old stuff, will be deleted
	byteValues.setData(bytes,byteSize);
}

// old stuff, will be deleted
VectorList<float> WinModData::getFloats() const
{
	return floatValues;
}

// old stuff, will be deleted
VectorList<unsigned long> WinModData::getDWords() const
{
	return dWordValues;
}
// old stuff, will be deleted
VectorList<unsigned short> WinModData::getWords() const
{
	return wordValues;
}
// old stuff, will be deleted
VectorList<int> WinModData::getBytes() const
{
	return byteValues;
}

PackedType<int> WinModData::getByteList() const
{
	return byteValues2;
}

PackedType<float> WinModData::getFloatList() const
{
	return floatValues2;
}

PackedType<WORD> WinModData::getWordList() const
{
	return wordValues2;
}

PackedType<DWORD> WinModData::getDWordList() const
{
	return dWordValues2;
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