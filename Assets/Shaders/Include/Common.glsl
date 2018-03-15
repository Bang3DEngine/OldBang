uniform mat4 B_Model;
uniform mat4 B_Normal;
uniform mat4 B_View;
uniform mat4 B_Projection;
uniform mat4 B_ProjectionView;
uniform mat4 B_PVM;

uniform float B_Camera_ZNear;
uniform float B_Camera_ZFar;

uniform vec2 B_Viewport_MinPos;
uniform vec2 B_Viewport_Size;

// Material related /////////////////////////
uniform bool  B_MaterialReceivesLighting;
uniform vec4  B_MaterialDiffuseColor;
uniform float B_AlphaCutoff;
uniform float B_MaterialShininess;
uniform vec2  B_UvOffset;
uniform vec2  B_UvMultiply;
uniform bool  B_HasTexture;
uniform sampler2D B_Texture0;
// ///////////////////////////////////////

// Light related /////////////////////////
uniform float B_AmbientLight;
uniform float B_LightRange;
uniform vec4  B_LightColor;
uniform float B_LightIntensity;
uniform vec3  B_LightForwardWorld;
uniform vec3  B_LightPositionWorld;
// ///////////////////////////////////////


// GBuffer textures //////////////////////
uniform sampler2D B_GTex_Normal;
uniform sampler2D B_GTex_DiffColor;
uniform sampler2D B_GTex_Misc;
uniform sampler2D B_GTex_Color;
uniform sampler2D B_GTex_DepthStencil;
// ///////////////////////////////////////

// Util functions /////////////////
float B_LinearizeDepth(float d)
{
    return (2 * B_Camera_ZNear) /
                (B_Camera_ZFar + B_Camera_ZNear -
                  d * (B_Camera_ZFar - B_Camera_ZNear));
}

#ifdef BANG_FRAGMENT
vec2 B_GetViewportStep() { return 1.0 / B_Viewport_Size; }
vec2 B_GetViewportPos() { return gl_FragCoord.xy - B_Viewport_MinPos; }
vec2 B_GetViewportUv() { return B_GetViewportPos() / B_Viewport_Size; }
#endif
//

// GBuffer Samplers //////////////////////
#ifdef BANG_FRAGMENT
vec4  B_SampleColor(vec2 uv) { return texture(B_GTex_Color, uv); }
vec3  B_SampleNormal(vec2 uv)
{
    return texture(B_GTex_Normal, uv).xyz * 2.0f - 1.0f;
}
vec4  B_SampleDiffColor(vec2 uv) { return texture(B_GTex_DiffColor, uv); }
bool  B_SampleReceivesLight (vec2 uv) { return texture(B_GTex_Misc, uv).r > 0; }
float B_SampleShininess (vec2 uv) { return texture(B_GTex_Misc, uv).g; }
float B_SampleDepth(vec2 uv) { return texture(B_GTex_DepthStencil, uv).r; }
float B_SampleFlags(vec2 uv) { return texture(B_GTex_Misc, uv).z; }

vec4  B_SampleColor()  { return B_SampleColor(B_GetViewportUv()); }
vec3  B_SampleNormal() { return B_SampleNormal(B_GetViewportUv()); }
vec4  B_SampleDiffColor() { return B_SampleDiffColor(B_GetViewportUv()); }
bool  B_SampleReceivesLight() { return B_SampleReceivesLight(B_GetViewportUv()); }
float B_SampleShininess () { return B_SampleShininess(B_GetViewportUv()) * 255.0f; }
float B_SampleDepth() { return B_SampleDepth(B_GetViewportUv()); }
float B_SampleFlags() { return B_SampleFlags(B_GetViewportUv()); }

vec4  B_SampleColorOffset(vec2 pixOffset)
  { return B_SampleColor(B_GetViewportUv() + B_GetViewportStep() * pixOffset); }
vec3  B_SampleNormalOffset(vec2 pixOffset)
  { return B_SampleNormal(B_GetViewportUv() + B_GetViewportStep() * pixOffset); }
vec4  B_SampleDiffColorOffset(vec2 pixOffset)
  { return B_SampleDiffColor(B_GetViewportUv() + B_GetViewportStep() * pixOffset); }
bool  B_SampleReceivesLightOffset(vec2 pixOffset)
  { return B_SampleReceivesLight(B_GetViewportUv() + B_GetViewportStep() * pixOffset); }
float B_SampleShininessOffset(vec2 pixOffset)
  { return B_SampleShininess(B_GetViewportUv() + B_GetViewportStep() * pixOffset); }
float B_SampleDepthOffset(vec2 pixOffset)
  { return B_SampleDepth(B_GetViewportUv() + B_GetViewportStep() * pixOffset); }
float B_SampleFlagsOffset(vec2 pixOffset)
  { return B_SampleFlags(B_GetViewportUv() + B_GetViewportStep() * pixOffset); }

vec3 B_GetCameraPositionWorld() { return inverse(B_View)[3].xyz; }

vec3 B_ComputeWorldPosition(float depth, vec2 uv)
{
    float x = uv.x * 2.0 - 1.0;
    float y = uv.y * 2.0 - 1.0;
    float z = depth * 2.0 - 1.0;
    vec4 projectedPos = vec4(x, y, z, 1);
    vec4 worldPos = (inverse(B_Projection) * projectedPos);
    worldPos = (inverse(B_View) * (worldPos/worldPos.w));
    return worldPos.xyz;
}
vec3 B_ComputeWorldPosition(float depth)
{
    return B_ComputeWorldPosition(depth, B_GetViewportUv());
}
vec3 B_ComputeWorldPosition() { return B_ComputeWorldPosition( B_SampleDepth() ); }
#endif
// ///////////////////////////////////////


