#include "UI.frag"

uniform bool B_X;
uniform bool  B_usingDistField;

uniform float B_textBlurriness;
uniform float B_textAlphaThreshold;

uniform float B_outlineWidth;
uniform vec4  B_outlineColor;
uniform float B_outlineBlurriness;

void main()
{
    vec2 charAtlasUv = B_FIn_Uv;
    if (B_X) { B_GIn_Color = vec4(charAtlasUv, 0, 1); return; }

    vec4 thisColor = texture2D(B_Texture0, charAtlasUv);
    if (B_usingDistField)
    {
        float dist = thisColor.r;

        float lowerCharLimit = B_textAlphaThreshold - B_textBlurriness;
        if (dist <= lowerCharLimit)
        {
            B_GIn_Color = B_MaterialDiffuseColor; // Just opaque char
        }
        else
        {
            // Outside opaque character zon
            float upperCharLimit = B_textAlphaThreshold + B_textBlurriness;
            /*
            if (B_outlineWidth > 0)
            {
                // Outline zone
                float outlineBlur  = B_outlineBlurriness;
                float outlineLimit = B_textAlphaThreshold + B_outlineWidth;
                float outlineLowerLimit = outlineLimit - outlineBlur;
                float outlineUpperLimit = outlineLimit + outlineBlur;

                float outlineAlpha = 1.0f - smoothstep(outlineLowerLimit,
                                                       outlineUpperLimit,
                                                       dist);
                B_GIn_Color = vec4(B_outlineColor.rgb,
                                   B_outlineColor.a * outlineAlpha);


                if (dist <= upperCharLimit)
                {
                    // Blend outline color with character color
                    float charAlpha = 1.0f - smoothstep(lowerCharLimit,
                                                        upperCharLimit, dist);
                    vec4 charColor = vec4(B_MaterialDiffuseColor.rgb, charAlpha);
                    float blendAlpha = max(charAlpha, B_GIn_Color.a);
                    B_GIn_Color = vec4(mix(B_GIn_Color.rgb, charColor.rgb,
                                           charAlpha),
                                       blendAlpha);
                }
            }
            else
            */
            {
                // No outline, but character has smoothstep alpha
                float charAlpha = 1.0f-smoothstep(lowerCharLimit,
                                                  upperCharLimit, dist);
                B_GIn_Color = vec4(B_MaterialDiffuseColor.rgb, charAlpha);
            }
        }
    }
    else
    {
        B_GIn_Color = vec4(B_MaterialDiffuseColor.rgb, thisColor.a);
    }
}
