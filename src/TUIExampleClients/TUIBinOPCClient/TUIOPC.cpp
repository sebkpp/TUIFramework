/*
Copyright (C) 2010, 2011, 2012 The Fraunhofer Institute for Production Systems and
Design Technology IPK. All rights reserved.

This file is part of the TUIFramework library.
It includes a software framework which contains common code
providing generic functionality for developing applications
with a tangible user interface (TUI).
designed for creating applications with a tangible user interface (TUI).

The TUIFramework library is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

The TUIFramework is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the TUIFramework.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "TUIOPC.h"

using namespace tuiframework;
using namespace std;

TUIOPC::TUIOPC()
{
}


TUIOPC::~TUIOPC() {
}


void TUIOPC::connect() {
	try {
		CONNECT(DigitalChangedOPCEvent, "OPCTUIApp", "dIn",
			TUIOPC, this, &TUIOPC::DigitalValueChanged);
		CONNECT(AnalogChangedOPCEvent, "OPCTUIApp", "aIn",
			TUIOPC, this, &TUIOPC::AnalogValueChanged);
	}
	catch (const Exception & e) {
		cerr << "Exception" << endl;
		cerr << e.getFormattedString() << endl;
	}
}


void TUIOPC::disconnect() {
	try {
		DISCONNECT(DigitalChangedOPCEvent, "OPCTUIApp", "dIn",
			TUIOPC, this, &TUIOPC::DigitalValueChanged);
		DISCONNECT(AnalogChangedOPCEvent, "OPCTUIApp", "aIn",
			TUIOPC, this, &TUIOPC::AnalogValueChanged);
	}
	catch (const Exception & e) {
		cerr << "Exception" << endl;
		cerr << e.getFormattedString() << endl;
	}
}


void TUIOPC::DigitalValueChanged(const DigitalChangedOPCEvent * e)
{
	//cout << "Test" << endl;
	//cout <<"Name: " << e->getPayload().getName() << "Type: " << e->getPayload().getCanonicalDataType() << "Value: " << e->getPayload().getValue().boolVal << endl;
	cout << "Digital: " << e->getPayload() << endl;
}

void TUIOPC::AnalogValueChanged(const AnalogChangedOPCEvent *e)
{
	cout << "Digital: " << e->getPayload() << endl;
}