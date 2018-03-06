
const int SHADOW_NONE = 0;
const int SHADOW_HARD = 1;

uniform int B_ShadowType;
uniform float B_LightShadowBias;
// uniform sampler2D B_LightShadowMap;
uniform sampler2DShadow B_LightShadowMap;
uniform mat4 B_WorldToShadowMapMatrix;

float GetFragmentLightness(const in vec3 pixelPosWorld)
{
    if (B_ShadowType == SHADOW_NONE) { return 1.0f; }

    // Get uvs in shadow map, and sample the shadow map depth
    vec2 shadowMapUv = (B_WorldToShadowMapMatrix * vec4(pixelPosWorld,1)).xy * 0.5f + 0.5f;
    // float shadowMapDepth = texture(B_LightShadowMap, shadowMapUv).r;

    // Get
    vec3 worldPosInLightSpace = (B_WorldToShadowMapMatrix * vec4(pixelPosWorld,1)).xyz;
    float worldPosDepthFromLightSpace = worldPosInLightSpace.z * 0.5f + 0.5f;
    float biasedWorldDepth = (worldPosDepthFromLightSpace - B_LightShadowBias);

    // Get the PCF value from 0 to 1
    float lightness = texture(B_LightShadowMap,
                              vec3(shadowMapUv, biasedWorldDepth));

    // Return depending on shadow type either (0 or 1), or [0.0, 1.0];
    return (B_ShadowType == SHADOW_HARD) ? step(0.5f, lightness) : lightness;
}

vec3 GetDirectionalLightColorApportation(const in vec3  pixelPosWorld,
                                         const in vec3  pixelNormalWorld,
                                         const in vec3  pixelDiffColor,
                                         const in float pixelShininess,
                                         const in vec3  lightForwardWorld,
                                         const in float lightIntensity,
                                         const in vec3  lightColor,
                                         const in vec3  camPosWorld)
{
    float lightDot     = max(0.0, dot(pixelNormalWorld, -lightForwardWorld));

    // DIFFUSE
    vec3 lightDiffuse  = pixelDiffColor * lightDot * lightIntensity * lightColor;

    // SPECULAR
    vec3 worldCamPos     = camPosWorld;
    vec3 pointToCamDir   = normalize(worldCamPos - pixelPosWorld);
    vec3 reflected       = -reflect(-lightForwardWorld, pixelNormalWorld);
    float specDot        = max(0.0, dot(reflected, pointToCamDir));
    float specShin       = min(pow(specDot, pixelShininess), 1.0);
    vec3 lightSpecular   = specShin * lightDot * lightIntensity * lightColor;
    lightSpecular *= 0.5f;

    return lightDiffuse + lightSpecular;
}
