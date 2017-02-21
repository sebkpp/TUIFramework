#include "GripperWithLEDUsingTracker.h"

#include <iostream>

using namespace tuiframework;
using namespace std;

GripperWithLEDUsingTracker::GripperWithLEDUsingTracker() {

    try {
        CONNECT(TrackerChangedEvent, "MyGripperWithLEDUsingTracker", "Transf",
                GripperWithLEDUsingTracker, this, &GripperWithLEDUsingTracker::transfChanged);

        CONNECT(AnalogChangedEvent, "MyGripperWithLEDUsingTracker", "Button",
                GripperWithLEDUsingTracker, this, &GripperWithLEDUsingTracker::buttonChanged);

        this->ledChannel = getSinkChannel("MyGripperWithLEDUsingTracker", "LED");
        if (this->ledChannel == 0) {
            cout << "this->ledChannel == 0" << endl;
        }

    } catch (const Exception & e) {
        cerr << "Exception" << endl;
        cerr << e.getFormattedString() << endl;
    }
}


GripperWithLEDUsingTracker::~GripperWithLEDUsingTracker() {
}


void GripperWithLEDUsingTracker::transfChanged(const TrackerChangedEvent * event) {
    this->transf = event->getPayload();
    cout << "GripperWithLEDUsingTracker::transfChanged: " << this->transf << endl;
}


void GripperWithLEDUsingTracker::buttonChanged(const AnalogChangedEvent * event) {
    this->button = event->getPayload();
    cout << "GripperWithLEDUsingTracker::buttonChanged: " << this->button << endl;

    this->ledChannel->push(new DigitalChangedEvent(-1, -1, this->button));
}


