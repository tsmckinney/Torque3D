#include "core/rendering/shaders/gl/torque.glsl"
#include "core/rendering/shaders/gl/hlslCompat.glsl"
#include "shadergen:/autogenConditioners.h"
#include "core/rendering/shaders/postFX/gl/postFX.glsl"

layout (binding = 0) uniform sampler2D inputTex;
uniform float distCoeff;
uniform float cubeDistort;
uniform vec3 colorDistort;

layout (location = 0) out vec4 OUT_col;

void main()
{
   vec2 tex = IN_uv0;

   float f = 0;
   float r2 = (tex.x - 0.5) * (tex.x - 0.5) + (tex.y - 0.5) * (tex.y - 0.5);       

   // Only compute the cubic distortion if necessary.
   if ( cubeDistort == 0.0 )
      f = 1 + r2 * distCoeff;
   else
      f = 1 + r2 * (distCoeff + cubeDistort * sqrt(r2));

   // Distort each color channel seperately to get a chromatic distortion effect.
   vec3 outColor;
   vec3 distort = vec3(f) + colorDistort;

   for ( int i=0; i < 3; i++ )
   {
      float x = distort[i] * ( tex.x - 0.5 ) + 0.5;
      float y = distort[i] * ( tex.y - 0.5 ) + 0.5;
      outColor[i] = textureLod( inputTex, vec2(x,y), 0. )[i];
   }
   OUT_col = hdrEncode( vec4(outColor.rgb, 1.) );
}
