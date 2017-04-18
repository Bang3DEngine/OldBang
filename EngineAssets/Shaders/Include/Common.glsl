
// Matrices //////////////////////////////
uniform mat4 B_Model;
uniform mat4 B_ModelInv;
uniform mat3 B_Normal;
uniform mat3 B_NormalInv;
uniform mat4 B_View;
uniform mat4 B_ViewInv;
uniform mat4 B_Projection;
uniform mat4 B_ProjectionInv;
uniform mat4 B_PVM;
// ///////////////////////////////////////


// Misc ////////////////////////////////
// Uniform that's always False. Useful to fool glsl optimization.
uniform bool B_False      = false;
uniform bool B_IsSelected = false;
// ///////////////////////////////////////


// Material related /////////////////////////
uniform bool  B_MaterialReceivesLighting;
uniform vec4  B_MaterialDiffuseColor;
uniform float B_AlphaCutoff = -1.0f;
uniform float B_MaterialShininess;
uniform vec2  B_UvMultiply = vec2(1);
uniform bool  B_HasTexture = false;
uniform sampler2D B_Texture0;
// ///////////////////////////////////////



// Screen related /////////////////////////
uniform vec2 B_ScreenSize;
#ifdef BANG_FRAGMENT
vec2 B_ScreenStep;
vec2 B_ScreenPos;
vec2 B_ScreenUv;
#endif
// ///////////////////////////////////////



// Light related /////////////////////////
uniform float B_LightRange;
uniform vec4  B_LightColor;
uniform float B_LightIntensity;
uniform vec3  B_LightForwardWorld;
uniform vec3  B_LightPositionWorld;
// ///////////////////////////////////////



// GBuffer textures //////////////////////
uniform sampler2D  B_GTex_NormalDepth;
uniform sampler2D  B_GTex_DiffColor;
uniform sampler2D  B_GTex_Misc;
uniform sampler2D  B_GTex_Color;
// ///////////////////////////////////////



// In/Out Structures ///////////////////////////
#ifdef BANG_G
    #ifdef BANG_VERTEX
        in vec3 B_In_PositionObject; // Order is important, for vao-vbo location
        in vec3 B_In_NormalObject;
        in vec2 B_In_Uv;

        out vec4 B_FragIn_PositionWorld;
        out vec3 B_FragIn_NormalWorld;
        out vec2 B_FragIn_Uv;
    #endif

    #ifdef BANG_FRAGMENT
        vec3  B_Out_NormalWorld;
        vec4  B_Out_DiffColor;
        vec4  B_Out_Color;
        bool  B_Out_ReceivesLighting;
        float B_Out_Shininess;
        float B_Out_Depth;
        float B_Out_Stencil;

        in vec4 B_FragIn_PositionWorld;
        in vec3 B_FragIn_NormalWorld;
        in vec2 B_FragIn_Uv;

        out vec4 B_GIn_NormalDepth;
        out vec4 B_GIn_DiffColor;
        out vec4 B_GIn_Misc;
        out vec4 B_GIn_Color;
    #endif
#endif

#ifdef BANG_SP
    #ifdef BANG_VERTEX
    #endif

    #ifdef BANG_FRAGMENT
        vec4 B_Out_Color;
        out vec4 B_GIn_Color;
    #endif
#endif
// ///////////////////////////////////////

// GBuffer Samplers //////////////////////
#ifdef BANG_FRAGMENT
    vec4  B_SampleColor(vec2 uv) { return texture2D(B_GTex_Color, uv); }
    vec3  B_SampleNormal(vec2 uv)
    {
        vec3 normXY   = texture2D(B_GTex_NormalDepth, uv).xyz;
        float normalZSign = (normXY.z >= 0.0 ? 1.0 : -1.0);
        float normalZ = sqrt(1.0f - dot(normXY.xy, normXY.xy));
        return vec3(normXY.xy, normalZ * normalZSign);
    }
    vec4  B_SampleDiffColor(vec2 uv) { return texture2D(B_GTex_DiffColor, uv); }
    bool  B_SampleReceivesLight (vec2 uv)
    {
        return texture2D(B_GTex_Misc, uv).r > 0;
    }
    float B_SampleShininess (vec2 uv)
    {
        return texture2D(B_GTex_Misc, uv).g;
    }

    float B_SampleDepth(vec2 uv)
    {
        float depthHigh = texture2D(B_GTex_NormalDepth, uv).z;
        float depthLow  = texture2D(B_GTex_NormalDepth, uv).w;
        float depth     = (abs(depthHigh) + depthLow) / 1024;
        return depth;
    }
    float B_SampleFlags(vec2 uv)
    {
        return texture2D(B_GTex_Misc, uv).z;
    }

    vec4  B_SampleColor()  { return B_SampleColor(B_ScreenUv); }
    vec3  B_SampleNormal() { return B_SampleNormal(B_ScreenUv); }
    vec4  B_SampleDiffColor() { return B_SampleDiffColor(B_ScreenUv); }
    bool  B_SampleReceivesLight() { return B_SampleReceivesLight(B_ScreenUv); }
    float B_SampleShininess () { return B_SampleShininess(B_ScreenUv); }
    float B_SampleDepth() { return B_SampleDepth(B_ScreenUv); }
    float B_SampleFlags() { return B_SampleFlags(B_ScreenUv); }

    vec4  B_SampleColorOffset(vec2 pixOffset)
      { return B_SampleColor(B_ScreenUv + B_ScreenStep * pixOffset); }
    vec3  B_SampleNormalOffset(vec2 pixOffset)
      { return B_SampleNormal(B_ScreenUv + B_ScreenStep * pixOffset); }
    vec4  B_SampleDiffColorOffset(vec2 pixOffset)
      { return B_SampleDiffColor(B_ScreenUv + B_ScreenStep * pixOffset); }
    bool  B_SampleReceivesLightOffset(vec2 pixOffset)
      { return B_SampleReceivesLight(B_ScreenUv + B_ScreenStep * pixOffset); }
    float B_SampleShininessOffset(vec2 pixOffset)
      { return B_SampleShininess(B_ScreenUv + B_ScreenStep * pixOffset); }
    float B_SampleDepthOffset(vec2 pixOffset)
      { return B_SampleDepth(B_ScreenUv + B_ScreenStep * pixOffset); }
    float B_SampleFlagsOffset(vec2 pixOffset)
      { return B_SampleFlags(B_ScreenUv + B_ScreenStep * pixOffset); }

    vec3 B_GetCameraPositionWorld()
    {
        return B_ViewInv[3].xyz;
    }

    vec3 B_ComputeWorldPosition(float depth)
    {
        float x = B_ScreenUv.x * 2.0 - 1.0;
        float y = B_ScreenUv.y * 2.0 - 1.0;
        float z = depth * 2.0 - 1.0;
        vec4 projectedPos = vec4(x, y, z, 1);
        vec4 worldPos = (B_ProjectionInv * projectedPos);
        worldPos = (B_ViewInv * (worldPos/worldPos.w));
        return worldPos.xyz;
    }
    vec3 B_ComputeWorldPosition() { return B_ComputeWorldPosition( B_SampleDepth() ); }
#endif

void InitCommon()
{
    #ifdef BANG_FRAGMENT
    B_ScreenStep = 1.0 / B_ScreenSize;
    B_ScreenPos  = gl_FragCoord.xy;
    B_ScreenUv   = B_ScreenPos / B_ScreenSize;
    #endif
}
// ///////////////////////////////////////


