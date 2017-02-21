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


#include <TUIPlugins/matrix4op/Matrix4MultiplicationMSP.h>

#include <tuiframework/server/ParameterGroup.h>
#include <tuiframework/core/Exception.h>
#define USE_TFDEBUG
#include <tuiframework/logging/Logger.h>

static const char * inATag = "A";
static const char * inAPackedTag = "AP";
static const char * inBTag = "B";
static const char * inBPackedTag = "BP";
static const char * outABTag = "A*B";
static const char * outAPackedBTag = "AP*B";
static const char * outABPackedTag = "A*BP";

using namespace std;

namespace tuiframework {

IMSP * Matrix4MultiplicationMSP::createFunction(void * arg) {
    MSPConfig * config = static_cast<MSPConfig *>(arg);
    return new Matrix4MultiplicationMSP(*config);
}

static std::string typeName = "Matrix4Multiplication";

const std::string & Matrix4MultiplicationMSP::getMSPTypeName() {
    return typeName;
}


Matrix4MultiplicationMSP::Matrix4MultiplicationMSP(const MSPConfig & config) :
    config(config),
    outAB(0),
    outAPackedB(0),
    outABPacked(0) {
    
    this->eventDelegateA.setReceiver(this, &Matrix4MultiplicationMSP::handleA);
    this->eventDelegateB.setReceiver(this, &Matrix4MultiplicationMSP::handleB);
    this->eventDelegateAP.setReceiver(this, &Matrix4MultiplicationMSP::handleAP);
    this->eventDelegateBP.setReceiver(this, &Matrix4MultiplicationMSP::handleBP);
    
    try {
        this->initMatrix(this->config.getParameterGroup().getParameterGroup("A"), this->a);
        this->initMatrix(this->config.getParameterGroup().getParameterGroup("B"), this->b);
        TFINFO("initial matrix A:" << this->a);
        TFINFO("initial matrix B:" << this->b);
        this->triggerA = this->config.getParameterGroup().getParameterGroup("multiply").getInt("triggerA") != 0;
        this->triggerB = this->config.getParameterGroup().getParameterGroup("multiply").getInt("triggerB") != 0;
        TFINFO("triggerA = " << this->triggerA);
        TFINFO("triggerB = " << this->triggerB);
    }
    catch (Exception & e) {
        e.addErrorMessage("in Matrix4MultiplicationMSP.", __FILE__, __LINE__);
        TFERROR(e.getFormattedString());
    }
}


Matrix4MultiplicationMSP::~Matrix4MultiplicationMSP() {
}


const std::string & Matrix4MultiplicationMSP::getTypeName() const {
    return getMSPTypeName();
}


IEventSink * Matrix4MultiplicationMSP::getEventSink(const std::string & name) {
    if (name.compare(inATag) == 0) {
        return &this->eventDelegateA;
    } else if (name.compare(inBTag) == 0) {
        return &this->eventDelegateB;
    } else if (name.compare(inAPackedTag) == 0) {
        return &this->eventDelegateAP;
    } else if (name.compare(inBPackedTag) == 0) {
        return &this->eventDelegateBP;
    } else {
        TFERROR("")
    }
        
    return 0;
}


void Matrix4MultiplicationMSP::registerEventSink(const std::string & name, IEventSink * eventSink) {
    if (name.compare(outABTag) == 0) {
        this->outAB = eventSink;
    } else if (name.compare(outAPackedBTag) == 0) {
        this->outAPackedB = eventSink;
    } else if (name.compare(outABPackedTag) == 0) {
        this->outABPacked = eventSink;
    } else {
        TFERROR("")
    }
}


const MSPType & Matrix4MultiplicationMSP::getMSPType() const {
    return this->type;
}


void Matrix4MultiplicationMSP::handleA(Matrix4Event * e) {    
    this->a = e->getPayload();
    if (this->outAB && this->triggerA) {
        this->outAB->push(new Matrix4Event(-1, -1, this->a*this->b));
    }
    
    delete e;
}


void Matrix4MultiplicationMSP::handleB(Matrix4Event * e) {    
    this->b = e->getPayload();
    if (this->outAB && this->triggerB) {
        this->outAB->push(new Matrix4Event(-1, -1, this->a*this->b));
    }
    
    delete e;
}


void Matrix4MultiplicationMSP::handleAP(PackedMatrix4Event * e) {
    if (this->outAPackedB) {
        const PackedType<Matrix4<double> > & p = e->getPayload();
        const vector<pair<int, Matrix4<double> > > & items = p.getItems();
        
        vector<pair<int, Matrix4<double> > >::const_iterator i = items.begin();
        vector<pair<int, Matrix4<double> > >::const_iterator e = items.end();

        PackedType<Matrix4<double> > p2;
        vector<pair<int, Matrix4<double> > > & items2 = p2.getItems();

        while (i != e) {
            items2.push_back(pair<int, Matrix4<double> >((*i).first, (*i).second*this->b));
            ++i;
        }

        this->outAPackedB->push(new PackedMatrix4Event(-1, -1, p2));
    }
    
    delete e;
}


void Matrix4MultiplicationMSP::handleBP(PackedMatrix4Event * e) {    
    if (this->outABPacked) {
        const PackedType<Matrix4<double> > & p = e->getPayload();
        const vector<pair<int, Matrix4<double> > > & items = p.getItems();
        
        vector<pair<int, Matrix4<double> > >::const_iterator i = items.begin();
        vector<pair<int, Matrix4<double> > >::const_iterator e = items.end();

        PackedType<Matrix4<double> > p2;
        vector<pair<int, Matrix4<double> > > & items2 = p2.getItems();

        while (i != e) {
            items2.push_back(pair<int, Matrix4<double> >((*i).first, this->a*(*i).second));
            ++i;
        }
        
        this->outABPacked->push(new PackedMatrix4Event(-1, -1, p2));
    }

    delete e;
}


void Matrix4MultiplicationMSP::initMatrix(const ParameterGroup & parameterGroup, Matrix4<double> & mat) {
    mat[0][0] = parameterGroup.getParameterGroup("row1").getDouble("col1");
    mat[0][1] = parameterGroup.getParameterGroup("row1").getDouble("col2");
    mat[0][2] = parameterGroup.getParameterGroup("row1").getDouble("col3");
    mat[0][3] = parameterGroup.getParameterGroup("row1").getDouble("col4");
    mat[1][0] = parameterGroup.getParameterGroup("row2").getDouble("col1");
    mat[1][1] = parameterGroup.getParameterGroup("row2").getDouble("col2");
    mat[1][2] = parameterGroup.getParameterGroup("row2").getDouble("col3");
    mat[1][3] = parameterGroup.getParameterGroup("row2").getDouble("col4");
    mat[2][0] = parameterGroup.getParameterGroup("row3").getDouble("col1");
    mat[2][1] = parameterGroup.getParameterGroup("row3").getDouble("col2");
    mat[2][2] = parameterGroup.getParameterGroup("row3").getDouble("col3");
    mat[2][3] = parameterGroup.getParameterGroup("row3").getDouble("col4");
    mat[3][0] = parameterGroup.getParameterGroup("row4").getDouble("col1");
    mat[3][1] = parameterGroup.getParameterGroup("row4").getDouble("col2");
    mat[3][2] = parameterGroup.getParameterGroup("row4").getDouble("col3");
    mat[3][3] = parameterGroup.getParameterGroup("row4").getDouble("col4");
}


}


