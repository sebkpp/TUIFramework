#ifndef _BluetoothPen_h_
#define _BluetoothPen_h_

#include <tuiframework/client/client.h>
//#include "client.h"
#include <TUIPlugins/tuitypes/common/CommonTypeReg.h>


class BluetoothPen {
public:
    BluetoothPen();
    virtual ~BluetoothPen();

    //void transfChanged(const tuiframework::Matrix4x4ChangedEvent * event);
    void menueButtonChanged(const DigitalChangedEvent * event);
    void analogueButtonChanged(const AnalogChangedEvent * event);

protected:
    //tuiframework::Matrix4x4<double> transf;
    bool menueButton;
    float analogueButton;
    tuiframework::IEventChannel * led;
};

#endif
