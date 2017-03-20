#include "TUIXController.h"

using namespace tuiframework;
using namespace std;

TUIXController::TUIXController() {
}


TUIXController::~TUIXController() {
}


void TUIXController::connect() {
	try {
		CONNECT(DigitalChangedEvent, "TUIXController1", "A-Button",
			TUIXController, this, &TUIXController::aButtonChanged);
		CONNECT(DigitalChangedEvent, "TUIXController1", "B-Button",
			TUIXController, this, &TUIXController::bButtonChanged);
		CONNECT(DigitalChangedEvent, "TUIXController1", "X-Button",
			TUIXController, this, &TUIXController::xButtonChanged);
		CONNECT(DigitalChangedEvent, "TUIXController1", "Y-Button",
			TUIXController, this, &TUIXController::yButtonChanged);
		CONNECT(DigitalChangedEvent, "TUIXController1", "Start-Button",
			TUIXController, this, &TUIXController::startButtonChanged);
		CONNECT(DigitalChangedEvent, "TUIXController1", "Back-Button",
			TUIXController, this, &TUIXController::backButtonChanged);
		CONNECT(DigitalChangedEvent, "TUIXController1", "Left-Shoulder-Button",
			TUIXController, this, &TUIXController::leftShoulderButtonChanged);
		CONNECT(DigitalChangedEvent, "TUIXController1", "Right-Shoulder-Button",
			TUIXController, this, &TUIXController::rightShoulderButtonChanged);
		CONNECT(DigitalChangedEvent, "TUIXController1", "Left-ThumbStick",
			TUIXController, this, &TUIXController::leftThumbstickButtonChanged);
		CONNECT(DigitalChangedEvent, "TUIXController1", "Right-ThumbStick",
			TUIXController, this, &TUIXController::rightThumbstickButtonChanged);
		CONNECT(DigitalChangedEvent, "TUIXController1", "dPad-Down",
			TUIXController, this, &TUIXController::dPadDownButtonChanged);
		CONNECT(DigitalChangedEvent, "TUIXController1", "dPad-Up",
			TUIXController, this, &TUIXController::dPadUpButtonChanged);
		CONNECT(DigitalChangedEvent, "TUIXController1", "dPad-Right",
			TUIXController, this, &TUIXController::dPadRightButtonChanged);
		CONNECT(DigitalChangedEvent, "TUIXController1", "dPad-Left",
			TUIXController, this, &TUIXController::dPadLeftButtonChanged);

		CONNECT(IntegerChangedEvent, "TUIXController1", "LShoulder-Delta",
			TUIXController, this, &TUIXController::leftShoulderDelta_Changed);
		CONNECT(IntegerChangedEvent, "TUIXController1", "RShoulder-Delta",
			TUIXController, this, &TUIXController::rightShoulderDelta_Changed);

		CONNECT(IntegerChangedEvent, "TUIXController1", "LThumbStick-DeltaX",
			TUIXController, this, &TUIXController::leftThumbDelta_X_Changed);
		CONNECT(IntegerChangedEvent, "TUIXController1", "LThumbStick-DeltaY",
			TUIXController, this, &TUIXController::leftThumbDelta_Y_Changed);
		CONNECT(IntegerChangedEvent, "TUIXController1", "RThumbStick-DeltaX",
			TUIXController, this, &TUIXController::rightThumbDelta_X_Changed);
		CONNECT(IntegerChangedEvent, "TUIXController1", "RThumbStick-DeltaY",
			TUIXController, this, &TUIXController::rightThumbDelta_Y_Changed);
	}
	catch (const Exception & e) {
		cerr << "Exception" << endl;
		cerr << e.getFormattedString() << endl;
	}
}


void TUIXController::disconnect() {
	try {
		DISCONNECT(DigitalChangedEvent, "TUIXController1", "A-Button",
			TUIXController, this, &TUIXController::aButtonChanged);
		DISCONNECT(DigitalChangedEvent, "TUIXController1", "B-Button",
			TUIXController, this, &TUIXController::bButtonChanged);
		DISCONNECT(DigitalChangedEvent, "TUIXController1", "X-Button",
			TUIXController, this, &TUIXController::xButtonChanged);
		DISCONNECT(DigitalChangedEvent, "TUIXController1", "Y-Button",
			TUIXController, this, &TUIXController::yButtonChanged);
		DISCONNECT(DigitalChangedEvent, "TUIXController1", "Start-Button",
			TUIXController, this, &TUIXController::startButtonChanged);
		DISCONNECT(DigitalChangedEvent, "TUIXController1", "Back-Button",
			TUIXController, this, &TUIXController::backButtonChanged);
		DISCONNECT(DigitalChangedEvent, "TUIXController1", "Left-Shoulder-Button",
			TUIXController, this, &TUIXController::leftShoulderButtonChanged);
		DISCONNECT(DigitalChangedEvent, "TUIXController1", "Right-Shoulder-Button",
			TUIXController, this, &TUIXController::rightShoulderButtonChanged);
		DISCONNECT(DigitalChangedEvent, "TUIXController1", "Left-Thumbstick",
			TUIXController, this, &TUIXController::leftThumbstickButtonChanged);
		DISCONNECT(DigitalChangedEvent, "TUIXController1", "Right-Thumbstick",
			TUIXController, this, &TUIXController::rightThumbstickButtonChanged);
		DISCONNECT(DigitalChangedEvent, "TUIXController1", "dPad-Down",
			TUIXController, this, &TUIXController::dPadDownButtonChanged);
		DISCONNECT(DigitalChangedEvent, "TUIXController1", "dPad-Up",
			TUIXController, this, &TUIXController::dPadUpButtonChanged);
		DISCONNECT(DigitalChangedEvent, "TUIXController1", "dPad-Right",
			TUIXController, this, &TUIXController::dPadRightButtonChanged);
		DISCONNECT(DigitalChangedEvent, "TUIXController1", "dPad-Left",
			TUIXController, this, &TUIXController::dPadLeftButtonChanged);

		DISCONNECT(IntegerChangedEvent, "TUIXController1", "LShoulder-Delta",
			TUIXController, this, &TUIXController::leftShoulderDelta_Changed);
		DISCONNECT(IntegerChangedEvent, "TUIXController1", "RShoulder-Delta",
			TUIXController, this, &TUIXController::rightShoulderDelta_Changed);

		DISCONNECT(IntegerChangedEvent, "TUIXController1", "LThumbStick-DeltaX",
			TUIXController, this, &TUIXController::leftThumbDelta_X_Changed);
		DISCONNECT(IntegerChangedEvent, "TUIXController1", "LThumbStick-DeltaY",
			TUIXController, this, &TUIXController::leftThumbDelta_Y_Changed);
		DISCONNECT(IntegerChangedEvent, "TUIXController1", "RThumbStick-DeltaX",
			TUIXController, this, &TUIXController::rightThumbDelta_X_Changed);
		DISCONNECT(IntegerChangedEvent, "TUIXController1", "RThumbStick-DeltaY",
			TUIXController, this, &TUIXController::rightThumbDelta_Y_Changed);

	}
	catch (const Exception & e) {
		cerr << "Exception" << endl;
		cerr << e.getFormattedString() << endl;
	}
}

void TUIXController::aButtonChanged(const DigitalChangedEvent * e)
{
	cout << "A-Button: " << e->getPayload() << endl;
}

void TUIXController::bButtonChanged(const DigitalChangedEvent * e)
{
	cout << "B-Button: " << e->getPayload() << endl;
}

void TUIXController::xButtonChanged(const DigitalChangedEvent * e)
{
	cout << "X-Button: " << e->getPayload() << endl;
}

void TUIXController::yButtonChanged(const DigitalChangedEvent * e)
{
	cout << "Y-Button: " << e->getPayload() << endl;
}

void TUIXController::startButtonChanged(const DigitalChangedEvent* e)
{
	cout << "Start-Button: " << e->getPayload() << endl;
}

void TUIXController::backButtonChanged(const DigitalChangedEvent* e)
{
	cout << "Back-Button: " << e->getPayload() << endl;
}

void TUIXController::leftShoulderButtonChanged(const DigitalChangedEvent* e)
{
	cout << "LeftShoulder-Button: " << e->getPayload() << endl;
}

void TUIXController::rightShoulderButtonChanged(const DigitalChangedEvent* e)
{
	cout << "RightShoulder-Button: " << e->getPayload() << endl;
}

void TUIXController::dPadDownButtonChanged(const DigitalChangedEvent* e)
{
	cout << "dPad-Down: " << e->getPayload() << endl;
}

void TUIXController::dPadUpButtonChanged(const DigitalChangedEvent* e)
{
	cout << "dPad-Up: " << e->getPayload() << endl;
}

void TUIXController::dPadLeftButtonChanged(const DigitalChangedEvent* e)
{
	cout << "dPad-Left: " << e->getPayload() << endl;
}

void TUIXController::dPadRightButtonChanged(const DigitalChangedEvent* e)
{
	cout << "dPad-Right: " << e->getPayload() << endl;
}

void TUIXController::leftThumbDelta_X_Changed(const IntegerChangedEvent* e)
{
	cout << "Left-Thumbstick-X: " << e->getPayload() << endl;
}

void TUIXController::leftThumbDelta_Y_Changed(const IntegerChangedEvent* e)
{
	cout << "Left-Thumbstick-Y: " << e->getPayload() << endl;
}

void TUIXController::rightThumbDelta_X_Changed(const IntegerChangedEvent* e)
{
	cout << "Right-Thumbstick-X: " << e->getPayload() << endl;
}

void TUIXController::rightThumbDelta_Y_Changed(const IntegerChangedEvent* e)
{
	cout << "Right-Thumbstick-Y: " << e->getPayload() << endl;
}

void TUIXController::leftShoulderDelta_Changed(const IntegerChangedEvent* e)
{
	cout << "Left-Shoulder-Delta: " << e->getPayload() << endl;
}

void TUIXController::rightShoulderDelta_Changed(const IntegerChangedEvent* e)
{
	cout << "Right-Shoulder-Delta: " << e->getPayload() << endl;
}

void TUIXController::leftThumbstickButtonChanged(const DigitalChangedEvent* e)
{
	cout << "Left-Thumbstick-Button: " << e->getPayload() << endl;
}

void TUIXController::rightThumbstickButtonChanged(const DigitalChangedEvent* e)
{
	cout << "Right-Thumbstick-Button: " << e->getPayload() << endl;
}
