//-----------------------------------------------------------------------------
// Copyright (c) 2012 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#include "core/rendering/shaders/gl/torque.glsl"
#include "core/rendering/shaders/gl/hlslCompat.glsl"
#include "shadergen:/autogenConditioners.h"
#include "core/rendering/shaders/postFX/gl/postFX.glsl"

layout(binding = 0) uniform sampler2D inputTex;
uniform float sharpness;
uniform vec2 targetSize;

layout (location = 0) out vec4 OUT_col;

void main()
{
   vec2 st = 1.0 / targetSize.xy;

   vec3 texA = texture( inputTex, IN_uv0 + vec2(-st.x, -st.y) * 1.5 ).rgb;
   vec3 texB = texture( inputTex, IN_uv0 + vec2( st.x, -st.y) * 1.5 ).rgb;
   vec3 texC = texture( inputTex, IN_uv0 + vec2(-st.x,  st.y) * 1.5 ).rgb;
   vec3 texD = texture( inputTex, IN_uv0 + vec2( st.x,  st.y) * 1.5 ).rgb;

   vec3 around = 0.25 * (texA + texB + texC + texD);
   vec3 center  = texture( inputTex, IN_uv0 ).rgb;

   vec3 col = center + (center - around) * sharpness;
   
   OUT_col = hdrEncode( vec4(col, 1.0) );
}
