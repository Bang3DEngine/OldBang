#ifndef ENTITY_H
#define ENTITY_H

#include "Part.h"
#include <list>

class Entity
{
private:
    std::list<Part*> parts;

public:
    Entity();

    template <class T>
    T* AddPart()
    {
        T *part = new T();
        parts.push_back(part);
        return part;
    }

    template <class T>
    T* GetPart()
    {
        for(auto part = parts.begin(); part != parts.end(); ++part)
        {
            T *tp = dynamic_cast<T>(*part);
            if(tp != nullptr)
            {
                return tp;
            }
        }
    }

    template <class T>
    void RemovePart()
    {
        for(auto part = parts.begin(); part != parts.end(); ++part)
        {
            T *tp = dynamic_cast<T>(*part);
            if(tp != nullptr)
            {
                part = parts.erase(part);
            }
        }
    }

    const std::list<Part*>* GetParts() { return &parts; }
};

#endif // ENTITY_H
