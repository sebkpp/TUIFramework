#include "TUICsharp.h"

TUICsharp::TUICsharp() {}

TUICsharp::~TUICsharp() {}

void TUICsharp::connect() {
	try {
		// Durchläuft den Vector mit gespeicherten Strukturen
		for (std::vector<listValues>::iterator it = list.begin(); it != list.end(); ++it) {
			// Verbindet sich je nach TUITypeID
			switch (it->tuiType) {
			case 11:
				CONNECT(DigitalChangedEvent, it->objectName, it->channelName, TUICsharp, this, &TUICsharp::SignalChanged);
				break;
			case 12:
				CONNECT(AnalogChangedEvent, it->objectName, it->channelName, TUICsharp, this, &TUICsharp::SignalChanged);
				break;
			case 13:
				CONNECT(IntegerChangedEvent, it->objectName, it->channelName, TUICsharp, this, &TUICsharp::SignalChanged);
				break;
			case 21:
				CONNECT(MouseEvent, it->objectName, it->channelName, TUICsharp, this, &TUICsharp::SignalChanged);
				break;
			case 25:
				CONNECT(Matrix4Event, it->objectName, it->channelName, TUICsharp, this, &TUICsharp::SignalChanged);
				break;
			default:
				break;
			}
		}

	} catch (const Exception & e) {
		cerr << "Exception" << endl;
		cerr << e.getFormattedString() << endl;
	}

}

void TUICsharp::disconnect() {
	try {
		// Durchläuft den Vector mit gespeicherten Strukturen
		for (std::vector<listValues>::iterator it = list.begin(); it != list.end(); ++it) {
			// Verbindet sich je nach TUITypeID
			switch (it->tuiType) {
			case 11:
				DISCONNECT(DigitalChangedEvent, it->objectName, it->channelName, TUICsharp, this, &TUICsharp::SignalChanged);
				break;
			case 12:
				DISCONNECT(AnalogChangedEvent, it->objectName, it->channelName, TUICsharp, this, &TUICsharp::SignalChanged);
				break;
			case 13:
				DISCONNECT(IntegerChangedEvent, it->objectName, it->channelName, TUICsharp, this, &TUICsharp::SignalChanged);
				break;
			case 21:
				DISCONNECT(MouseEvent, it->objectName, it->channelName, TUICsharp, this, &TUICsharp::SignalChanged);
				break;
			case 25:
				DISCONNECT(Matrix4Event, it->objectName, it->channelName, TUICsharp, this, &TUICsharp::SignalChanged);
				break;
			default:
				break;
			}
		}

	} catch (const Exception & e) {
		cerr << "Exception" << endl;
		cerr << e.getFormattedString() << endl;
	}
}

void TUICsharp::connecting(int TUIType, std::string TUIObjectName, std::string portName, integerCallback call) {
	listVal.tuiType = TUIType;
	listVal.objectName = TUIObjectName;
	listVal.channelName = portName;
	listVal.intCall = call;

	// Fügt die Struktur der Liste hinzu.
	list.push_back(listVal);
}

void TUICsharp::connecting(int TUIType, std::string TUIObjectName, std::string portName, floatCallback call) {
	listVal.tuiType = TUIType;
	listVal.objectName = TUIObjectName;
	listVal.channelName = portName;
	listVal.floatCall = call;

	// Fügt die Struktur der Liste hinzu.
	list.push_back(listVal);
}

void TUICsharp::connecting(int TUIType, std::string TUIObjectName, std::string portName, boolCallback call) {
	listVal.tuiType = TUIType;
	listVal.objectName = TUIObjectName;
	listVal.channelName = portName;
	listVal.boolCall = call;

	// Fügt die Struktur der Liste hinzu.
	list.push_back(listVal);
}

void TUICsharp::connecting(int TUIType, std::string TUIObjectName, std::string portName, mouseCallback call) {
	listVal.tuiType = TUIType;
	listVal.objectName = TUIObjectName;
	listVal.channelName = portName;
	listVal.mouseCall = call;

	// Fügt die Struktur der Liste hinzu.
	list.push_back(listVal);
}

void TUICsharp::connecting(int TUIType, std::string TUIObjectName, std::string portName, matrix4Callback call) {
	listVal.tuiType = TUIType;
	listVal.objectName = TUIObjectName;
	listVal.channelName = portName;
	listVal.matrix4Call = call;

	// Fügt die Struktur der Liste hinzu.
	list.push_back(listVal);
}



void TUICsharp::SignalChanged(const IntegerChangedEvent * e) {
	for (int i = 0; i < list.size(); ++i) {
		if (i == (e->getAddress().getPortNr() - 1)) {
			// Ruft die Callback-Funktion auf
			list.at(i).intCall(e->getPayload());
		}
	}
}

void TUICsharp::SignalChanged(const DigitalChangedEvent * e) {
	SendDebugMessage(11223344); // temp test
	for (int i = 0; i < list.size(); ++i) {
		if (i == (e->getAddress().getPortNr() - 1)) {
			// Ruft die Callback-Funktion auf
			list.at(i).boolCall(e->getPayload());
		}
	}
}

void TUICsharp::SignalChanged(const AnalogChangedEvent * e) {
	for (int i = 0; i < list.size(); ++i) {
		if (i == (e->getAddress().getPortNr() - 1)) {
			// Ruft die Callback-Funktion auf
			list.at(i).floatCall(e->getPayload());
		}
	}
}

void TUICsharp::SignalChanged(const MouseEvent * e) {
	for (int i = 0; i < list.size(); ++i) {
		if (i == (e->getAddress().getPortNr() - 1)) {
			// Ruft die Callback-Funktion auf
			list.at(i).mouseCall(e->getPayload());
		}
	}
}

void TUICsharp::SignalChanged(const Matrix4Event * e) {
	for (int i = 0; i < list.size(); ++i) {
		if (i == (e->getAddress().getPortNr() - 1)) {
			// Ruft die Callback-Funktion auf
			list.at(i).matrix4Call(e->getPayload());
		}
	}
}

int TUICsharp::getParameterCount() {
	return list.size();
}

void TUICsharp::SendDebugMessage(int i) {
	debugMessages.push(i);
}