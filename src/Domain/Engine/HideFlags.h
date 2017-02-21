#ifndef HIDEFLAGS
#define HIDEFLAGS

enum HideFlags
{
    None            = 0,

    HideInHierarchy = 1,
    HideInInspector = 2,
    HideInGame = 2,
    DontSerialize = 4,
    DontClone = 8,
    HideInSelection = 16,
    HideInChildren  = 32,

    HideAndDontSave = HideInHierarchy | HideInInspector |
                      HideInChildren  | HideInSelection |
                      HideInGame |
                      DontSerialize   | DontClone
};


inline HideFlags operator|(HideFlags a, HideFlags b)
{
    return static_cast<HideFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inline HideFlags operator^(HideFlags a, HideFlags b)
{
    return static_cast<HideFlags>(static_cast<int>(a) ^ static_cast<int>(b));
}

#endif // HIDEFLAGS

