#ifndef HIDEFLAGS
#define HIDEFLAGS

enum HideFlags
{
    None            = 0,

    HideInHierarchy = 1,
    DontSaveToScene = 2,
    HideInSelection = 4,

    HideAndDontSave = HideInHierarchy | HideInSelection |
                      DontSaveToScene
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

