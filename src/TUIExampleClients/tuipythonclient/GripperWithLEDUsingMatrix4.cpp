#include "GripperWithLEDUsingMatrix4.h"

#include <iostream>

using namespace tuiframework;
using namespace std;

GripperWithLEDUsingMatrix4::GripperWithLEDUsingMatrix4() {
    try {
        CONNECT(Matrix4ChangedEvent, "MyGripperWithLEDUsingMatrix4", "Transf",
                GripperWithLEDUsingMatrix4, this, &GripperWithLEDUsingMatrix4::transfChanged);
        CONNECT(DigitalChangedEvent, "MyGripperWithLEDUsingMatrix4", "Button",
                GripperWithLEDUsingMatrix4, this, &GripperWithLEDUsingMatrix4::buttonChanged);

        this->ledChannel = getSinkChannel("MyGripperWithLEDUsingMatrix4", "LED");
        if (this->ledChannel == 0) {
            cout << "this->ledChannel == 0" << endl;
        }

    } catch (const Exception & e) {
        cerr << "Exception" << endl;
        cerr << e.getFormattedString() << endl;
    }
/*
    connect<Matrix4ChangedEvent, GripperWithLEDUsingMatrix4>(
    	tsrc("MyGripperWithLEDUsingMatrix4", "Transf"),
    	this,
    	&GripperWithLEDUsingMatrix4::transfChanged);
    connect<DigitalChangedEvent, GripperWithLEDUsingMatrix4>(
    	tsrc("MyGripperWithLEDUsingMatrix4", "Button"),
    	this,
    	&GripperWithLEDUsingMatrix4::buttonChanged);

    this->ledChannel = tsnk("MyGripperWithLEDUsingMatrix4", "LED");
    if (this->ledChannel == 0) {
        cout << "this->ledChannel == 0" << endl;
    }
*/
    cout << "GripperWithLEDUsingMatrix4()" << endl;
}


GripperWithLEDUsingMatrix4::~GripperWithLEDUsingMatrix4() {
}


void GripperWithLEDUsingMatrix4::transfChanged(const Matrix4ChangedEvent * event) {
    this->transf = event->getPayload();
    cout << "GripperWithLEDUsingMatrix4::transfChanged: " << this->transf << endl;
}


void GripperWithLEDUsingMatrix4::buttonChanged(const DigitalChangedEvent * event) {
    this->button = event->getPayload();
    cout << "GripperWithLEDUsingMatrix4::buttonChanged: " << (this->button ? "True" : "False") << endl;

    this->ledChannel->push(new DigitalChangedEvent(-1, -1, this->button));
}

