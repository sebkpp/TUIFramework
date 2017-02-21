#include "HeadtrackTool.h"

#include <iostream>

using namespace tuiframework;
using namespace std;

HeadtrackTool::HeadtrackTool() {

    try {
        CONNECT(Matrix4ChangedEvent, "OurBody", "OurTransformation",
                HeadtrackTool, this, &HeadtrackTool::tranfChanged);
    } catch (const Exception & e) {
        cerr << "Exception" << endl;
        cerr << e.getFormattedString() << endl;
    }
}


HeadtrackTool::~HeadtrackTool() {
}


void HeadtrackTool::tranfChanged(const Matrix4ChangedEvent * event) {
    this->transf = event->getPayload();
    cout << "HeadtrackTool::transfChanged: " << this->transf << endl;
}


