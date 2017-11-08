// Ins location definition
#define B_LOC_VIN_POSITION 0
#define B_LOC_VIN_NORMAL   1
#define B_LOC_VIN_UV       2

// In/Out Structures
#ifdef BANG_G
    #ifdef BANG_VERTEX
        layout(location = B_LOC_VIN_POSITION) in vec3 B_VIn_Position;
        layout(location = B_LOC_VIN_NORMAL)   in vec3 B_VIn_Normal;
        layout(location = B_LOC_VIN_UV)       in vec2 B_VIn_Uv;
    #endif

    #ifdef BANG_FRAGMENT
        in vec3 B_FIn_Position;
        in vec3 B_FIn_Normal;
        in vec2 B_FIn_Uv;
    #endif
#endif

#ifdef BANG_SP
    #ifdef BANG_FRAGMENT
    #endif
#endif
