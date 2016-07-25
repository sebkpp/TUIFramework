#include "TUIWinMod.h"

using namespace tuiframework;
using namespace std;

TUIWinMod::TUIWinMod(void)
{

}


TUIWinMod::~TUIWinMod(void)
{
}



void TUIWinMod::connect() {
	
    try {
		CONNECT(AnalogVectorChangedEvent, "WinModTUIApp", "fIn", TUIWinMod, this, &TUIWinMod::AnalogSignalChanged);
		CONNECT(IntegerVectorChangedEvent, "WinModTUIApp", "bIn", TUIWinMod, this, &TUIWinMod::DigitalSignalChanged);
		CONNECT(WordChangedEvent, "WinModTUIApp", "wIn", TUIWinMod, this, &TUIWinMod::WordSignalChanged);
		CONNECT(DWordChangedEvent, "WinModTUIApp", "dwIn", TUIWinMod, this, &TUIWinMod::dWordSignalChanged);	

		// Output Sink
		
		this->dOutChannel = getSinkChannel("WinModTUIApp", "bOut");
        if (this->dOutChannel == 0) {
            cout << "this->dOutChannel == 0" << endl;
        }

		this->aOutChannel = getSinkChannel("WinModTUIApp", "fOut");
        if (this->aOutChannel == 0) {
            cout << "this->aOutChannel == 0" << endl;
        }
		
		this->wOutChannel = getSinkChannel("WinModTUIApp", "wOut");
        if (this->wOutChannel == 0) {
            cout << "this->wOutChannel == 0" << endl;
        }
		
		this->dwOutChannel = getSinkChannel("WinModTUIApp", "dwOut");
        if (this->dwOutChannel == 0) {
            cout << "this->DwOutChannel == 0" << endl;
        }
		

    } catch (const Exception & e) {
        cerr << "Exception" << endl;
        cerr << e.getFormattedString() << endl;
    }
	
}


void TUIWinMod::disconnect() {
    try {
		DISCONNECT(AnalogVectorChangedEvent, "WinModTUIApp", "fIn", TUIWinMod, this, &TUIWinMod::AnalogSignalChanged);
		DISCONNECT(IntegerVectorChangedEvent, "WinModTUIApp", "bIn", TUIWinMod, this, &TUIWinMod::DigitalSignalChanged);
		DISCONNECT(WordChangedEvent, "WinModTUIApp", "wIn", TUIWinMod, this, &TUIWinMod::WordSignalChanged);
		DISCONNECT(DWordChangedEvent, "WinModTUIApp", "dwIn", TUIWinMod, this, &TUIWinMod::dWordSignalChanged);	

    } catch (const Exception & e) {
        cerr << "Exception" << endl;
        cerr << e.getFormattedString() << endl;
    }
}


void TUIWinMod::WordSignalChanged(const WordChangedEvent *e)
{
	
	this->wordValues = e->getPayload();
	WordChangedEvent *wordChangeEvent = new WordChangedEvent(-1,-1,wordValues);
	this->wOutChannel->push(wordChangeEvent);

	//delete wordChangeEvent;
	//delete e;
}


void TUIWinMod::dWordSignalChanged(const DWordChangedEvent *e)
{
	this->dWordValues = e->getPayload();
	DWordChangedEvent *dWordChangeEvent = new DWordChangedEvent(-1,-1,dWordValues);
	this->dwOutChannel->push(dWordChangeEvent);

	//delete dWordChangeEvent;
	//delete e;
}



void TUIWinMod::AnalogSignalChanged(const AnalogVectorChangedEvent * e) {
	
	this->floatValues = e->getPayload();
	
	/*
	for(int i = 0; i < e->getPayload().size(); i++)
	{
		cout << "value:" << i << " " << e->getPayload().getData().at(i) << endl;
	}
	*/
	//AnalogVectorChangedEvent* analogChangeEvent = new AnalogVectorChangedEvent(-1, -1, floatValues);
	//this->aOutChannel->push(analogChangeEvent);
	
	//delete analogChangeEvent;
	//delete e;
}

/*
void TUIWinMod::MatrixSignalChanged(const PackedMatrix4Event * e) {

	//cout << "Test" << endl;
	//cout << e->getPayload().getFloats().getData().size()<< endl; 
	cout << "matrix: " << e->getPayload()<< endl;
	//cout << e->getPayload().getData().second << endl;
	//cout << "Size: "<< this->floatValues.size() << endl;
	//cout << "Size: "<< floatValue.size() << endl;


	//cout << floatValues.getData()[0] << endl;
	//AnalogVectorChangedEvent* analogChangeEvent = new AnalogVectorChangedEvent(-1, -1, floatValues);
	//this->aOutChannel->push(analogChangeEvent);

	//delete analogChangeEvent;
	//delete e;
}
*/
/*
void TUIWinMod::Matrix1SignalChanged(const Matrix4Event * e) {

	//cout << "Test" << endl;
	//cout << e->getPayload().getFloats().getData().size()<< endl; 
	cout << "matrix: " << e->getPayload() << endl;
	//cout << e->getPayload().getData().second << endl;
	//cout << "Size: "<< this->floatValues.size() << endl;
	//cout << "Size: "<< floatValue.size() << endl;
	//
	//for(int i=0;i<floatValues.size();i++)
	//{
	//cout << "Analog Signal: " << floatValues.at(i) << endl;
	//}
	//

	//cout << floatValues.getData()[0] << endl;
	//AnalogVectorChangedEvent* analogChangeEvent = new AnalogVectorChangedEvent(-1, -1, floatValues);
	//this->aOutChannel->push(analogChangeEvent);

	//delete analogChangeEvent;
	//delete e;
}
*/

void TUIWinMod::DigitalSignalChanged(const IntegerVectorChangedEvent * e) {
	//this->byteValues = e->getPayload();
	
	
	//Integer View for Debug

	//for(int i=0;i<byteValues.size();i++)
	//{
	//	cout << "Byte Signal: " << (byteValues.at(i)) << endl;
	//}
	
	// Hex View for Debug
	//cout <<"Hex: " << hex << e->getPayload() << endl;
	

	//IntegerVectorChangedEvent* byteChangeEvent = new IntegerVectorChangedEvent(-1, -1, byteValues);
	//this->dOutChannel->push(byteChangeEvent);

	//delete byteChangeEvent;
	//delete e;
}
