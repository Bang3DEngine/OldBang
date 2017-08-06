#ifndef ISCENEEVENTLISTENER_H
#define ISCENEEVENTLISTENER_H

#include "Bang/List.h"

#define PROPAGATE_EVENT(FUNCTION, ITERABLE) do {\
    for (auto it = (ITERABLE).Begin(); it != (ITERABLE).End(); ++it ) \
    {\
        if ((*it)->IsEnabled()) { \
            (*it)->FUNCTION;   \
        } \
    }\
} while (0)

template<class T>
class SceneNode
{
public:
    T* const& parent = p_parent;

    virtual void Start ()
    {
        PROPAGATE_EVENT(Start(), m_children);
        if (!IsStarted()) { OnStart(); m_started = true; }
    }

    virtual void Update()
    {
        PROPAGATE_EVENT(Update(), m_children);
        OnUpdate();
    }

    virtual void ParentSizeChanged()
    {
        PROPAGATE_EVENT(ParentSizeChanged(), m_children);
        OnParentSizeChanged();
    }

    virtual void DrawGizmos()
    {
        PROPAGATE_EVENT(DrawGizmos(), m_children);
        OnDrawGizmos();
    }

    virtual void Destroy()
    {
        PROPAGATE_EVENT(Destroy(), m_children);
        OnDestroy();
    }

    bool IsStarted() const
    {
        return m_started;
    }

    // Children functions
    const List<T*>& GetChildren() const
    {
        return m_children;
    }

    T* GetChild(int index) const
    {
        auto it = GetChildren().Begin(); std::advance(it, index);
        return *it;
    }

    List<T*> GetChildrenRecursively() const
    {
        List<T*> cc;
        for (T *c : m_children)
        {
            cc.PushBack(c);
            List<T*> childChildren = c->GetChildrenRecursively();
            cc.Splice(cc.Begin(), childChildren);
        }
        return cc;
    }

    bool IsChildOf(const T *_parent, bool recursive = true) const
    {
        if (!parent) { return false; }

        if (recursive)
        {
            return parent == _parent || parent->IsChildOf(_parent);
        }
        return parent == _parent;
    }

    void SetParent(SceneNode<T> *newParent, int _index = -1)
    {
        if (parent) { parent->m_children.Remove( SCAST<T*>(this) ); }

        p_parent = SCAST<T*>(newParent);
        if (parent)
        {
            int index = (_index != -1 ? _index : parent->m_children.Size());
            parent->m_children.Insert(index, SCAST<T*>(this));
            SceneNode<T>::ParentSizeChanged();
        }
    }

    T* GetParent() const { return p_parent; }

protected:
    SceneNode () {}
    virtual ~SceneNode() {}

    virtual void OnStart() {}
    virtual void OnUpdate() {}
    virtual void OnParentSizeChanged() {}
    virtual void OnDrawGizmos() {}
    virtual void OnDestroy() {}

protected:
    List<T*> m_children;
    T* p_parent = nullptr;
    bool m_started = false;
};

#endif // SCENEEVENTLISTENER_H
