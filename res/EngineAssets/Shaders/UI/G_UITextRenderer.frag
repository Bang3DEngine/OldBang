#include "G.frag"

uniform vec2  B_fontAtlasSize;

uniform bool  B_usingDistField;

uniform float B_textBlurriness;
uniform float B_textAlphaThreshold;

uniform float B_outlineWidth;
uniform vec4  B_outlineColor;
uniform float B_outlineBlurriness;

void Main()
{
    B_UsePlainDiffuseColor = true;

    vec2 charAtlasUv = B_FIn_Uv;

    vec4 thisColor = texture2D(B_Texture0, charAtlasUv);
    if (B_usingDistField)
    {
        float dist = thisColor.r;

        float lowerCharLimit = B_textAlphaThreshold - B_textBlurriness;
        if (dist <= lowerCharLimit)
        {
            B_FOut.Color = B_FOut.Diffuse; // Just opaque char
        }
        else
        {
            // Outside opaque character zone

            float upperCharLimit = B_textAlphaThreshold + B_textBlurriness;
            if (B_outlineWidth > 0)
            {
                // Outline zone
                float outlineBlur  = B_outlineBlurriness;
                float outlineLimit = B_textAlphaThreshold + B_outlineWidth;
                float outlineLowerLimit = outlineLimit - outlineBlur;
                float outlineUpperLimit = outlineLimit + outlineBlur;

                if (dist > outlineUpperLimit) { discard; }

                float outlineAlpha = 1.0f - smoothstep(outlineLowerLimit,
                                                       outlineUpperLimit,
                                                       dist);
                B_FOut.Color = vec4(B_outlineColor.rgb,
                                    B_outlineColor.a * outlineAlpha);


                if (dist <= upperCharLimit)
                {
                    // Blend outline color with character color
                    float charAlpha = 1.0f - smoothstep(lowerCharLimit,
                                                        upperCharLimit, dist);
                    vec4 charColor = vec4(B_FOut.Diffuse.rgb, charAlpha);
                    float blendAlpha = max(charAlpha, B_FOut.Color.a);
                    B_FOut.Color = vec4(mix(B_FOut.Color.rgb, charColor.rgb,
                                           charAlpha),
                                       blendAlpha);
                }
            }
            else
            {
                // No outline, but character has smoothstep alpha
                if (dist > upperCharLimit) { discard; }

                float charAlpha = 1.0f-smoothstep(lowerCharLimit,
                                                  upperCharLimit, dist);
                B_FOut.Color = vec4(B_FOut.Diffuse.rgb, charAlpha);
            }
        }
    }
    else
    {
        B_FOut.Color = vec4(B_FOut.Diffuse.rgb, thisColor.a);
    }
}
