#include "GripperWithLEDUsingMatrix4x4.h"

#include <iostream>

using namespace tuiframework;
using namespace std;

GripperWithLEDUsingMatrix4x4::GripperWithLEDUsingMatrix4x4() {
    try {
		/*
        CONNECT(Matrix4x4ChangedEvent, "MyGripperWithLEDUsingMatrix4x4", "Transf",
                GripperWithLEDUsingMatrix4x4, this, &GripperWithLEDUsingMatrix4x4::transfChanged);*/
        CONNECT(DigitalChangedEvent, "MyGripperWithLEDUsingMatrix4x4", "Button2",
                GripperWithLEDUsingMatrix4x4, this, &GripperWithLEDUsingMatrix4x4::buttonChanged);

        this->ledChannel = getSinkChannel("MyGripperWithLEDUsingMatrix4x4", "LED");
        if (this->ledChannel == 0) {
            cout << "this->ledChannel == 0" << endl;
        }

    } catch (const Exception & e) {
        cerr << "Exception" << endl;
        cerr << e.getFormattedString() << endl;
    }
/*
    connect<Matrix4ChangedEvent, GripperWithLEDUsingMatrix4x4>(
    	tsrc("MyGripperWithLEDUsingMatrix4x4", "Transf"),
    	this,
    	&GripperWithLEDUsingMatrix4x4::transfChanged);
    connect<DigitalChangedEvent, GripperWithLEDUsingMatrix4x4>(
    	tsrc("MyGripperWithLEDUsingMatrix4x4", "Button"),
    	this,
    	&GripperWithLEDUsingMatrix4x4::buttonChanged);

    this->ledChannel = tsnk("MyGripperWithLEDUsingMatrix4x4", "LED");
    if (this->ledChannel == 0) {
        cout << "this->ledChannel == 0" << endl;
    }
*/
    cout << "GripperWithLEDUsingMatrix4x4()" << endl;
}


GripperWithLEDUsingMatrix4x4::~GripperWithLEDUsingMatrix4x4() {
}

/*
void GripperWithLEDUsingMatrix4x4::transfChanged(const Matrix4x4ChangedEvent * event) {
    this->transf = event->getPayload();
    cout << "GripperWithLEDUsingMatrix4x4::transfChanged: " << this->transf << endl;
}
*/

void GripperWithLEDUsingMatrix4x4::buttonChanged(const DigitalChangedEvent * event) {
    this->button = event->getPayload();
    cout << "GripperWithLEDUsingMatrix4x4::buttonChanged: " << (this->button ? "True" : "False") << endl;

    this->ledChannel->push(new DigitalChangedEvent(-1, -1, this->button));
}

