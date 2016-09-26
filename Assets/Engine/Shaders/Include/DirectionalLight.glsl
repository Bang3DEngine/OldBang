vec3 GetDirectionalLightColorApportation(vec3  pixelPosWorld,
                                         vec3  pixelNormalWorld,
                                         vec3  pixelDiffColor,
                                         float pixelShininess,
                                         vec3  lightForwardWorld,
                                         float lightIntensity,
                                         vec3  lightColor,
                                         vec3  camPosWorld)
{
    float lightDot     = dot(pixelNormalWorld, -lightForwardWorld);
    if (lightDot <= 0.0f) discard;

    // DIFFUSE
    vec3 lightDiffuse  = pixelDiffColor * lightDot * lightIntensity * lightColor;

    // SPECULAR
    vec3 worldCamPos     = camPosWorld;
    vec3 pointToCamDir   = normalize(worldCamPos - pixelPosWorld);
    vec3 lightToPointDir = -lightForwardWorld;
    vec3 reflected       = -reflect(lightToPointDir, pixelNormalWorld);
    float specDot        = max(0.0, dot(reflected, pointToCamDir));
    float specShin       = pow(specDot, pixelShininess);
    specShin = min(specShin, 1.0);
    vec3 lightSpecular   = specShin * lightDot * lightIntensity * lightColor;

    return lightDiffuse + lightSpecular;
}
