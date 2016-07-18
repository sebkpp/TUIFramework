#ifndef _GripperWithLEDUsingMatrix4_h_
#define _GripperWithLEDUsingMatrix4_h_

#include <tuiframework/client/client.h>
#include <TUIPlugins/tuitypes/common/CommonTypeReg.h>

class GripperWithLEDUsingMatrix4 {
public:
    GripperWithLEDUsingMatrix4();
    virtual ~GripperWithLEDUsingMatrix4();

    void transfChanged(const Matrix4ChangedEvent * event);
    void buttonChanged(const DigitalChangedEvent * event);

protected:
    Matrix4Data transf;
    bool button;
    tuiframework::IEventChannel * ledChannel;
};

#endif
