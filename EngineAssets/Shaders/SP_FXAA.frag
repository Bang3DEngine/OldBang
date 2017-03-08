#define BANG_SP
#define BANG_FRAGMENT
#include "Uniforms.glsl"

in vec2 B_InUv;
in vec4 B_position_raw_vout_fin;

out vec4 fragColor;

/*********************************************
This was copied from:
    http://blog.simonrodriguez.fr/articles/30-07-2016_implementing_fxaa.html

Very nice explanation and implementation of FXAA!
*********************************************/

#define EDGE_THRESHOLD_MIN 0.0312
#define EDGE_THRESHOLD_MAX 0.125
#define SUBPIXEL_QUALITY 0.75
#define ITERATIONS 12

float rgb2luma(vec3 rgb)
{
    return sqrt(dot(rgb, vec3(0.299, 0.587, 0.114)));
}

void main()
{
  vec2 uv = B_ScreenUv;
  vec2 ps = B_ScreenStep;
  vec3 colorCenter = B_SampleColor().rgb;

  // Get lumas around
  float lumaM = rgb2luma(colorCenter);
  float lumaS = rgb2luma(B_SampleColor(uv + ps * vec2( 0,-1)).rgb);
  float lumaN = rgb2luma(B_SampleColor(uv + ps * vec2( 0, 1)).rgb);
  float lumaW = rgb2luma(B_SampleColor(uv + ps * vec2(-1, 0)).rgb);
  float lumaE = rgb2luma(B_SampleColor(uv + ps * vec2( 1, 0)).rgb);

  // Find the maximum and minimum luma around the current fragment.
  float lumaMin = min(lumaM, min(min(lumaS,lumaN), min(lumaW,lumaE)));
  float lumaMax = max(lumaM, max(max(lumaS,lumaN), max(lumaW,lumaE)));
  float lumaRange = lumaMax - lumaMin; // Compute the delta.

  // If the luma variation is lower that a threshold (or if we are in a really
  // dark area), we are not on an edge, don't perform any AA.
  if (lumaRange < max(EDGE_THRESHOLD_MIN, lumaMax * EDGE_THRESHOLD_MAX))
  {
      fragColor = vec4(colorCenter, 1);
      return;
  }

  // Query the 4 remaining corners lumas.
  float lumaSW = rgb2luma(B_SampleColor(uv + ps * vec2(-1,-1)).rgb);
  float lumaNE = rgb2luma(B_SampleColor(uv + ps * vec2( 1, 1)).rgb);
  float lumaNW = rgb2luma(B_SampleColor(uv + ps * vec2(-1, 1)).rgb);
  float lumaSE = rgb2luma(B_SampleColor(uv + ps * vec2( 1,-1)).rgb);

  // Combine the four edges lumas (using intermediary variables for future
  // computations with the same values). And for corners.
  float lumaSN = lumaS + lumaN;
  float lumaWE = lumaW + lumaE;
  float lumaWCorners = lumaSW + lumaNW;
  float lumaSCorners = lumaSW + lumaSE;
  float lumaECorners = lumaSE + lumaNE;
  float lumaNCorners = lumaNE + lumaNW;

  // Compute an approx. of the gradient along the horizontal and vertical axis.
  float edgeHorizontal = abs(-2.0 * lumaW + lumaWCorners)  +
                         abs(-2.0 * lumaM + lumaSN ) * 2.0 +
                         abs(-2.0 * lumaE + lumaECorners);
  float edgeVertical   = abs(-2.0 * lumaN + lumaNCorners) +
                         abs(-2.0 * lumaM + lumaWE) * 2.0 +
                         abs(-2.0 * lumaS + lumaSCorners);

  // Is the local edge horizontal or vertical ?
  bool isHorizontal = (edgeHorizontal >= edgeVertical);

  // Select the two neighboring texels lumas in the opposite direction to the
  // local edge.
  float luma1 = isHorizontal ? lumaS : lumaW;
  float luma2 = isHorizontal ? lumaN : lumaE;
  // Compute gradients in this direction.
  float gradient1 = luma1 - lumaM;
  float gradient2 = luma2 - lumaM;

  // Which direction is the steepest ?
  bool is1Steepest = abs(gradient1) >= abs(gradient2);

  // Gradient in the corresponding direction, normalized.
  float gradientScaled = 0.25 * max( abs(gradient1), abs(gradient2) );

  // Choose the step size (one pixel) according to the edge direction.
  vec2 inverseScreenSize = 1.0 / B_ScreenSize;
  float stepLength = isHorizontal ? inverseScreenSize.y : inverseScreenSize.x;

  // Average luma in the correct direction.
  float lumaLocalAverage = 0.0;

  if (is1Steepest)
  {
      stepLength = - stepLength; // Switch the direction
      lumaLocalAverage = 0.5 * (luma1 + lumaM);
  }
  else { lumaLocalAverage = 0.5 * (luma2 + lumaM); }

  // Shift UV in the correct direction by half a pixel.
  vec2 currentUv = uv;
  if (isHorizontal) { currentUv.y += stepLength * 0.5; }
  else { currentUv.x += stepLength * 0.5; }

  // Compute offset (for each iteration step) in the right direction.
  vec2 offset = isHorizontal ? vec2(inverseScreenSize.x, 0.0) :
                               vec2(0.0, inverseScreenSize.y);

  // Compute UVs to explore on each side of the edge, orthogonally.
  vec2 uv1 = currentUv - offset;
  vec2 uv2 = currentUv + offset;

  // Read the lumas at both current extremities of the exploration segment, and
  // compute the delta wrt to the local average luma.
  float lumaEnd1 = rgb2luma(B_SampleColor(uv1).rgb);
  float lumaEnd2 = rgb2luma(B_SampleColor(uv2).rgb);
  lumaEnd1 -= lumaLocalAverage;
  lumaEnd2 -= lumaLocalAverage;

  // If the luma deltas at the current extremities are larger than the
  // local gradient, we have reached the side of the edge.
  bool reached1 = abs(lumaEnd1) >= gradientScaled;
  bool reached2 = abs(lumaEnd2) >= gradientScaled;
  bool reachedBoth = reached1 && reached2;

  // If the side is not reached, we continue to explore in this direction.
  if (!reached1) { uv1 -= offset; }
  if (!reached2) { uv2 += offset; }

  // If both sides have not been reached, continue to explore.
  float quality[12];
  quality[0] = quality[1] = quality[2] = quality[3] = quality[4] = 0;
  quality[5] = 1.5; quality[6] = quality[7] = quality[8] =  quality[9] = 2.0;
  quality[10] = 4.0; quality[11] = 8.0;

  if (!reachedBoth)
  {
      for (int i = 2; i < ITERATIONS; i++)
      {
          if (!reached1)
          {   // If needed, read luma in 1st direction, compute delta.
              lumaEnd1 = rgb2luma(B_SampleColor(uv1).rgb) - lumaLocalAverage;
          }

          if (!reached2)
          {   // If needed, read luma in opposite direction, compute delta.
              lumaEnd2 = rgb2luma(B_SampleColor(uv2).rgb) - lumaLocalAverage;
          }

          // If the luma deltas at the current extremities is larger than the
          // local gradient, we have reached the side of the edge.
          reached1 = abs(lumaEnd1) >= gradientScaled;
          reached2 = abs(lumaEnd2) >= gradientScaled;
          reachedBoth = reached1 && reached2;

          // If the side is not reached, we continue to explore in this
          // direction, with a variable quality.
          if (!reached1) { uv1 -= offset * quality[i]; }
          if (!reached2) { uv2 += offset * quality[i]; }

          if (reachedBoth) { break; } // If both sides have been reached, stop.
      }
  }

  // Compute the distances to each extremity of the edge.
  float distance1 = isHorizontal ? (uv.x - uv1.x) : (uv.y - uv1.y);
  float distance2 = isHorizontal ? (uv2.x - uv.x) : (uv2.y - uv.y);

  // In which direction is the extremity of the edge closer ?
  bool isDirection1 = distance1 < distance2;
  float distanceFinal = min(distance1, distance2);

  // Length of the edge.
  float edgeThickness = (distance1 + distance2);

  // UV offset: read in the direction of the closest side of the edge.
  float pixelOffset = - distanceFinal / edgeThickness + 0.5;
  // Is the luma at center smaller than the local average ?
  bool isLumaMSmaller = lumaM < lumaLocalAverage;

  // If the luma at center is smaller than at its neighbour,
  // the delta luma at each end should be positive (same variation).
  // (in the direction of the closer side of the edge.)
  bool correctVariation = ((isDirection1 ? lumaEnd1 : lumaEnd2) < 0.0) !=
                            isLumaMSmaller;

  // If the luma variation is incorrect, do not offset.
  float finalOffset = correctVariation ? pixelOffset : 0.0;

  // Sub-pixel shifting
  // Full weighted average of the luma over the 3x3 neighborhood.
  float lumaAverage = (1.0 / 12.0) *
                        (2.0 * (lumaSN + lumaWE) + lumaWCorners + lumaECorners);
  // Ratio of the delta between the global average and the center luma,
  // over the luma range in the 3x3 neighborhood.
  float subPixelOffset1 = clamp( abs(lumaAverage - lumaM) / lumaRange, 0, 1);
  float subPixelOffset2 = (-2.0 * subPixelOffset1 + 3.0) *
                                    subPixelOffset1 * subPixelOffset1;
  // Compute a sub-pixel offset based on this delta.
  float subPixelOffsetFinal = subPixelOffset2 * subPixelOffset2 *
                              SUBPIXEL_QUALITY;

  // Pick the biggest of the two offsets.
  finalOffset = max(finalOffset,subPixelOffsetFinal);

  vec2 finalUv = uv; // Compute the final UV coordinates.
  if(isHorizontal) { finalUv.y += finalOffset * stepLength; }
  else { finalUv.x += finalOffset * stepLength; }

  // Read the color at the new UV coordinates, and use it.
  vec3 finalColor = B_SampleColor(finalUv).rgb;
  fragColor = vec4(finalColor, 1);
}
