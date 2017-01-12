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

    return lightDiffuse + lightSpecular;
}
