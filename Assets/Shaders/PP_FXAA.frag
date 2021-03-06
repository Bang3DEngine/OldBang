#include "PP.frag"

#define FXAA_SPAN_MAX 4.0
#define FXAA_REDUCE_MUL   (1.0/FXAA_SPAN_MAX)
#define FXAA_REDUCE_MIN   (1.0/128.0)
#define FXAA_SUBPIX_SHIFT (1.0/4.0)

void main()
{
    vec2 uv = B_GetViewportUv();

    vec3 rgbNW = B_SampleColor(uv + vec2(-1, -1) * B_GetViewportStep()).rgb;
    vec3 rgbNE = B_SampleColor(uv + vec2( 1, -1) * B_GetViewportStep()).rgb;
    vec3 rgbSW = B_SampleColor(uv + vec2(-1,  1) * B_GetViewportStep()).rgb;
    vec3 rgbSE = B_SampleColor(uv + vec2( 1,  1) * B_GetViewportStep()).rgb;
    vec3 rgbM  = B_SampleColor(uv).rgb;
    vec3 luma = vec3(0.299, 0.587, 0.114);

    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM = dot(rgbM, luma);
    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) *
                                (0.25 * FXAA_REDUCE_MUL),
                          FXAA_REDUCE_MIN);

    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = min(
       vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX),
       max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),  dir * rcpDirMin)
     ) * B_GetViewportStep();

    vec3 rgbA = 0.5 * (
            B_SampleColor(uv + dir * (1.0 / 3.0 - 0.5)).rgb +
            B_SampleColor(uv + dir * (2.0 / 3.0 - 0.5)).rgb);
    vec3 rgbB = rgbA * 0.5 + 0.25 * (
            B_SampleColor(uv + dir * -0.5).rgb +
            B_SampleColor(uv + dir *  0.5).rgb);

    float lumaB = dot(rgbB, luma);
    vec3 col = ((lumaB < lumaMin) || (lumaB > lumaMax)) ? rgbA : rgbB;
    B_GIn_Color = vec4(col, 1);
}
