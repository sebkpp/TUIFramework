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

#include <tuiframework/server/MSPFactorySingleton.h>

#include <tuiframework/core/Version.h>
#include <tuiframework/core/Imsp.h>
#include <tuiframework/server/mspConfig.h>
#include <tuiframework/server/IMSPFactory.h>

#include <TUIPlugins/typeconverter/AnalogToDigitalMSP.h>
#include <TUIPlugins/typeconverter/DigitalToAnalogMSP.h>
#include <TUIPlugins/typeconverter/TrackerToMatrix4MSP.h>
#include <TUIPlugins/typeconverter/PackedByteToByteMSP.h>
#include <TUIPlugins/typeconverter/FloatToPackedFloatMSP.h>
#include <TUIPlugins/typeconverter/ByteToPackedByteMSP.h>
#include <TUIPlugins/typeconverter/PackedFloatToFloatMSP.h>
#include <TUIPlugins/typeconverter/PackedWordToWordMSP.h>
#include <TUIPlugins/typeconverter/PackedDWordToDWordMSP.h>

#include <vector>

/**
 *  TypeConverterPluginWin32
 *
 *  \author Oliver Belaifa
 *  \date 2013
 */ 

using namespace tuiframework;

namespace tuiframework {

static std::vector<std::string> mspTypeNameVector;

void dllInitPlugin() {
	MSPFactorySingleton::getInstance()->registerCreateFunction(AnalogToDigitalMSP::getMSPTypeName(), AnalogToDigitalMSP::createFunction);
    mspTypeNameVector.push_back(AnalogToDigitalMSP::getMSPTypeName());

	MSPFactorySingleton::getInstance()->registerCreateFunction(DigitalToAnalogMSP::getMSPTypeName(), DigitalToAnalogMSP::createFunction);
    mspTypeNameVector.push_back(DigitalToAnalogMSP::getMSPTypeName());
	
	MSPFactorySingleton::getInstance()->registerCreateFunction(TrackerToMatrix4MSP::getMSPTypeName(), TrackerToMatrix4MSP::createFunction);
    mspTypeNameVector.push_back(TrackerToMatrix4MSP::getMSPTypeName());

	MSPFactorySingleton::getInstance()->registerCreateFunction(PackedByteToByteMSP::getMSPTypeName(), PackedByteToByteMSP::createFunction);
	mspTypeNameVector.push_back(PackedByteToByteMSP::getMSPTypeName());

	MSPFactorySingleton::getInstance()->registerCreateFunction(PackedFloatToFloatMSP::getMSPTypeName(), PackedFloatToFloatMSP::createFunction);
	mspTypeNameVector.push_back(PackedFloatToFloatMSP::getMSPTypeName());

	MSPFactorySingleton::getInstance()->registerCreateFunction(PackedWordToWordMSP::getMSPTypeName(), PackedWordToWordMSP::createFunction);
	mspTypeNameVector.push_back(PackedWordToWordMSP::getMSPTypeName());

	MSPFactorySingleton::getInstance()->registerCreateFunction(PackedDWordToDWordMSP::getMSPTypeName(), PackedDWordToDWordMSP::createFunction);
	mspTypeNameVector.push_back(PackedDWordToDWordMSP::getMSPTypeName());
	
	MSPFactorySingleton::getInstance()->registerCreateFunction(FloatToPackedFloatMSP::getMSPTypeName(), FloatToPackedFloatMSP::createFunction);
	mspTypeNameVector.push_back(FloatToPackedFloatMSP::getMSPTypeName());

	MSPFactorySingleton::getInstance()->registerCreateFunction(ByteToPackedByteMSP::getMSPTypeName(), ByteToPackedByteMSP::createFunction);
	mspTypeNameVector.push_back(ByteToPackedByteMSP::getMSPTypeName());

}


std::string dllGetTFVersion() {
    return TUIFRAMEWORK_VERSION;
}


std::string dllGetDLLVersion() {
    return "1";
}


void dllMSPRegistration(IMSPFactory * MSPFactory) {
    MSPFactory->registerCreateFunction(AnalogToDigitalMSP::getMSPTypeName(), AnalogToDigitalMSP::createFunction);

	MSPFactory->registerCreateFunction(DigitalToAnalogMSP::getMSPTypeName(), DigitalToAnalogMSP::createFunction);
	MSPFactory->registerCreateFunction(TrackerToMatrix4MSP::getMSPTypeName(), TrackerToMatrix4MSP::createFunction);
	MSPFactory->registerCreateFunction(FloatToPackedFloatMSP::getMSPTypeName(), FloatToPackedFloatMSP::createFunction);
	MSPFactory->registerCreateFunction(ByteToPackedByteMSP::getMSPTypeName(), ByteToPackedByteMSP::createFunction);

	MSPFactory->registerCreateFunction(PackedByteToByteMSP::getMSPTypeName(), PackedByteToByteMSP::createFunction);
	MSPFactory->registerCreateFunction(PackedFloatToFloatMSP::getMSPTypeName(), PackedFloatToFloatMSP::createFunction);
	MSPFactory->registerCreateFunction(PackedWordToWordMSP::getMSPTypeName(), PackedWordToWordMSP::createFunction);
	MSPFactory->registerCreateFunction(PackedDWordToDWordMSP::getMSPTypeName(), PackedDWordToDWordMSP::createFunction);

}


const std::vector<std::string> & dllGetMSPTypeNameVector() {
    return mspTypeNameVector;
}


std::string dllGetMSPTypeVersion(const std::string & mspTypeName) {
    return "1";
}


IMSP * dllNewMSPInstance(const std::string & mspTypeName, MSPConfig & mspConfig) {
    return MSPFactorySingleton::getInstance()->createInstance(mspTypeName, &mspConfig);
}


void dllDeleteMSPInstance(IMSP * msp) {
    delete msp;
}


}
