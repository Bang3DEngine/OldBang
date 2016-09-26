vec3 GetPointLightColorApportation(vec3  pixelPosWorld,
                                   vec3  pixelNormalWorld,
                                   vec3  pixelDiffColor,
                                   float pixelShininess,
                                   vec3  lightPosWorld,
                                   float lightIntensity,
                                   float lightRange,
                                   vec3  lightColor,
                                   vec3  camPosWorld)
{
    vec3 dir = normalize(lightPosWorld - pixelPosWorld);
    float lightDot = dot(pixelNormalWorld, dir);
    if (lightDot <= 0.0f) discard;

    // Linear Attenuation
    float d = distance(lightPosWorld, pixelPosWorld) ;
    float linearAtt = 1.0f - d / lightRange; // 0 is light's pos, 1 is max range
    if (linearAtt <= 0.0f) discard;
    linearAtt = min(linearAtt, 1.0);

    float intensityAtt = lightIntensity * linearAtt;

    // DIFFUSE
    vec3 lightDiff = pixelDiffColor * lightDot * intensityAtt * lightColor;

    // SPECULAR
    vec3 worldCamPos     = camPosWorld;
    vec3 pointToCamDir   = normalize(worldCamPos - pixelPosWorld);
    vec3 lightToPointDir = normalize(lightPosWorld - pixelPosWorld);
    vec3 reflected       = -reflect(lightToPointDir, pixelNormalWorld);
    float specDot        = dot(reflected, pointToCamDir);
    if (specDot <= 0.0f) discard;

    float specShin = pow(specDot, pixelShininess);
    specShin = min(specShin, 1.0);

    vec3 lightSpecular   = specShin * lightDot * intensityAtt * lightIntensity * lightColor;

    return lightDiff + lightSpecular;
}
