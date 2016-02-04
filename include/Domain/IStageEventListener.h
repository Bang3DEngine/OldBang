#ifndef ISTAGEEVENTLISTENER_H
#define ISTAGEEVENTLISTENER_H

class IStageEventListener
{
private:
    virtual void _OnStart() = 0;
    virtual void _OnUpdate() = 0;
    virtual void _OnDestroy() = 0;

protected:
    IStageEventListener() {}

    virtual void OnStart() {}
    virtual void OnUpdate() {}
    virtual void OnDestroy() {}
};

#endif // ISTAGEEVENTLISTENER_H
