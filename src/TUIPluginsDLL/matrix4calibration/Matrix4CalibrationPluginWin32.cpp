#include  "../../tuiframework/server/DeviceFactorySingleton.h"

#include  "../../tuiframework/core/Version.h"
#include  "../../tuiframework/core/IDevice.h"
#include  "../../tuiframework/server/DeviceConfig.h"
#include  "../../tuiframework/server/IDeviceFactory.h"

#include "../../TUIPlugins/matrix4calibration/Matrix4CalibrationDev.h"

using namespace tuidevices;
using namespace tuiframework;

namespace tuiframework {

static std::vector<std::string> deviceTypeNameVector;

void dllInitPlugin() {
	DeviceFactorySingleton::getInstance()->registerCreateFunction(Matrix4CalibrationDev::getDeviceName(), Matrix4CalibrationDev::createFunction);
    deviceTypeNameVector.push_back(Matrix4CalibrationDev::getDeviceName());
}


std::string dllGetTFVersion() {
    return TUIFRAMEWORK_VERSION;
}


std::string dllGetDLLVersion() {
    return "1";
}


void dllDeviceRegistration(IDeviceFactory * deviceFactory) {
    deviceFactory->registerCreateFunction(Matrix4CalibrationDev::getDeviceName(), Matrix4CalibrationDev::createFunction);
}


const std::vector<std::string> & dllGetDeviceTypeNameVector() {
    return deviceTypeNameVector;
}


std::string dllGetDeviceTypeVersion(const std::string & deviceTypeName) {
    return "1";
}


IDevice * dllNewDeviceInstance(const std::string & deviceTypeName, DeviceConfig & deviceConfig) {
    return DeviceFactorySingleton::getInstance()->createInstance(deviceTypeName, &deviceConfig);
}


void dllDeleteDeviceInstance(IDevice * device) {
    delete device;
}


}
