#ifndef STAGEEVENTLISTENER_H
#define STAGEEVENTLISTENER_H

#include <list>
#include <functional>

class StageEventListener
{
private:
    virtual void _OnStart() = 0;
    virtual void _OnUpdate() = 0;
    virtual void _OnDrawing() = 0;
    virtual void _OnDestroy() = 0;

    //void Propagate(function<void()> func, std::iterator begin, std::iterator end);

protected:
    StageEventListener() {}

    virtual void OnStart() {}
    virtual void OnUpdate() {}
    virtual void OnDrawing() {}
    virtual void OnDestroy() {}
};

#endif // STAGEEVENTLISTENER_H
