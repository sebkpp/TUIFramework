#ifndef _GripperWithLEDUsingTracker_h_
#define _GripperWithLEDUsingTracker_h_

#include <tuiframework/client/client.h>
#include <TUIPlugins/tuitypes/common/CommonTypeReg.h>

class GripperWithLEDUsingTracker {
public:
    GripperWithLEDUsingTracker();
    virtual ~GripperWithLEDUsingTracker();

    void transfChanged(const TrackerChangedEvent * event);
    void buttonChanged(const AnalogChangedEvent * event);

protected:
    TrackerData transf;
    float button;
    tuiframework::IEventChannel * ledChannel;
};

#endif
