#pragma once

#include "../../TUIPlugins/tuitypes/common/CommonTypeReg.h"

class TUIXController
{
public:
	TUIXController();
	~TUIXController();

	void connect();
	void disconnect();

	void aButtonChanged(const DigitalChangedEvent * e);
	void bButtonChanged(const DigitalChangedEvent * e);
	void xButtonChanged(const DigitalChangedEvent * e);
	void yButtonChanged(const DigitalChangedEvent * e);
	void startButtonChanged(const DigitalChangedEvent * e);
	void backButtonChanged(const DigitalChangedEvent * e);
	void leftShoulderButtonChanged(const DigitalChangedEvent * e);
	void rightShoulderButtonChanged(const DigitalChangedEvent * e);
	void leftThumbstickButtonChanged(const DigitalChangedEvent * e);
	void rightThumbstickButtonChanged(const DigitalChangedEvent * e);
	void dPadDownButtonChanged(const DigitalChangedEvent * e);
	void dPadUpButtonChanged(const DigitalChangedEvent * e);
	void dPadLeftButtonChanged(const DigitalChangedEvent * e);
	void dPadRightButtonChanged(const DigitalChangedEvent * e);

	void leftThumbDelta_X_Changed(const IntegerChangedEvent * e);
	void leftThumbDelta_Y_Changed(const IntegerChangedEvent * e);
	void rightThumbDelta_X_Changed(const IntegerChangedEvent * e);
	void rightThumbDelta_Y_Changed(const IntegerChangedEvent * e);

	void leftShoulderDelta_Changed(const IntegerChangedEvent * e);
	void rightShoulderDelta_Changed(const IntegerChangedEvent * e);
};

