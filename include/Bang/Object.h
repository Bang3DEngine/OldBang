#ifndef OBJECT_H
#define OBJECT_H

class Object
{
public:
    virtual ~Object();

    virtual void Start();
    virtual void OnStart();

    void SetEnabled(bool enabled);

    bool IsEnabled() const;
    bool IsStarted() const;

protected:
    Object();

private:
    bool m_enabled = true;
    bool m_started = false;
};

#endif // OBJECT_H
