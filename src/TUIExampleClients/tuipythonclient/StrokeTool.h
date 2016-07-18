#ifndef _StrokeTool_h_
#define _StrokeTool_h_

#include <tuiframework/client/client.h>
#include <TUIPlugins/tuitypes/common/CommonTypeReg.h>

class StrokeTool {
public:
    StrokeTool();
    virtual ~StrokeTool();

    void transfChanged(const TrackerChangedEvent * event);
    void widthChanged(const AnalogChangedEvent * event);

protected:
    TrackerData transf;
    float width;
};

#endif
