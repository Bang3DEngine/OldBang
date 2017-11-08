// Outs location definition
#define B_LOC_GBUFFER_OUT_NORMAL  0
#define B_LOC_GBUFFER_OUT_DIFFUSE 1
#define B_LOC_GBUFFER_OUT_MISC    2
#define B_LOC_GBUFFER_OUT_COLOR   3

// In/Out Structures
#ifdef BANG_G
    #ifdef BANG_VERTEX
        out vec3 B_FIn_Position;
        out vec3 B_FIn_Normal;
        out vec2 B_FIn_Uv;
    #endif

    #ifdef BANG_FRAGMENT
        vec3  B_Out_NormalWorld;
        vec4  B_Out_DiffColor;
        vec4  B_Out_Color;
        bool  B_Out_ReceivesLighting;
        float B_Out_Shininess;

        layout(location = B_LOC_GBUFFER_OUT_NORMAL)  out vec4 B_FOut_Normal;
        layout(location = B_LOC_GBUFFER_OUT_DIFFUSE) out vec4 B_FOut_Diffuse;
        layout(location = B_LOC_GBUFFER_OUT_MISC)    out vec4 B_FOut_Misc;
        layout(location = B_LOC_GBUFFER_OUT_COLOR)   out vec4 B_FOut_Color;
    #endif
#endif

#ifdef BANG_SP
    #ifdef BANG_FRAGMENT
        vec4 B_Out_Color;
        out vec4 B_FOut_Color;
    #endif
#endif
