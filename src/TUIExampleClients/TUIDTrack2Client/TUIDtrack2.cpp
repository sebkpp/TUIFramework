#include "TUIDtrack2.h"

using namespace tuiframework;
using namespace std;

TUIDtrack2::TUIDtrack2() {
}


TUIDtrack2::~TUIDtrack2() {
}


void TUIDtrack2::connect() {
    try {
        CONNECT(Matrix4ChangedEvent, "TUIDtrack2Instance", "OurTransformation",
                TUIDtrack2, this, &TUIDtrack2::ourTransformationChanged);

    } catch (const Exception & e) {
        cerr << "Exception" << endl;
        cerr << e.getFormattedString() << endl;
    }
}


void TUIDtrack2::disconnect() {
    try {
        DISCONNECT(Matrix4ChangedEvent, "TUIDtrack2Instance", "OurTransformation",
                   TUIDtrack2, this, &TUIDtrack2::ourTransformationChanged);

    } catch (const Exception & e) {
        cerr << "Exception" << endl;
        cerr << e.getFormattedString() << endl;
    }
}


void TUIDtrack2::ourTransformationChanged(const Matrix4ChangedEvent * e) {
	cout << "TUIDtrack2: ourTransformationChanged: " << e->getPayload() << endl;
}
