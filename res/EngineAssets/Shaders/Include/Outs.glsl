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
        struct B_FragmentOut
        {
            vec3  Normal;
            vec4  Diffuse;
            vec4  Color;
            bool  ReceivesLighting;
            float Shininess;
        }
        B_FOut;

        layout(location = B_LOC_GBUFFER_OUT_NORMAL)  out vec4 B_GIn_Normal;
        layout(location = B_LOC_GBUFFER_OUT_DIFFUSE) out vec4 B_GIn_Diffuse;
        layout(location = B_LOC_GBUFFER_OUT_MISC)    out vec4 B_GIn_Misc;
        layout(location = B_LOC_GBUFFER_OUT_COLOR)   out vec4 B_GIn_Color;
    #endif
#endif

#ifdef BANG_SP
    #ifdef BANG_FRAGMENT
        struct B_FragmentOut
        {
            vec4 Color;
        }
        B_FOut;

        layout(location = 0) out vec4 B_GIn_Color;
    #endif
#endif
