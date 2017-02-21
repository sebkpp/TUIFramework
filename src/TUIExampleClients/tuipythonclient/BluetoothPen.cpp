#include "BluetoothPen.h"

#include <iostream>

using namespace tuiframework;
using namespace std;

BluetoothPen::BluetoothPen() :
        menueButton(false),
        analogueButton(0) {
    try {
		/*
        CONNECT(Matrix4x4ChangedEvent, "MyBluetoothPen", "Transf",
                BluetoothPen, this, &BluetoothPen::transfChanged);*/
        CONNECT(DigitalChangedEvent, "MyBluetoothPen", "MenueButton",
                BluetoothPen, this, &BluetoothPen::menueButtonChanged);
        CONNECT(AnalogChangedEvent, "MyBluetoothPen", "AnalogueButton",
                BluetoothPen, this, &BluetoothPen::analogueButtonChanged);

        this->led = getSinkChannel("MyBluetoothPen", "LED");
        if (this->led == 0) {
            cout << "this->ledChannel == 0" << endl;
        }

    } catch (const Exception & e) {
        cerr << "Exception" << endl;
        cerr << e.getFormattedString() << endl;
    }
}


BluetoothPen::~BluetoothPen() {
}

/*
void BluetoothPen::transfChanged(const Matrix4x4ChangedEvent * event) {
    this->transf = event->getPayload();
    cout << "BluetoothPen::transfChanged: " << this->transf << endl;
}
*/

void BluetoothPen::menueButtonChanged(const DigitalChangedEvent * event) {
    this->menueButton = event->getPayload();
    cout << "BluetoothPen::menueButtonChanged: " << (this->menueButton ? "True" : "False") << endl;

    this->led->push(new DigitalChangedEvent(-1, -1, this->menueButton));
}


void BluetoothPen::analogueButtonChanged(const AnalogChangedEvent * event) {
    this->analogueButton = event->getPayload();
    cout << "BluetoothPen::analogueButtonChanged: " << this->analogueButton << endl;
}

