#include "StrokeTool.h"

#include <iostream>

using namespace tuiframework;
using namespace std;

StrokeTool::StrokeTool() {

    try {
        CONNECT(TrackerChangedEvent, "MyStrokeTool", "Transf",
                StrokeTool, this, &StrokeTool::transfChanged);
        CONNECT(AnalogChangedEvent, "MyStrokeTool", "Width",
                StrokeTool, this, &StrokeTool::widthChanged);
    } catch (const Exception & e) {
        cerr << "Exception" << endl;
        cerr << e.getFormattedString() << endl;
    }
}


StrokeTool::~StrokeTool() {
}


void StrokeTool::transfChanged(const TrackerChangedEvent * event) {
    this->transf = event->getPayload();
    cout << "StrokeTool::transfChanged: " << this->transf << endl;
}


void StrokeTool::widthChanged(const AnalogChangedEvent * event) {
    this->width = event->getPayload();
    cout << "StrokeTool::widthChanged: " << this->width << endl;
}

