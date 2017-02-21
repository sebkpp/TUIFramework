#ifndef _HeadtrackTool_h_
#define _HeadtrackTool_h_

#include <tuiframework/client/client.h>
#include <TUIPlugins/tuitypes/common/CommonTypeReg.h>

class HeadtrackTool {
public:
    HeadtrackTool();
    virtual ~HeadtrackTool();

    void tranfChanged(const Matrix4ChangedEvent * event);

protected:
    Matrix4Data transf;
};

#endif
