vec3 GetDirectionalLightColorApportation(vec3  pixelPosWorld,
                                         vec3  pixelNormalWorld,
                                         vec3  pixelDiffColor,
                                         float pixelShininess,
                                         vec3  lightForwardWorld,
                                         float lightIntensity,
                                         vec3  lightColor,
                                         vec3  camPosWorld)
{
    float lightDot     = max(0.0f, dot(pixelNormalWorld, normalize(-lightForwardWorld)));

    // DIFFUSE
    vec3 lightDiffuse  = pixelDiffColor * lightDot * lightIntensity * lightColor;

    // SPECULAR
    vec3 worldCamPos     = camPosWorld;
    vec3 pointToCamDir   = normalize(worldCamPos - pixelPosWorld);
    vec3 lightToPointDir = normalize(-lightForwardWorld);
    vec3 reflected       = normalize( -reflect(lightToPointDir, pixelNormalWorld) );
    float specDot        = max(0.0, dot(reflected, pointToCamDir));
    float specShin       = pow(specDot, pixelShininess);
    specShin = clamp(specShin, 0.0, 1.0);
    vec3 lightSpecular   = specShin * lightDot * lightIntensity * lightColor;

    return lightDiffuse + lightSpecular;
}
