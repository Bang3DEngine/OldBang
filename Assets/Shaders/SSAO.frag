#include "PP.frag"

#define MAX_RANDOM_OFFSETS 32

uniform float B_SSAORadius = 1.0f;

uniform vec2 B_RandomRotationsUvMultiply;
uniform sampler2D B_RandomRotations;

uniform int B_NumRandomOffsets;
uniform vec3 B_RandomHemisphereOffsetsArray[MAX_RANDOM_OFFSETS];

void main()
{
    float depth   = B_SampleDepth();
    if (depth > 0.99999f) { B_GIn_Color = vec4( vec3(0), 1 ); return; }

    vec3 normal   = normalize( B_SampleNormal() );
    vec3 worldPos = B_ComputeWorldPosition(depth);
    vec3 viewPos  = (B_View * vec4(worldPos, 1)).xyz;

    vec2 vpUv = B_GetViewportUv(); // Get viewport uvs

    // Get random rotation vector
    vec3 randomRot = texture(B_RandomRotations,
                             vpUv * B_RandomRotationsUvMultiply).xyz;
    randomRot.xy = (randomRot.xy * 2.0f - 1.0f); // Map (0,1) to (-1,1)

    // Create basis to orient offset to normal, and rotated with randomRot
    vec3 tangent = normalize(randomRot - normal * dot(randomRot, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 fromWorldToNormal = mat3(tangent, bitangent, normal);

    float occlusionSum = 0.0;
    float totalOcclusion = 0.0;
    float SSAORadius   = B_SSAORadius * (1.0f + depth);
    float SSAORadiusSQ = (SSAORadius * SSAORadius);
    for (int i = 0; i < B_NumRandomOffsets; ++i) // Sample around!
    {
        // Get random hemisphere offset
        vec3 randomOffset = B_RandomHemisphereOffsetsArray[i];
        vec3 randomOffsetLocal = fromWorldToNormal * randomOffset;
        vec3 sampleWorldPos = worldPos + (randomOffsetLocal * SSAORadius);

        // Get depth in depth buffer
        vec4 projectedPos = B_ProjectionView * vec4(sampleWorldPos, 1);
        projectedPos.xyz /= projectedPos.w;
        projectedPos.xyz = projectedPos.xyz * 0.5 + 0.5;
        vec2 gbufferDepthUv = projectedPos.xy;
        float gbufferDepth = ( B_SampleDepth(gbufferDepthUv) );

        // Check if the sampled point is inside the sphere
        vec3 gbufferWorldPos = B_ComputeWorldPosition(gbufferDepth, gbufferDepthUv);
        vec3 sampleVsGBufferSampleDist = (sampleWorldPos - gbufferWorldPos);
        float sqDistSampleVsGBufferSample = dot(sampleVsGBufferSampleDist,
                                                sampleVsGBufferSampleDist);
        float isInsideSphere = 1.0 - smoothstep(0.0, SSAORadiusSQ,
                                                sqDistSampleVsGBufferSample); // ( sqDistSampleVsGBufferSample <= SSAORadiusSQ ) ? 1.0 : 0.0;

        // Finally determine if we are occluded or not for this sample
        float sampleWorldPosDepth = projectedPos.z;
        float deltaDepth = (sampleWorldPosDepth - gbufferDepth);
        float distanceFactor = length(randomOffsetLocal);
        // float distanceFactor = distance(sampleWorldPos, worldPos) / SSAORadius;
        distanceFactor = ( (1.0 / pow((distanceFactor + 0.1), 2) ) ) / 10.0;
        float Bias       = 0.0001; // 0.000001;
        float occluded   = step(Bias, deltaDepth); // smoothstep(Bias, 1.0, deltaDepth); // step(Bias, deltaDepth);
        float occlusionApport = distanceFactor * occluded * isInsideSphere;

        occlusionSum   += occlusionApport; // Accumulate occlusion
        totalOcclusion += distanceFactor;
    }
    float occlusion = (occlusionSum / totalOcclusion);
    // occlusion = pow(occlusion, 2);

    B_GIn_Color = vec4( vec3(occlusion), 1 );
}
