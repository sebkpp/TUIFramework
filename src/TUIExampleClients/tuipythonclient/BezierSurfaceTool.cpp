#include "BezierSurfaceTool.h"

#include <iostream>

using namespace tuiframework;
using namespace std;

BezierSurfaceTool::BezierSurfaceTool() {
    try {
        CONNECT(TrackerChangedEvent, "MyBezierSurfaceTool", "TransfLeft",
                BezierSurfaceTool, this, &BezierSurfaceTool::tranfLeftChanged);
        CONNECT(TrackerChangedEvent, "MyBezierSurfaceTool", "TransfRight",
                BezierSurfaceTool, this, &BezierSurfaceTool::tranfRightChanged);
        CONNECT(DigitalChangedEvent, "MyBezierSurfaceTool", "Button1",
                BezierSurfaceTool, this, &BezierSurfaceTool::buttonLeftChanged);
        CONNECT(DigitalChangedEvent, "MyBezierSurfaceTool", "Button2",
                BezierSurfaceTool, this, &BezierSurfaceTool::buttonRightChanged);
    } catch (const Exception & e) {
        cerr << "Exception" << endl;
        cerr << e.getFormattedString() << endl;
    }
}


BezierSurfaceTool::~BezierSurfaceTool() {
}


void BezierSurfaceTool::tranfLeftChanged(const TrackerChangedEvent * event) {
    this->transfLeft = event->getPayload();
    cout << "BezierSurfaceTool::tranfLeftChanged: " << this->transfLeft << endl;
    
    this->tryDrawing();
}


void BezierSurfaceTool::tranfRightChanged(const TrackerChangedEvent * event) {
    this->transfRight = event->getPayload();
    cout << "BezierSurfaceTool::tranfRightChanged: " << this->transfRight << endl;

    this->tryDrawing();
}


void BezierSurfaceTool::buttonLeftChanged(const DigitalChangedEvent * event) {
    this->buttonLeft = event->getPayload();
    cout << "BezierSurfaceTool::buttonLeftChanged: " << (this->buttonLeft ? "true" : "false") << endl;

    this->tryDrawing();
    
}


void BezierSurfaceTool::buttonRightChanged(const DigitalChangedEvent * event) {
    this->buttonRight = event->getPayload();
    cout << "BezierSurfaceTool::buttonRightChanged: " << (this->buttonRight ? "true" : "false") << endl;
    
    this->tryDrawing();
}


void BezierSurfaceTool::tryDrawing() {
    if (!this->buttonLeft || !this->buttonRight) {
        return;
    }
    // ok - lets draw

}
