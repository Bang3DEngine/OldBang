#ifndef IGUIDABLE_H
#define IGUIDABLE_H

#include "Bang/GUID.h"

NAMESPACE_BANG_BEGIN

class IGUIDable
{
public:
	IGUIDable();
	virtual ~IGUIDable();

    const GUID& GetGUID() const;

protected:
    void SetGUID(const GUID &guid);

private:
    GUID m_GUID;
};

NAMESPACE_BANG_END

#endif // IGUIDABLE_H

