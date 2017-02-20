#ifndef HIDEFLAGS
#define HIDEFLAGS

enum HideFlags
{
    None            = 0,

    HideInHierarchy = 1,
    HideInInspector = 2,
    DontSerialize = 4,
    HideInSelection = 8,
    HideInChildren  = 16,

    HideAndDontSave = HideInHierarchy | HideInInspector |
                      HideInChildren  | HideInSelection |
                      DontSerialize
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

