#ifndef ISCENEEVENTLISTENER_H
#define ISCENEEVENTLISTENER_H

#define PROPAGATE_EVENT(FUNCTION, ITERABLE) do {\
    for (auto it = (ITERABLE).Begin(); it != (ITERABLE).End(); ++it ) \
    {\
        if ((*it)->IsEnabled()) { \
            (*it)->FUNCTION;   \
        } \
    }\
} while (0)

class ISceneEventListener
{
public:
    virtual void Start ()
    {
        if (!IsStarted()) { OnStart(); m_started = true; }
    }

    virtual void Update() { OnUpdate(); }
    virtual void ParentSizeChanged() { OnParentSizeChanged(); }
    virtual void DrawGizmos() { OnDrawGizmos(); }
    virtual void Destroy() { OnDestroy(); }

    bool IsStarted() const { return m_started; }

protected:
    ISceneEventListener () {}
    virtual ~ISceneEventListener() {}

    virtual void OnStart() {}
    virtual void OnUpdate() {}
    virtual void OnParentSizeChanged() {}
    virtual void OnDrawGizmos() {}
    virtual void OnDestroy() {}

private:
    bool m_started = false;
};

#endif // SCENEEVENTLISTENER_H
