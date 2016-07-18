#ifndef _BezierSurfaceTool_h_
#define _BezierSurfaceTool_h_

#include <tuiframework/client/client.h>
#include <TUIPlugins/tuitypes/common/CommonTypeReg.h>

class BezierSurfaceTool {
public:
    BezierSurfaceTool();
    virtual ~BezierSurfaceTool();

    void tranfLeftChanged(const TrackerChangedEvent * event);
    void tranfRightChanged(const TrackerChangedEvent * event);
    void buttonLeftChanged(const DigitalChangedEvent * event);
    void buttonRightChanged(const DigitalChangedEvent * event);

protected:
    void tryDrawing();

protected:
    TrackerData transfLeft;
    TrackerData transfRight;
    bool buttonLeft;
    bool buttonRight;
};

#endif
