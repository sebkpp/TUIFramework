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


#include "SceneGraph.h"

using namespace tuiframework;
using namespace std;

SceneGraphObject::SceneGraphObject() :
    pressure(0) {
}


SceneGraphObject::~SceneGraphObject() {
}


void SceneGraphObject::connect() {
    try {
        CONNECT(AnalogVectorChangedEvent, "HT100", "fIn",
                SceneGraphObject, this, &SceneGraphObject::ht100Changed);
		CONNECT(AnalogVectorChangedEvent, "HT200", "fIn",
			SceneGraphObject, this, &SceneGraphObject::ht200Changed);

    } catch (const Exception & e) {
        cerr << "Exception" << endl;
        cerr << e.getFormattedString() << endl;
    }
}


void SceneGraphObject::disconnect() {
    try {
        DISCONNECT(AnalogVectorChangedEvent, "HT100", "fIn",
			SceneGraphObject, this, &SceneGraphObject::ht100Changed);
		DISCONNECT(AnalogVectorChangedEvent, "HT200", "fIn",
			SceneGraphObject, this, &SceneGraphObject::ht200Changed);
    } catch (const Exception & e) {
        cerr << "Exception" << endl;
        cerr << e.getFormattedString() << endl;
    }
}

void SceneGraphObject::ht100Changed(const AnalogVectorChangedEvent * e) {
	//e->getPayload();
	for (int i = 0;i < e->getPayload().size(); i++)
	{
		cout << "HT100 changed " << e->getPayload().at(i) << endl;

	}
}

void SceneGraphObject::ht200Changed(const AnalogVectorChangedEvent * e) {
	//e->getPayload();
	cout << "HT200 changed "<< endl;
}

