
uniform float B_LightShadowBias;
uniform sampler2D B_LightShadowMap;
uniform mat4 B_WorldToShadowMapMatrix;

float GetFragmentShadowness(const in vec3 pixelPosWorld)
{
    vec2 shadowMapUv = (B_WorldToShadowMapMatrix * vec4(pixelPosWorld,1)).xy * 0.5f + 0.5f;
    vec3 worldPosInLightSpace = (B_WorldToShadowMapMatrix * vec4(pixelPosWorld,1)).xyz;

    float shadowMapDepth = texture(B_LightShadowMap, shadowMapUv).r;
    float worldPosDepthFromLightSpace = worldPosInLightSpace.z * 0.5f + 0.5f;

    float shadowDiff = (worldPosDepthFromLightSpace - shadowMapDepth);
    if (shadowDiff > B_LightShadowBias)
    {
        return 1.0f;
    }
    return 0.0f;
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
