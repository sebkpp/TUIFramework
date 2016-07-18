#ifndef _GripperWithLEDUsingMatrix4x4_h_
#define _GripperWithLEDUsingMatrix4x4_h_

#include <tuiframework/client/client.h>
#include <TUIPlugins/tuitypes/common/CommonTypeReg.h>

class GripperWithLEDUsingMatrix4x4 {
public:
    GripperWithLEDUsingMatrix4x4();
    virtual ~GripperWithLEDUsingMatrix4x4();

    //void transfChanged(const Matrix4x4ChangedEvent * event);
    void buttonChanged(const DigitalChangedEvent * event);

protected:
    //Matrix4x4<double> transf;
    bool button;
    tuiframework::IEventChannel * ledChannel;
};

#endif
