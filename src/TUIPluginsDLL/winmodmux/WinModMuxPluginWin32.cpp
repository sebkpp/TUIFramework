#include <tuiframework/server/MSPFactorySingleton.h>

#include <tuiframework/core/Version.h>
#include <tuiframework/core/Imsp.h>
#include <tuiframework/server/mspConfig.h>
#include <tuiframework/server/IMSPFactory.h>

#include <C:/Users/cadamano/Documents/TUI_return_windMOD_10_02_17/src/TUIPlugins/winmodmux/FloatToFloatPackage.h>
#include <C:/Users/cadamano/Documents/TUI_return_windMOD_10_02_17/src/TUIPlugins/winmodmux/ByteToBytePackage.h>

using namespace tuiframework;

namespace tuiframework {

	static std::vector<std::string> mspTypeNameVector;

	void dllInitPlugin() {
		MSPFactorySingleton::getInstance()->registerCreateFunction(FloatToFloatPackage::getMSPTypeName(), FloatToFloatPackage::createFunction);
		mspTypeNameVector.push_back(FloatToFloatPackage::getMSPTypeName());
		MSPFactorySingleton::getInstance()->registerCreateFunction(ByteToBytePackage::getMSPTypeName(), ByteToBytePackage::createFunction);
		mspTypeNameVector.push_back(ByteToBytePackage::getMSPTypeName());
	}


	std::string dllGetTFVersion() {
		return TUIFRAMEWORK_VERSION;
	}


	std::string dllGetDLLVersion() {
		return "1";
	}


	void dllMSPRegistration(IMSPFactory * MSPFactory) {
		MSPFactory->registerCreateFunction(FloatToFloatPackage::getMSPTypeName(), FloatToFloatPackage::createFunction);
		MSPFactory->registerCreateFunction(ByteToBytePackage::getMSPTypeName(), ByteToBytePackage::createFunction);
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
